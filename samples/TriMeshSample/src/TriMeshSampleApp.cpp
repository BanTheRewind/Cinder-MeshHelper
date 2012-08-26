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

#include "cinder/app/AppBasic.h"
#include "cinder/Arcball.h"
#include "cinder/Camera.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/params/Params.h"
#include "cinder/TriMesh.h"

class TriMeshSampleApp : public ci::app::AppBasic 
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
	enum {
		MESH_TYPE_CUBE, 
		MESH_TYPE_SPHERE, 
		MESH_TYPE_CYLINDER, 
		MESH_TYPE_CONE, 
		MESH_TYPE_CIRCLE, 
		MESH_TYPE_SQUARE, 
		MESH_TYPE_CUSTOM
	} typedef MeshType;

	void						createMeshes();
	ci::TriMesh					mCircle;
	ci::TriMesh					mCone;
	ci::TriMesh					mCube;
	ci::TriMesh					mCustom;
	ci::TriMesh					mCylinder;
	ci::TriMesh					mSphere;
	ci::TriMesh					mSquare;

	ci::gl::VboMesh				mSphereVbo;
	
	int32_t						mMeshIndex;
	std::vector<std::string>	mMeshTitles;

	int32_t						mNumSegments;
	int32_t						mNumSegmentsPrev;

	ci::Vec3f					mScale;

	ci::Arcball					mArcball;
	ci::CameraPersp				mCamera;

	ci::gl::Light				*mLight;
	bool						mLightEnabled;

	ci::gl::Texture				mTexture;
	bool						mTextureEnabled;

	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGl		mParams;
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

void TriMeshSampleApp::createMeshes()
{
	//mCircle		= MeshHelper::createCircleTriMesh( mNumSegments );
	//mCone		= MeshHelper::createConeTriMesh( mNumSegments );
	//mCube		= MeshHelper::createCubeTriMesh();
	//mCylinder	= MeshHelper::createCylinderTriMesh( mNumSegments );
	mSphere		= MeshHelper::createSphereTriMesh( mNumSegments );
	mSphereVbo	= MeshHelper::createSphereVboMesh( mNumSegments );
	//mSquare		= MeshHelper::createSquareTriMesh();
	return;
	/////////////////////////////////////////////////////////////////////////////

	vector<size_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	for ( int32_t y = 0; y < mNumSegments; y++ ) {
		for ( int32_t x = 0; x < mNumSegments; x++ ) {
			texCoords.push_back( Vec2f( (float)x / (float)mNumSegments, (float)y / (float)mNumSegments ) );

			int32_t xn = x + 1 >= mNumSegments ? 0 : 1;
			int32_t yn = y + 1 >= mNumSegments ? 0 : 1;
			indices.push_back( x + mNumSegments * y );
			indices.push_back( ( x + xn ) + mNumSegments * y);
			indices.push_back( ( x + xn ) + mNumSegments * ( y + yn ) );
			indices.push_back( x + mNumSegments * ( y + yn ) );
			indices.push_back( ( x + xn ) + mNumSegments * ( y + yn ) );
			indices.push_back( x + mNumSegments * y );
		}
	}

	float halfHeight	= (float)mNumSegments * 0.5f;
	float halfWidth		= (float)mNumSegments * 0.5f;
	
	Vec3f scale( Vec2f::one() * ( 1.0f / (float)mNumSegments ), 1.0f );
	Vec3f offset( -0.5f, 0.5f, 0.0f );

	for ( int32_t y = 0; y < mNumSegments; y++ ) {
		for ( int32_t x = 0; x < mNumSegments; x++ ) {
			float value = randFloat();

			Vec3f position( (float)x - halfWidth, value, (float)y - halfHeight );
			positions.push_back( position /** scale + offset*/ );

			normals.push_back( Vec3f::zero() );
		}
	}

	for ( int32_t y = 0; y < mNumSegments - 1; y++ ) {
		for ( int32_t x = 0; x < mNumSegments - 1; x++ ) {
			Vec3f vert0 = positions[ indices[ ( x + mNumSegments * y ) * 6 ] ];
			Vec3f vert1 = positions[ indices[ ( ( x + 1 ) + mNumSegments * y ) * 6 ] ];
			Vec3f vert2 = positions[ indices[ ( ( x + 1 ) + mNumSegments * ( y + 1 ) ) * 6 ] ];
			normals[ x + mNumSegments * y ] = Vec3f( ( vert1 - vert0 ).cross( vert1 - vert2 ).normalized() );
		}
	}

	mCustom = MeshHelper::createTriMesh( indices, positions, normals, texCoords );
}

void TriMeshSampleApp::draw()
{
	gl::setViewport( getWindowBounds() );
	gl::setMatrices( mCamera );
	gl::clear( ColorAf::gray( 0.6f ) );

	glMultMatrixf( mArcball.getQuat() );

	if ( mLightEnabled ) {
		gl::enable( GL_LIGHTING );
	}
	if ( mTextureEnabled && mTexture ) {
		gl::enable( GL_TEXTURE_2D );
		mTexture.bind();
	}

	gl::pushMatrices();
	gl::scale( mScale );
	
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
	case MESH_TYPE_SPHERE:
		gl::pushMatrices();
		gl::translate( -1.25f, 0.0f );
		gl::draw( mSphereVbo );
		gl::popMatrices();
		gl::pushMatrices();
		gl::translate( 1.25f, 0.0f );
		gl::draw( mSphere );
		gl::popMatrices();
		break;
	case MESH_TYPE_SQUARE:
		gl::draw( mSquare );
		break;
	}
	
	gl::popMatrices();

	if ( mTextureEnabled && mTexture ) {
		mTexture.unbind();
		gl::disable( GL_TEXTURE_2D );
	}
	if ( mLightEnabled ) {
		gl::disable( GL_LIGHTING );
	}

	mParams.draw();
}

void TriMeshSampleApp::mouseDown( MouseEvent event )
{
	mArcball.mouseDown( event.getPos() );
}

void TriMeshSampleApp::mouseDrag( MouseEvent event )
{
	mArcball.mouseDrag( event.getPos() );
}

void TriMeshSampleApp::mouseWheel( MouseEvent event )
{
	Vec3f eye = mCamera.getEyePoint();
	eye.z += event.getWheelIncrement();
	mCamera.setEyePoint( eye );
}

void TriMeshSampleApp::screenShot()
{
	writeImage( getAppPath() / ( "frame_" + toString( getElapsedFrames() ) + ".png" ), copyWindowSurface() );
}

void TriMeshSampleApp::setup()
{
	setFrameRate( 60.0f );
	setWindowSize( 800, 600 );

	glShadeModel( GL_SMOOTH );
	gl::enable( GL_POLYGON_SMOOTH );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	gl::enable( GL_NORMALIZE );
	gl::enableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();

	mTexture = gl::Texture( loadImage( loadResource( RES_TEXTURE ) ) );

	mFrameRate			= 0.0f;
	mFullScreen			= false;
	mLightEnabled		= true;
	mMeshIndex			= 1;
	mNumSegments		= 64;
	mNumSegmentsPrev	= mNumSegments;
	mTextureEnabled		= true;
	
	mArcball = Arcball( getWindowSize() );
	mArcball.setRadius( (float)getWindowHeight() * 0.5f );
	
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 60.0f, 0.001f, 5000.0f );
	mCamera.lookAt( Vec3f( 0.0f, 0.0f, -500.0f ), Vec3f::zero() );

	mScale = Vec3f::one() * 100.0f;

	mLight = new gl::Light( gl::Light::DIRECTIONAL, 0 );
	mLight->setAmbient( ColorAf( Colorf::gray( 0.2f ), 1.0f ) );
	mLight->setDiffuse( ColorAf::white() );
	mLight->enable();
	
	mMeshTitles.push_back( "Cube" );
	mMeshTitles.push_back( "Sphere" );
	mMeshTitles.push_back( "Cylinder" );
	mMeshTitles.push_back( "Cone" );
	mMeshTitles.push_back( "Circle" );
	mMeshTitles.push_back( "Square" );
	mMeshTitles.push_back( "Custom" );

	mParams = params::InterfaceGl( "Params", Vec2i( 180, 300 ) );
	mParams.addParam( "Frame rate",			&mFrameRate,									"", true									);
	mParams.addSeparator();
	mParams.addParam( "Circle segments",	&mNumSegments,									"keyDecr=s keyIncr=S min=3 max=1024 step=1"	);
	mParams.addParam( "Enable light",		&mLightEnabled,									"key=l"										);
	mParams.addParam( "Enable texture",		&mTextureEnabled,								"key=t"										);
	mParams.addParam( "Mesh type",			mMeshTitles, &mMeshIndex,						"keyDecr=m keyIncr=M"						);
	mParams.addParam( "Scale",				&mScale																						);
	mParams.addSeparator();
	mParams.addParam( "Full screen",		&mFullScreen,									"key=f"										);
	mParams.addButton( "Screen shot",		bind( &TriMeshSampleApp::screenShot, this ),	"key=space"									);
	mParams.addButton( "Quit",				bind( &TriMeshSampleApp::quit, this ),			"key=q"										);

	createMeshes();
}

void TriMeshSampleApp::shutdown()
{
	if ( mLight != 0 ) {
		mLight = 0;
	}
}

void TriMeshSampleApp::update()
{
	mFrameRate = getAverageFps();

	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
	}

	if ( mNumSegmentsPrev != mNumSegments ) {
		createMeshes();
		mNumSegmentsPrev = mNumSegments;
	}

	mLight->update( mCamera );
}

CINDER_APP_BASIC( TriMeshSampleApp, RendererGl )
