/*
* 
* Copyright (c) 2013, Ban the Rewind
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
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/params/Params.h"

class SkyBoxSampleApp : public ci::app::AppBasic 
{
public:
	void						draw();
	void						mouseDown( ci::app::MouseEvent event );
	void						mouseDrag( ci::app::MouseEvent event );
	void						prepareSettings( ci::app::AppBasic::Settings* settings );
	void						setup();
	void						update();
private:
	// Camera
	ci::Arcball					mArcball;
	ci::CameraPersp				mCamera;

	// Skybox
	ci::gl::VboMeshRef			mMesh;
	ci::gl::Texture				mTexture;

	// Params and utilities
	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGl		mParams;
	void						screenShot();
};

#include "cinder/ImageIo.h"
#include "cinder/Surface.h"
#include "cinder/Utilities.h"
#include "MeshHelper.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void SkyBoxSampleApp::draw()
{
	// Set up view
	gl::setViewport( getWindowBounds() );
	gl::clear( Colorf::gray( 0.5f ) );
	gl::setMatrices( mCamera );
	glMultMatrixf( mArcball.getQuat() );

	// Draw mesh
	gl::color( ColorAf::white() );
	gl::draw( mMesh );
	
	// Draw params GUI
	mParams.draw();
}

void SkyBoxSampleApp::mouseDown( MouseEvent event )
{
	// Rotate with arcball
	mArcball.mouseDown( event.getPos() );
}

void SkyBoxSampleApp::mouseDrag( MouseEvent event )
{
	// Rotate with arcball
	mArcball.mouseDrag( event.getPos() );
}

void SkyBoxSampleApp::prepareSettings( Settings* settings )
{
	settings->setFrameRate( 60.0f );
	settings->setFullScreen( false );
	settings->setResizable( false );
	settings->setWindowSize( 1280, 720 );
}

// Saves screenshot
void SkyBoxSampleApp::screenShot()
{
	fs::path path = getAppPath();
#if !defined( CINDR_MSW )
	path = path.parent_path();
#endif
	writeImage( path / ( "frame_" + toString( getElapsedFrames() ) + ".png" ), copyWindowSurface() );
}

void SkyBoxSampleApp::setup()
{
	gl::enable( GL_POLYGON_SMOOTH );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	gl::enable( GL_TEXTURE_2D );
	gl::enableDepthRead();
	gl::enableDepthWrite();
	gl::clear();
	
	// Define properties
	mFrameRate			= 0.0f;
	mFullScreen			= false;
	
	// Set up the arcball
	mArcball = Arcball( getWindowSize() );
	mArcball.setRadius( (float)getWindowHeight() * 0.5f );
	
	// Set up the camera
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f, 0.0001f, 10.0f );
	mCamera.lookAt( Vec3f( 0.0f, 0.0f, 0.5f ), Vec3f::zero() );
	
	// Set up the params GUI
	mParams = params::InterfaceGl( "Params", Vec2i( 200, 200 ) );
	mParams.addParam( "Frame rate",		&mFrameRate,								"", true	);
	mParams.addParam( "Full screen",	&mFullScreen,								"key=f"		);
	mParams.addButton( "Screen shot",	bind( &SkyBoxSampleApp::screenShot, this ),	"key=space"	);
	mParams.addButton( "Quit",			bind( &SkyBoxSampleApp::quit, this ),		"key=q"		);

	// Load the texture map
	gl::Texture::Format format;
	format.setMagFilter( GL_LINEAR );
	format.setMinFilter( GL_LINEAR );
	format.setWrap( GL_REPEAT, GL_REPEAT );
	mTexture = gl::Texture( loadImage( loadResource( RES_TEXTURE_PNG ) ), format );
	mTexture.bind();

	// Enumerate the side in the same order that MeshHelper
	// builds a cube
	enum : size_t {
		BACK, BOTTOM, FRONT, LEFT, RIGHT, TOP
	};
	
	// Use MeshHelepr to create a cube
	TriMesh cube = MeshHelper::createCube();
	
	// Calculate the scale
	Vec2f scale = Vec2f::one() / Vec2f( 3.0f, 4.0f );
	
	// Declare a vector to hold texture coordinates
	vector<Vec2f> texCoords;
	
	// Iterate through the six cube sides
	for ( size_t i = 0; i < 6; ++i ) {
		
		// UV offset and relative coordinates
		Vec2f offset = Vec2f::zero();
		Vec2f uv0( 0.0f, 1.0f );
		Vec2f uv1( 1.0f, 1.0f );
		Vec2f uv2( 0.0f, 0.0f );
		Vec2f uv3( 1.0f, 0.0f );
		
		// Change the offset for each side
		switch ( i ) {
			case BACK:
				offset = Vec2f( 1.0f, 3.0f );
				
				// Back must be flipped over
				uv0.y = 1.0f - uv0.y;
				uv1.y = 1.0f - uv1.y;
				uv2.y = 1.0f - uv2.y;
				uv3.y = 1.0f - uv3.y;
				break;
			case BOTTOM:
				offset = Vec2f( 1.0f, 2.0f );
				break;
			case FRONT:
				offset = Vec2f( 1.0f, 1.0f );
				break;
			case LEFT:
				offset = Vec2f( 2.0f, 1.0f );
				break;
			case RIGHT:
				offset = Vec2f( 0.0f, 1.0f );
				break;
			case TOP:
				offset = Vec2f( 1.0f, 0.0f );
				break;
		}
		
		// Scale and add textre
		texCoords.push_back( ( offset + uv1 ) * scale );
		texCoords.push_back( ( offset + uv0 ) * scale );
		texCoords.push_back( ( offset + uv2 ) * scale );
		texCoords.push_back( ( offset + uv2 ) * scale );
		texCoords.push_back( ( offset + uv3 ) * scale );
		texCoords.push_back( ( offset + uv1 ) * scale );
	}
	
	// Create the mesh using existing cube data and new texture coordinates
	mMesh = gl::VboMesh::create( MeshHelper::create( cube.getIndices(), cube.getVertices(), cube.getNormals(), texCoords ) );
}

void SkyBoxSampleApp::update()
{
	// Update FPS
	mFrameRate = getAverageFps();

	// Toggle fullscreen
	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
	}
}

CINDER_APP_BASIC( SkyBoxSampleApp, RendererGl( RendererGl::AA_MSAA_32 ) )
