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
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/params/Params.h"

class VtfSampleApp : public ci::app::AppNative 
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

	// Sphere VBO
	void						createMesh();
	ci::gl::VboMesh				mSphere;
	
	// Displacement
	float						mDisplacementHeight;
	ci::gl::GlslProg			mShaderVtf;

	// For selecting mesh type from params
	int32_t						mMeshIndex;
	std::vector<std::string>	mMeshTitles;

	// Mesh resolution
	ci::Vec2i					mResolution;
	ci::Vec2i					mResolutionPrev;

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

	// Dynamic texture
	ci::gl::Fbo					mFbo;
	bool						mDrawFbo;
	ci::gl::GlslProg			mShaderTex;
	float						mSpeed;
	float						mTheta;

	// Params and utilities
	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGl		mParams;
	bool						mWireframe;
	void						screenShot();
};

#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "MeshHelper.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void VtfSampleApp::createMesh() 
{ 
	// Initialize FBO
	gl::Fbo::Format format;
	format.setColorInternalFormat( GL_RGB32F_ARB );
	mFbo = gl::Fbo( mResolution.x, mResolution.y, format );

	// Initialize FBO texture
	mFbo.bindFramebuffer();
	gl::setViewport( mFbo.getBounds() );
	gl::clear();
	mFbo.unbindFramebuffer();
	mFbo.getTexture().setWrap( GL_REPEAT, GL_REPEAT );

	// Generate sphere
	mSphere = gl::VboMesh( MeshHelper::createSphere( mResolution ) );
}

void VtfSampleApp::draw()
{
	// Bind FBO and set up window
	mFbo.bindFramebuffer();
	gl::setViewport( mFbo.getBounds() );
	gl::setMatricesWindow( mFbo.getSize() );
	gl::clear();

	// Bind and configure dynamic texture shader
	mShaderTex.bind();
	mShaderTex.uniform( "theta", mTheta );

	// Draw shader output
	gl::enable( GL_TEXTURE_2D );
	gl::color( Colorf::white() );
	gl::begin( GL_TRIANGLES );

	// Define quad vertices
	Vec2f vert0( (float)mFbo.getBounds().x1, (float)mFbo.getBounds().y1 );
	Vec2f vert1( (float)mFbo.getBounds().x2, (float)mFbo.getBounds().y1 );
	Vec2f vert2( (float)mFbo.getBounds().x1, (float)mFbo.getBounds().y2 );
	Vec2f vert3( (float)mFbo.getBounds().x2, (float)mFbo.getBounds().y2 );

	// Define quad texture coordinates
	Vec2f uv0( 0.0f, 0.0f );
	Vec2f uv1( 1.0f, 0.0f );
	Vec2f uv2( 0.0f, 1.0f );
	Vec2f uv3( 1.0f, 1.0f );

	// Draw quad (two triangles)
	gl::texCoord( uv0 );
	gl::vertex( vert0 );
	gl::texCoord( uv2 );
	gl::vertex( vert2 );
	gl::texCoord( uv1 );
	gl::vertex( vert1 );

	gl::texCoord( uv1 );
	gl::vertex( vert1 );
	gl::texCoord( uv2 );
	gl::vertex( vert2 );
	gl::texCoord( uv3 );
	gl::vertex( vert3 );

	gl::end();
	gl::disable( GL_TEXTURE_2D );

	// Unbind everything
	mShaderTex.unbind();
	mFbo.unbindFramebuffer();

	///////////////////////////////////////////////////////////////

	// Set up window
	gl::setViewport( getWindowBounds() );
	gl::setMatrices( mCamera );
	gl::clear( ColorAf::gray( 0.6f ) );

	// Use arcball to rotate model view
	glMultMatrixf( mArcball.getQuat() );

	// Enabled lighting, texture mapping, wireframe
	gl::enable( GL_TEXTURE_2D );
	if ( mLightEnabled ) {
		gl::enable( GL_LIGHTING );
	}
	if ( mTextureEnabled && mTexture ) {
		mTexture.bind();
	}
	if ( mWireframe ) {
		gl::enableWireframe();
	}

	// Bind and configure displacement shader
	mShaderVtf.bind();
	mShaderVtf.uniform( "displacement", 0 );
	mShaderVtf.uniform( "eyePoint", mCamera.getEyePoint() );
	mShaderVtf.uniform( "height", mDisplacementHeight );
	mShaderVtf.uniform( "lightingEnabled", mLightEnabled );
	mShaderVtf.uniform( "scale", mScale );
	mShaderVtf.uniform( "tex", 1 );
	mShaderVtf.uniform( "textureEnabled", mTextureEnabled );
	
	// Bind textures
	mFbo.bindTexture( 0, 0 );
	mTexture.bind( 1 );

	// Draw sphere
	gl::draw( mSphere );
	
	// Unbind everything
	mShaderVtf.unbind();
	mTexture.unbind();
	mFbo.unbindTexture();
	
	// Disable wireframe, texture mapping, lighting
	if ( mWireframe ) {
		gl::disableWireframe();
	}
	if ( mTextureEnabled && mTexture ) {
		mTexture.unbind();
	}
	if ( mLightEnabled ) {
		gl::disable( GL_LIGHTING );
	}
	gl::disable( GL_TEXTURE_2D );

	///////////////////////////////////////////////////////////////

	// Draw FBO
	if ( mDrawFbo ) {
		gl::enable( GL_TEXTURE_2D );
		gl::setMatricesWindow( getWindowSize() );
		Rectf preview( 220.0f, 20.0f, 300.0f, 100.0f );
		gl::draw( mFbo.getTexture(), mFbo.getBounds(), preview );
		gl::disable( GL_TEXTURE_2D );
	}

	// Draw params GUI
	mParams.draw();
}

void VtfSampleApp::mouseDown( MouseEvent event )
{
	// Rotate with arcball
	mArcball.mouseDown( event.getPos() );
}

void VtfSampleApp::mouseDrag( MouseEvent event )
{
	// Rotate with arcball
	mArcball.mouseDrag( event.getPos() );
}

void VtfSampleApp::mouseWheel( MouseEvent event )
{
	// Zoom in/out with mouse wheel
	Vec3f eye = mCamera.getEyePoint();
	eye.z += event.getWheelIncrement() * 0.1f;
	mCamera.setEyePoint( eye );
}

// Saves screenshot
void VtfSampleApp::screenShot()
{
	writeImage( getAppPath() / ( "frame_" + toString( getElapsedFrames() ) + ".png" ), copyWindowSurface() );
}

void VtfSampleApp::setup()
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
	mDisplacementHeight	= 8.0f;
	mDrawFbo			= true;
	mFrameRate			= 0.0f;
	mFullScreen			= false;
	mLightEnabled		= true;
	mMeshIndex			= 0;
	mResolution			= Vec2i( 64, 64 );
	mResolutionPrev		= mResolution;
	mScale				= Vec3f::one();
	mSpeed				= 1.0f;
	mTheta				= 0.0f;
	mTextureEnabled		= true;
	mWireframe			= false;
	
	// Set up the arcball
	mArcball = Arcball( getWindowSize() );
	mArcball.setRadius( (float)getWindowHeight() * 0.5f );
	
	// Set up the camera
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 60.0f, 0.0001f, 100.0f );
	mCamera.lookAt( Vec3f( 0.0f, 0.0f, -16.0f ), Vec3f::zero() );

	// Set up the light
	mLight = new gl::Light( gl::Light::DIRECTIONAL, 0 );
	mLight->setAmbient( ColorAf::black() );
	mLight->setDiffuse( ColorAf::black() );
	mLight->setDirection( Vec3f::one() );
	mLight->setPosition( Vec3f::one() * -1.0f );
	mLight->setSpecular( ColorAf::white() );
	mLight->enable();
	
	// Set up the params GUI
	mParams = params::InterfaceGl( "Params", Vec2i( 200, 320 ) );
	mParams.addParam( "Frame rate",		&mFrameRate,									"", true												);
	mParams.addSeparator();
	mParams.addParam( "Displ. height",	&mDisplacementHeight,							"keyDecr=h keyIncr=H min=-1000.0 max=1000.0 step=0.001"	);
	mParams.addParam( "Displ. speed",	&mSpeed,										"keyDecr=s keyIncr=S min=0.0 max=1000.0 step=0.01"		);
	mParams.addParam( "Draw FBO",		&mDrawFbo,										"key=d"													);
	mParams.addParam( "Enable light",	&mLightEnabled,									"key=l"													);
	mParams.addParam( "Enable texture",	&mTextureEnabled,								"key=t"													);
	mParams.addParam( "Resolution X",	&mResolution.x,									"keyDecr=x keyIncr=X min=1 max=1024 step=1"				);
	mParams.addParam( "Resolution Y",	&mResolution.y,									"keyDecr=y keyIncr=Y min=1 max=1024 step=1"				);
	mParams.addParam( "Scale",			&mScale																									);
	mParams.addParam( "Wireframe",		&mWireframe,									"key=w"													);
	mParams.addSeparator();
	mParams.addParam( "Full screen",	&mFullScreen,									"key=f"													);
	mParams.addButton( "Screen shot",	bind( &VtfSampleApp::screenShot, this ),		"key=space"												);
	mParams.addButton( "Quit",			bind( &VtfSampleApp::quit, this ),				"key=q"													);

	// Load shaders
	try {
		mShaderTex = gl::GlslProg( loadResource( RES_SHADER_TEX_VERT ), loadResource( RES_SHADER_TEX_FRAG ) );
	} catch ( gl::GlslProgCompileExc ex ) {
		console() << "Unable to compile texture shader:\n" << ex.what() << "\n";
	}
	try {
		mShaderVtf = gl::GlslProg( loadResource( RES_SHADER_VTF_VERT ), loadResource( RES_SHADER_VTF_FRAG ) );
	} catch ( gl::GlslProgCompileExc ex ) {
		console() << "Unable to compile VTF shader:\n" << ex.what() << "\n";
	}
	
	// Initialize FBO and VBO
	createMesh();
}

void VtfSampleApp::shutdown()
{
	// Clean up
	if ( mLight != 0 ) {
		mLight = 0;
	}
}

void VtfSampleApp::update()
{
	// Update FPS
	mFrameRate = getAverageFps();

	// Toggle fullscreen
	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
	}

	// Reset the mesh if the segment count changes
	if ( mResolutionPrev != mResolution ) {
		createMesh();
		mResolutionPrev = mResolution;
	}

	// Update animation position
	float time	= (float)getElapsedSeconds() * mSpeed;
	mTheta		= math<float>::sin( time );

	// Update light on every frame
	mLight->update( mCamera );
}

CINDER_APP_BASIC( VtfSampleApp, RendererGl )
