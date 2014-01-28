/*
* 
* Copyright (c) 2014, Ban the Rewind
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
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/params/Params.h"

class DeferredApp : public ci::app::AppNative 
{
public:
	void						draw();
	void						mouseDown( ci::app::MouseEvent event );
	void						mouseDrag( ci::app::MouseEvent event );
	void						prepareSettings( ci::app::AppNative::Settings* settings );
	void						setup();
	void						shutdown();
	void						update();
private:
	ci::gl::VboMeshRef			mCone;
	ci::gl::VboMeshRef			mCube;
	ci::gl::VboMeshRef			mSphere;
	ci::gl::VboMeshRef			mSquare;

	std::vector<ci::Matrix44f>	mCubes;

	ci::Arcball					mArcball;
	ci::CameraPersp				mCamera;
	ci::Vec2f					mCursor;
	ci::Vec2f					mCursorTarget;
	
	ci::ColorAf					mLightAmbient;
	float						mLightAttenuationConstant;
	float						mLightAttenuationLinear;
	float						mLightAttenuationQuadratic;
	ci::ColorAf					mLightDiffuse;
	float						mLightIntensity;
	ci::Vec3f					mLightPosition;
	float						mLightScale;
	ci::ColorAf					mLightSpecular;

	ci::ColorAf					mMaterialAmbient;
	ci::ColorAf					mMaterialDiffuse;
	float						mMaterialShininess;
	ci::ColorAf					mMaterialSpecular;

	ci::gl::Fbo					mFbo;
	ci::gl::GlslProgRef			mShaderGBuffer;
	ci::gl::GlslProgRef			mShaderLight;
	ci::gl::TextureRef			mTexture;

	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGlRef	mParams;
	void						screenShot();
};

#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "MeshHelper.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const static GLenum kBuffers[] = { 
	GL_COLOR_ATTACHMENT0, 
	GL_COLOR_ATTACHMENT1, 
	GL_COLOR_ATTACHMENT2, 
	GL_COLOR_ATTACHMENT3 
};

void DeferredApp::draw()
{
	mFbo.bindFramebuffer();
	glDrawBuffers( 4, kBuffers );
	gl::setViewport( mFbo.getBounds() );
	gl::clear();
	gl::setMatrices( mCamera );
	gl::color( Colorf::white() );

	gl::enableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();

	mShaderGBuffer->bind();

	mShaderGBuffer->uniform( "glow",	false );
	mShaderGBuffer->uniform( "tex",		0 );
	
	gl::enable( GL_TEXTURE_2D );
	mTexture->bind();
	gl::pushModelView();
	gl::multModelView( mArcball.getQuat() );
	for ( const Matrix44f& m : mCubes ) {
		gl::pushModelView();
		gl::multModelView( m );
		gl::draw( mCube );
		gl::popModelView();
	}
	gl::popModelView();
	mTexture->unbind();

	mShaderGBuffer->uniform( "glow",	true );
	gl::color( ColorAf::white() );
	gl::pushModelView();
	gl::translate( mLightPosition );
	float d = ( (float)M_PI * 2.0f ) / 5.0f;
	float t = d;
	for ( size_t i = 0; i < 5; ++i, t += d ) {
		gl::pushModelView();
		gl::translate( Vec3f( cos( t ), sin( t ), -sin( t ) ) * ( mLightScale * 0.5f ) );
		gl::scale( Vec3f::one() * mLightScale * 0.15f );
		gl::draw( mSphere );
		gl::popModelView();
	}
	gl::popModelView();

	mShaderGBuffer->unbind();
	mFbo.unbindFramebuffer();

	gl::setViewport( getWindowBounds() );
	gl::clear( mLightAmbient );
	gl::setMatrices( mCamera );
	
	for ( int32_t i = 0; i < 4; ++i ) {
		mFbo.bindTexture( i, i );
	}

	mShaderLight->bind();
	mShaderLight->uniform( "eyePoint",					mCamera.getEyePoint() );
	mShaderLight->uniform( "lightAmbient",				mLightAmbient );
	mShaderLight->uniform( "lightAttenuationConstant",	mLightAttenuationConstant );
	mShaderLight->uniform( "lightAttenuationLinear",	mLightAttenuationLinear );
	mShaderLight->uniform( "lightAttenuationQuadratic",	mLightAttenuationQuadratic );
	mShaderLight->uniform( "lightDiffuse",				mLightDiffuse );
	//mShaderLight->uniform( "lightPosition",				mLightPosition );
	mShaderLight->uniform( "lightSpecular",				mLightSpecular );
	mShaderLight->uniform( "materialAmbient",			mMaterialAmbient );
	mShaderLight->uniform( "materialDiffuse",			mMaterialDiffuse );
	mShaderLight->uniform( "materialShininess",			mMaterialShininess );
	mShaderLight->uniform( "materialSpecular",			mMaterialSpecular );
	mShaderLight->uniform( "texAlbedo",					0 );
	mShaderLight->uniform( "texGlow",					1 );
	mShaderLight->uniform( "texNormal",					2 );
	mShaderLight->uniform( "texPosition",				3 );
	mShaderLight->uniform( "windowSize",				Vec2f( getWindowSize() ) );

	gl::pushModelView();
	gl::translate( mLightPosition );
	d = ( (float)M_PI * 2.0f ) / 5.0f;
	t = d;
	for ( size_t i = 0; i < 5; ++i, t += d ) {
		gl::pushModelView();
		Vec3f pos = Vec3f( cos( t ), sin( t ), -sin( t ) ) * mLightScale;
		mShaderLight->uniform( "lightPosition", mLightPosition + pos );
		gl::translate( pos );
		gl::scale( Vec3f::one() * mLightScale );
		gl::draw( mSphere );
		gl::popModelView();
	}
	gl::popModelView();

	mShaderLight->unbind();

	for ( GLuint i = 0; i < 4; ++i ) {
		mFbo.unbindTexture();
	}

	gl::setMatricesWindow( getWindowSize() );
	gl::color( Colorf::white() );
	gl::disableDepthRead();
	gl::disableDepthWrite();

	Vec2i sz( 160, 120 );
	Vec2i pos( getWindowWidth() - sz.x, 0 );
	for ( size_t i = 0; i < 4; ++i ) {
		gl::draw( mFbo.getTexture( i ), mFbo.getTexture( i ).getBounds(), Rectf( Area( pos, pos + sz ) ) );
		pos.y += sz.y;
	}

	mParams->draw();
}

void DeferredApp::mouseDown( MouseEvent event )
{
	mouseDrag( event );
}

void DeferredApp::mouseDrag( MouseEvent event )
{
	mCursorTarget = Vec2f( event.getPos() );
}

void DeferredApp::screenShot()
{
	writeImage( getAppPath() / ( "frame_" + toString( getElapsedFrames() ) + ".png" ), copyWindowSurface() );
}

void DeferredApp::prepareSettings( Settings* settings )
{
	settings->setFrameRate( 60.0f );
	settings->setWindowSize( 1024, 768 );
}

void DeferredApp::setup()
{
	try {
		mShaderGBuffer = gl::GlslProg::create( loadResource( RES_GLSL_GBUFFER_VERT ), loadResource( RES_GLSL_GBUFFER_FRAG ) );
	} catch ( gl::GlslProgCompileExc ex ) {
		console() << "GBuffer: " << ex.what() << endl;
		quit();
		return;
	}
	try {
		mShaderLight = gl::GlslProg::create( loadResource( RES_GLSL_LIGHT_VERT ), loadResource( RES_GLSL_LIGHT_POINT_FRAG ) );
	} catch ( gl::GlslProgCompileExc ex ) {
		console() << "Point light: " << ex.what() << endl;
		quit();
		return;
	}

	mCone						= gl::VboMesh::create( MeshHelper::createCylinder( Vec2i( 64, 32 ), 0.0f, 1.0f, false, true ) );
	mCube						= gl::VboMesh::create( MeshHelper::createCube( Vec3i::one() ) );
	mCursor						= getWindowCenter();
	mCursorTarget				= mCursor;
	mFrameRate					= 0.0f;
	mFullScreen					= false;
	mLightAmbient				= ColorAf::black();
	mLightAttenuationConstant	= 0.05f;
	mLightAttenuationLinear		= 0.005f;
	mLightAttenuationQuadratic	= 0.0005f;
	mLightDiffuse				= ColorAf::gray( 0.5f );
	mLightIntensity				= 0.5f;
	mLightPosition				= Vec3f( 0.0f, 0.0f, -2.0f );
	mLightScale					= 6.0f;
	mLightSpecular				= ColorAf::white();
	mMaterialAmbient			= ColorAf::black();
	mMaterialDiffuse			= ColorAf::gray( 0.5f );
	mMaterialShininess			= 100.0f;
	mMaterialSpecular			= ColorAf::white();
	mParams						= params::InterfaceGl::create( "Params", Vec2i( 200, 400 ) );
	mSphere						= gl::VboMesh::create( MeshHelper::createSphere( Vec2i( 64, 32 ) ) );
	mSquare						= gl::VboMesh::create( MeshHelper::createSquare() );
	mTexture					= gl::Texture::create( loadImage( loadResource( RES_PNG_TEXTURE ) ) );
	
	mArcball.setWindowSize( getWindowSize() );
	mArcball.setRadius( getWindowCenter().y );

	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 60.0f, 1.0f, 1000.0f );
	mCamera.lookAt( Vec3f( 0.0f, 0.0f, 10.0f ), Vec3f::zero() );
	
	gl::Fbo::Format format;
	format.enableColorBuffer( true, 4 );
	format.setColorInternalFormat( GL_RGBA32F );
	format.setMagFilter( GL_LINEAR );
	format.setMinFilter( GL_LINEAR );
	format.setWrap( GL_CLAMP, GL_CLAMP );
	mFbo = gl::Fbo( getWindowWidth(), getWindowHeight(), format );
	mFbo.bindFramebuffer();
	glDrawBuffers( 4, kBuffers );
	gl::setViewport( mFbo.getBounds() );
	gl::clear();
	mFbo.unbindFramebuffer();

	mParams->addParam( "Frame rate",			&mFrameRate,					"", true );
	mParams->addParam( "Full screen",			&mFullScreen,					"key=f" );
	mParams->addButton( "Screen shot",			[ & ]() { screenShot(); },		"key=space" );
	mParams->addButton( "Quit",					[ & ]() { quit(); },			"key=q" );
	mParams->addSeparator();
	mParams->addParam( "Light ambient",			&mLightAmbient );
	mParams->addParam( "Light att const",		&mLightAttenuationConstant,		"min=0.0 max=1.0 step=0.001" );
	mParams->addParam( "Light att linear",		&mLightAttenuationLinear,		"min=0.0 max=1.0 step=0.00001" );
	mParams->addParam( "Light att quad",		&mLightAttenuationQuadratic,	"min=0.0 max=1.0 step=0.000001" );
	mParams->addParam( "Light diffuse",			&mLightDiffuse );
	mParams->addParam( "Light position",		&mLightPosition );
	mParams->addParam( "Light scale",			&mLightScale,					"min=0.0 max=10000.0 step=0.01" );
	mParams->addParam( "Light specular",		&mLightSpecular);
	mParams->addSeparator();
	mParams->addParam( "Material ambient",		&mMaterialAmbient );
	mParams->addParam( "Material diffuse",		&mMaterialDiffuse );
	mParams->addParam( "Material shininess",	&mMaterialShininess,			"min=0.0 max=10000.0 step=0.01" );
	mParams->addParam( "Material specular",		&mMaterialSpecular);

	int32_t count = randInt( 50, 100 );
	for ( int32_t i = 0; i < count; ++i ) {
		Matrix44f m;
		m.setToIdentity();
		m.translate( randVec3f() * randFloat( 2.0f, 7.0f ) );
		m.translate( Vec3f( 0.0f, 0.0f, -2.0f ) );
		m.rotate( randVec3f() * 360.0f );
		m.scale( randVec3f() * 1.5f + Vec3f::one() * 0.5f );
		mCubes.push_back( m );
	}
}

void DeferredApp::shutdown()
{
}

void DeferredApp::update()
{
	mFrameRate = getAverageFps();

	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
	}

	mCursor = mCursor.lerp( 0.3f, mCursorTarget );
	mArcball.mouseDrag( Vec2i( mCursor ) );
}

CINDER_APP_BASIC( DeferredApp, RendererGl( RendererGl::AA_MSAA_32 ) )
