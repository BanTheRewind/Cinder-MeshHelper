/*
* 
* Copyright (c) 2012, Ban the Rewind
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or 
* without modification, are permitted provided that the following 
* conditions are met:
* 
* Redistributions of source code must retain the above copyright 
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright 
* notice, this list of conditions and the following disclaimer in 
* the documentation and/or other materials provided with the 
* distribution.
* 
* Neither the name of the Ban the Rewind nor the names of its 
* contributors may be used to endorse or promote products 
* derived from this software without specific prior written 
* permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
*/

#include "cinder/app/AppNative.h"
#include "cinder/Arcball.h"
#include "cinder/Camera.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/params/Params.h"

class VboMeshSampleApp : public ci::app::AppNative 
{
public:
	void						draw();
	void						mouseDown( ci::app::MouseEvent event );
	void						mouseDrag( ci::app::MouseEvent event );
	void						mouseWheel( ci::app::MouseEvent event );
	void						setup();
	void						shutdown();
	void						update();
private:

	// Enumerate mesh types
	enum {
		MESH_TYPE_CUBE, 
		MESH_TYPE_SPHERE, 
		MESH_TYPE_CYLINDER, 
		MESH_TYPE_CONE, 
		MESH_TYPE_TORUS, 
		MESH_TYPE_ICOSAHEDRON, 
		MESH_TYPE_CIRCLE, 
		MESH_TYPE_SQUARE, 
		MESH_TYPE_RING, 
		MESH_TYPE_CUSTOM
	} typedef MeshType;

	// The VboMeshes
	void						createMeshes();
	ci::gl::VboMesh				mCircle;
	ci::gl::VboMesh				mCone;
	ci::gl::VboMesh				mCube;
	ci::gl::VboMesh				mCustom;
	ci::gl::VboMesh				mCylinder;
	ci::gl::VboMesh				mIcosahedron;
	ci::gl::VboMesh				mRing;
	ci::gl::VboMesh				mSphere;
	ci::gl::VboMesh				mSquare;
	ci::gl::VboMesh				mTorus;
	
	// For selecting mesh type from params
	int32_t						mMeshIndex;
	std::vector<std::string>	mMeshTitles;

	// Mesh resolution
	int32_t						mDivision;
	int32_t						mDivisionPrev;
	ci::Vec3i					mResolution;
	ci::Vec3i					mResolutionPrev;

	// Mesh scale
	ci::Vec3f					mScale;

	// Camera
	ci::Arcball					mArcball;
	ci::CameraPersp				mCamera;

	// Lighting
	ci::gl::Light				*mLight;
	bool						mLightEnabled;

	// Texture map
	ci::gl::Texture				mTexture;
	bool						mTextureEnabled;

	// Params and utilities
	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGl		mParams;
	bool						mWireframe;
	void						screenShot();
};

#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/Surface.h"
#include "cinder/Utilities.h"
#include "MeshHelper.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// Creates VboMeshes
void VboMeshSampleApp::createMeshes()
{
	// Use the MeshHelper to generate primitives
	mCircle			= gl::VboMesh( MeshHelper::createCircle( mResolution.xy() ) );
	mCone			= gl::VboMesh( MeshHelper::createCylinder( mResolution.xy(), 0.0f, 1.0f, false, true ) );
	mCube			= gl::VboMesh( MeshHelper::createCube( mResolution ) );
	mCylinder		= gl::VboMesh( MeshHelper::createCylinder( mResolution.xy() ) );
	mIcosahedron	= gl::VboMesh( MeshHelper::createIcosahedron( mDivision ) );
	mRing			= gl::VboMesh( MeshHelper::createRing( mResolution.xy() ) );
	mSphere			= gl::VboMesh( MeshHelper::createSphere( mResolution.xy() ) );
	mSquare			= gl::VboMesh( MeshHelper::createSquare( mResolution.xy() ) );
	mTorus			= gl::VboMesh( MeshHelper::createTorus( mResolution.xy() ) );
	
	/////////////////////////////////////////////////////////////////////////////
	// Custom mesh

	// Declare vectors
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	// Mesh dimensions
	float halfHeight	= (float)mResolution.x * 0.5f;
	float halfWidth		= (float)mResolution.y * 0.5f;
	float unit			= 3.0f / (float)mResolution.x;
	Vec3f scale( unit, 0.5f, unit );
	Vec3f offset( -0.5f, 0.5f, 0.0f );

	// Iterate through rows and columns using segment count
	for ( int32_t y = 0; y < mResolution.y; y++ ) {
		for ( int32_t x = 0; x < mResolution.x; x++ ) {

			// Set texture coordinate in [ 0 - 1, 0 - 1 ] range
			Vec2f texCoord( (float)x / (float)mResolution.x, (float)y / (float)mResolution.y );
			texCoords.push_back( texCoord );

			// Use random value for Y position
			float value = randFloat();

			// Set vertex position
			Vec3f position( (float)x - halfWidth, value, (float)y - halfHeight );
			positions.push_back( position * scale + offset );

			// Add a default normal for now (we'll calculate this down below)
			normals.push_back( Vec3f::zero() );

			// Add indices to form quad from two triangles
			int32_t xn = x + 1 >= mResolution.x ? 0 : 1;
			int32_t yn = y + 1 >= mResolution.y ? 0 : 1;
			indices.push_back( x + mResolution.x * y );
			indices.push_back( ( x + xn ) + mResolution.x * y);
			indices.push_back( ( x + xn ) + mResolution.x * ( y + yn ) );
			indices.push_back( x + mResolution.x * ( y + yn ) );
			indices.push_back( ( x + xn ) + mResolution.x * ( y + yn ) );
			indices.push_back( x + mResolution.x * y );
		}
	}

	// Iterate through again to set normals
	for ( int32_t y = 0; y < mResolution.y - 1; y++ ) {
		for ( int32_t x = 0; x < mResolution.x - 1; x++ ) {
			Vec3f vert0 = positions[ indices[ ( x + mResolution.x * y ) * 6 ] ];
			Vec3f vert1 = positions[ indices[ ( ( x + 1 ) + mResolution.x * y ) * 6 ] ];
			Vec3f vert2 = positions[ indices[ ( ( x + 1 ) + mResolution.x * ( y + 1 ) ) * 6 ] ];
			normals[ x + mResolution.x * y ] = Vec3f( ( vert1 - vert0 ).cross( vert1 - vert2 ).normalized() );
		}
	}

	// Use the MeshHelper to create a VboMesh from our vectors
	mCustom = gl::VboMesh( MeshHelper::create( indices, positions, normals, texCoords ) );
}

void VboMeshSampleApp::draw()
{
	// Set up window
	gl::setViewport( getWindowBounds() );
	gl::setMatrices( mCamera );
	gl::clear( ColorAf::gray( 0.6f ) );

	// Use arcball to rotate model view
	glMultMatrixf( mArcball.getQuat() );

	// Enabled lighting, texture mapping, wireframe
	if ( mLightEnabled ) {
		gl::enable( GL_LIGHTING );
	}
	if ( mTextureEnabled && mTexture ) {
		gl::enable( GL_TEXTURE_2D );
		mTexture.bind();
	}
	if ( mWireframe ) {
		gl::enableWireframe();
	}
	
	// Apply scale
	gl::pushMatrices();
	gl::scale( mScale );
	
	// Draw selected mesh
	switch ( (MeshType)mMeshIndex ) {
	case MESH_TYPE_CIRCLE:
		gl::draw( mCircle );
		break;
	case MESH_TYPE_CONE:
		gl::draw( mCone );
		break;
	case MESH_TYPE_CUBE:
		gl::draw( mCube );
		break;
	case MESH_TYPE_CUSTOM:
		gl::draw( mCustom );
		break;
	case MESH_TYPE_CYLINDER:
		gl::draw( mCylinder );
		break;
	case MESH_TYPE_ICOSAHEDRON:
		gl::draw( mIcosahedron );
		break;
	case MESH_TYPE_RING:
		gl::draw( mRing );
		break;
	case MESH_TYPE_SPHERE:
		gl::draw( mSphere );
		break;
	case MESH_TYPE_SQUARE:
		gl::draw( mSquare );
		break;
	case MESH_TYPE_TORUS:
		gl::draw( mTorus );
		break;
	}
	
	// End scale
	gl::popMatrices();

	// Disable wireframe, texture mapping, lighting
	if ( mWireframe ) {
		gl::disableWireframe();
	}
	if ( mTextureEnabled && mTexture ) {
		mTexture.unbind();
		gl::disable( GL_TEXTURE_2D );
	}
	if ( mLightEnabled ) {
		gl::disable( GL_LIGHTING );
	}

	// Draw params GUI
	mParams.draw();
}

void VboMeshSampleApp::mouseDown( MouseEvent event )
{
	// Rotate with arcball
	mArcball.mouseDown( event.getPos() );
}

void VboMeshSampleApp::mouseDrag( MouseEvent event )
{
	// Rotate with arcball
	mArcball.mouseDrag( event.getPos() );
}

void VboMeshSampleApp::mouseWheel( MouseEvent event )
{
	// Zoom in/out with mouse wheel
	Vec3f eye = mCamera.getEyePoint();
	eye.z += event.getWheelIncrement() * 0.1f;
	mCamera.setEyePoint( eye );
}

// Saves screenshot
void VboMeshSampleApp::screenShot()
{
	writeImage( getAppPath() / ( "frame_" + toString( getElapsedFrames() ) + ".png" ), copyWindowSurface() );
}

void VboMeshSampleApp::setup()
{
	// Setting an unrealistically high frame rate effectively
	// disables frame rate limiting
	setFrameRate( 10000.0f );
	setWindowSize( 800, 600 );

	// Set up OpenGL to work with default lighting
	glShadeModel( GL_SMOOTH );
	gl::enable( GL_POLYGON_SMOOTH );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	gl::enable( GL_NORMALIZE );
	gl::enableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();

	// Load the texture map
	mTexture = gl::Texture( loadImage( loadResource( RES_TEXTURE ) ) );

	// Define properties
	mDivision			= 1;
	mDivisionPrev		= mDivision;
	mFrameRate			= 0.0f;
	mFullScreen			= false;
	mLightEnabled		= true;
	mMeshIndex			= 0;
	mResolution			= Vec3i( 12, 12, 12 );
	mResolutionPrev		= mResolution;
	mScale				= Vec3f::one();
	mTextureEnabled		= true;
	mWireframe			= false;
	
	// Set up the arcball
	mArcball = Arcball( getWindowSize() );
	mArcball.setRadius( (float)getWindowHeight() * 0.5f );
	
	// Set up the camera
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 60.0f, 0.0001f, 10.0f );
	mCamera.lookAt( Vec3f( 0.0f, 0.0f, -5.0f ), Vec3f::zero() );

	// Set up the light
	mLight = new gl::Light( gl::Light::DIRECTIONAL, 0 );
	mLight->setAmbient( ColorAf::white() );
	mLight->setDiffuse( ColorAf::white() );
	mLight->setDirection( Vec3f::one() );
	mLight->setPosition( Vec3f::one() * -1.0f );
	mLight->setSpecular( ColorAf::white() );
	mLight->enable();
	
	// Define the mesh titles for the params GUI
	mMeshTitles.push_back( "Cube" );
	mMeshTitles.push_back( "Sphere" );
	mMeshTitles.push_back( "Cylinder" );
	mMeshTitles.push_back( "Cone" );
	mMeshTitles.push_back( "Torus" );
	mMeshTitles.push_back( "Icosahedron" );
	mMeshTitles.push_back( "Circle" );
	mMeshTitles.push_back( "Square" );
	mMeshTitles.push_back( "Ring" );
	mMeshTitles.push_back( "Custom" );

	// Set up the params GUI
	mParams = params::InterfaceGl( "Params", Vec2i( 200, 320 ) );
	mParams.addParam( "Frame rate",		&mFrameRate,									"", true									);
	mParams.addSeparator();
	mParams.addParam( "Enable light",	&mLightEnabled,									"key=l"										);
	mParams.addParam( "Enable texture",	&mTextureEnabled,								"key=t"										);
	mParams.addParam( "Ico division",	&mDivision,										"keyDecr=d keyIncr=D min=1 max=10 step=1"	);
	mParams.addParam( "Mesh type",		mMeshTitles, &mMeshIndex,						"keyDecr=m keyIncr=M"						);
	mParams.addParam( "Resolution X",	&mResolution.x,									"keyDecr=x keyIncr=X min=1 max=1024 step=1"	);
	mParams.addParam( "Resolution Y",	&mResolution.y,									"keyDecr=y keyIncr=Y min=1 max=1024 step=1"	);
	mParams.addParam( "Resolution Z",	&mResolution.z,									"keyDecr=z keyIncr=Z min=1 max=1024 step=1"	);
	mParams.addParam( "Scale",			&mScale																						);
	mParams.addParam( "Wireframe",		&mWireframe,									"key=w"										);
	mParams.addSeparator();
	mParams.addParam( "Full screen",	&mFullScreen,									"key=f"										);
	mParams.addButton( "Screen shot",	bind( &VboMeshSampleApp::screenShot, this ),	"key=space"									);
	mParams.addButton( "Quit",			bind( &VboMeshSampleApp::quit, this ),			"key=q"										);

	// Generate meshes
	createMeshes();
}

void VboMeshSampleApp::shutdown()
{
	// Clean up
	if ( mLight != 0 ) {
		mLight = 0;
	}
}

void VboMeshSampleApp::update()
{
	// Update FPS
	mFrameRate = getAverageFps();

	// Toggle fullscreen
	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
	}

	// Reset the meshes if the segment count changes
	if ( mDivisionPrev	!= mDivision || 
		mResolutionPrev != mResolution ) {
		createMeshes();
		mDivisionPrev	= mDivision;
		mResolutionPrev = mResolution;
	}

	// Update light on every frame
	mLight->update( mCamera );
}

CINDER_APP_BASIC( VboMeshSampleApp, RendererGl )
