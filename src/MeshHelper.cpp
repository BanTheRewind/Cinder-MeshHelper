/*
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

#include "MeshHelper.h"

using namespace ci;
using namespace std;

TriMesh MeshHelper::createTriMesh( vector<uint32_t> &indices, const vector<Vec3f> &positions, 
	const vector<Vec3f> &normals, const vector<Vec2f> &texCoords )
{
	TriMesh mesh;
	if ( indices.size() > 0 ) {
		mesh.appendIndices( &indices[ 0 ], indices.size() );
	}
	if ( normals.size() > 0 ) {
		for ( vector<Vec3f>::const_iterator iter = normals.begin(); iter != normals.end(); ++iter ) {
			mesh.appendNormal( *iter );
		}
	}
	if ( positions.size() > 0 ) {
		mesh.appendVertices( &positions[ 0 ], positions.size() );
	}
	if ( texCoords.size() > 0 ) {
		for ( vector<Vec2f>::const_iterator iter = texCoords.begin(); iter != texCoords.end(); ++iter ) {
			mesh.appendTexCoord( *iter );
		}
	}
	return mesh;
}

TriMesh MeshHelper::createCircleTriMesh( const Vec2i &resolution )
{
	return createRingTriMesh( resolution, 0.0f );
}

TriMesh MeshHelper::createCubeTriMesh( const Vec3i &resolution )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	ci::TriMesh front	= createSquareTriMesh( Vec2i( resolution.x, resolution.y ) );
	ci::TriMesh left	= createSquareTriMesh( Vec2i( resolution.z, resolution.y ) );
	ci::TriMesh top		= createSquareTriMesh( Vec2i( resolution.x, resolution.z ) );
	
	Vec3f normal;
	Vec3f offset;
	Matrix44f transform;

	// Back
	normal = Vec3f( 0.0f, 0.0f, -1.0f );
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate( offset );
	for ( vector<Vec3f>::iterator iter = front.getVertices().begin(); iter != front.getVertices().end(); ++iter ) {
		positions.push_back( transform.transformPoint( *iter ) );
		normals.push_back( normal );
	}
	for ( vector<Vec2f>::iterator iter = front.getTexCoords().begin(); iter != front.getTexCoords().end(); ++iter ) {
		texCoords.push_back( *iter );
	}

	// Bottom
	normal = Vec3f( 0.0f, -1.0f, 0.0f );
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate( offset );
	transform.rotate( Vec3f( -(float)M_PI * 0.5f, 0.0f, 0.0f ) );
	transform.translate( offset * -1.0f );
	transform.translate( offset );
	for ( vector<Vec3f>::iterator iter = top.getVertices().begin(); iter != top.getVertices().end(); ++iter ) {
		positions.push_back( transform.transformPoint( *iter ) );
		normals.push_back( normal );
	}
	for ( vector<Vec2f>::iterator iter = top.getTexCoords().begin(); iter != top.getTexCoords().end(); ++iter ) {
		texCoords.push_back( *iter );
	}

	normal = Vec3f( 0.0f, 0.0f, 1.0f );
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate( offset );
	for ( vector<Vec3f>::iterator iter = front.getVertices().begin(); iter != front.getVertices().end(); ++iter ) {
		positions.push_back( transform.transformPoint( *iter ) );
		normals.push_back( normal );
	}
	for ( vector<Vec2f>::iterator iter = front.getTexCoords().begin(); iter != front.getTexCoords().end(); ++iter ) {
		texCoords.push_back( *iter );
	}

	normal = Vec3f( -1.0f, 0.0f, 0.0f );
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate( offset );
	transform.rotate( Vec3f( 0.0f, -(float)M_PI * 0.5f, 0.0f ) );
	transform.translate( offset * -1.0f );
	transform.translate( offset );
	for ( vector<Vec3f>::iterator iter = left.getVertices().begin(); iter != left.getVertices().end(); ++iter ) {
		positions.push_back( transform.transformPoint( *iter ) );
		normals.push_back( normal );
	}
	for ( vector<Vec2f>::iterator iter = left.getTexCoords().begin(); iter != left.getTexCoords().end(); ++iter ) {
		texCoords.push_back( *iter );
	}

	// Right
	normal = Vec3f( 1.0f, 0.0f, 0.0f );
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate( offset );
	transform.rotate( Vec3f( 0.0f, (float)M_PI * 0.5f, 0.0f ) );
	transform.translate( offset * -1.0f );
	transform.translate( offset );
	for ( vector<Vec3f>::iterator iter = left.getVertices().begin(); iter != left.getVertices().end(); ++iter ) {
		positions.push_back( transform.transformPoint( *iter ) );
		normals.push_back( normal );
	}
	for ( vector<Vec2f>::iterator iter = left.getTexCoords().begin(); iter != left.getTexCoords().end(); ++iter ) {
		texCoords.push_back( *iter );
	}

	normal = Vec3f( 0.0f, 1.0f, 0.0f );
	offset = normal * 0.5f;
	transform.setToIdentity();
	transform.translate( offset );
	transform.rotate( Vec3f( (float)M_PI * 0.5f, 0.0f, 0.0f ) );
	transform.translate( offset * -1.0f );
	transform.translate( offset );
	for ( vector<Vec3f>::iterator iter = top.getVertices().begin(); iter != top.getVertices().end(); ++iter ) {
		positions.push_back( transform.transformPoint( *iter ) );
		normals.push_back( normal );
	}
	for ( vector<Vec2f>::iterator iter = top.getTexCoords().begin(); iter != top.getTexCoords().end(); ++iter ) {
		texCoords.push_back( *iter );
	}

	for ( size_t i = 0; i < positions.size(); ++i ) {
		indices.push_back( i );
	}

	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createCylinderTriMesh( const Vec2i &resolution, float topRadius, float baseRadius, bool closeTop, bool closeBase )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec3f> srcNormals;
	vector<Vec3f> srcPositions;
	vector<Vec2f> srcTexCoords;
	vector<Vec2f> texCoords;

	float delta = ( 2.0f * (float)M_PI ) / (float)resolution.x;
	float step	= 1.0f / (float)resolution.y;
	float ut	= 1.0f / (float)resolution.x;

	uint32_t p = 0;
	for ( float phi = 0.0f; p <= (uint32_t)resolution.y; ++p, phi += step ) {
		uint32_t t	= 0;
		float u		= 0.0f;
		for ( float theta = 0.0f; t < (uint32_t)resolution.x; ++t, u += ut, theta += delta ) {

			float radius = lerp( baseRadius, topRadius, phi );

			Vec3f position( 
				math<float>::cos( theta ) * radius, 
				phi - 0.5f, 
				math<float>::sin( theta ) * radius
				);
			srcPositions.push_back( position );

			Vec3f normal = Vec3f( position.x, 0.0f, position.z ).normalized();
			normal.y = 0.0f;
			srcNormals.push_back( normal );

			Vec2f texCoord( u, phi );
			srcTexCoords.push_back( texCoord );
		}
	}

	srcNormals.push_back( Vec3f( 0.0f, 1.0f, 0.0f ) );
	srcNormals.push_back( Vec3f( 0.0f, -1.0f, 0.0f ) );
	srcPositions.push_back( Vec3f( 0.0f, -0.5f, 0.0f ) );
	srcPositions.push_back( Vec3f( 0.0f, 0.5f, 0.0f ) );
	srcTexCoords.push_back( Vec2f( 0.0f, 0.0f ) );
	srcTexCoords.push_back( Vec2f( 0.0f, 1.0f ) );
	int32_t topCenter		= (int32_t)srcPositions.size() - 1;
	int32_t bottomCenter	= topCenter - 1;

	if ( closeTop ) {
		for ( uint32_t t = 0; t < (uint32_t)resolution.x; ++t ) {
			uint32_t n = t + 1 >= (uint32_t)resolution.x ? 0 : t + 1;

			normals.push_back( srcNormals[ topCenter ] );
			normals.push_back( srcNormals[ topCenter ] );
			normals.push_back( srcNormals[ topCenter ] );

			positions.push_back( srcPositions[ topCenter ] );
			positions.push_back( srcPositions[ (uint32_t)( resolution.x * resolution.y ) + n ] );
			positions.push_back( srcPositions[ (uint32_t)( resolution.x * resolution.y ) + t ] );

			texCoords.push_back( srcTexCoords[ topCenter ] );
			texCoords.push_back( srcTexCoords[ topCenter ] );
			texCoords.push_back( srcTexCoords[ topCenter ] );
		}
	}

	for ( uint32_t p = 0; p < (uint32_t)resolution.y; ++p ) {
		for ( uint32_t t = 0; t < (uint32_t)resolution.x; ++t ) {
			uint32_t n = t + 1 >= (uint32_t)resolution.x ? 0 : t + 1;
		
			uint32_t index0 = ( p + 0 ) * (uint32_t)resolution.x + t;
			uint32_t index1 = ( p + 0 ) * (uint32_t)resolution.x + n;
			uint32_t index2 = ( p + 1 ) * (uint32_t)resolution.x + t;
			uint32_t index3 = ( p + 1 ) * (uint32_t)resolution.x + n;

			normals.push_back( srcNormals[ index0 ] );
			normals.push_back( srcNormals[ index2 ] );
			normals.push_back( srcNormals[ index1 ] );
			normals.push_back( srcNormals[ index1 ] );
			normals.push_back( srcNormals[ index2 ] );
			normals.push_back( srcNormals[ index3 ] );

			positions.push_back( srcPositions[ index0 ] );
			positions.push_back( srcPositions[ index2 ] );
			positions.push_back( srcPositions[ index1 ] );
			positions.push_back( srcPositions[ index1 ] );
			positions.push_back( srcPositions[ index2 ] );
			positions.push_back( srcPositions[ index3 ] );

			texCoords.push_back( srcTexCoords[ index0 ] );
			texCoords.push_back( srcTexCoords[ index2 ] );
			texCoords.push_back( srcTexCoords[ index1 ] );
			texCoords.push_back( srcTexCoords[ index1 ] );
			texCoords.push_back( srcTexCoords[ index2 ] );
			texCoords.push_back( srcTexCoords[ index3 ] );
		}
	}

	if ( closeBase ) {
		for ( uint32_t t = 0; t < (uint32_t)resolution.x; ++t ) {
			uint32_t n = t + 1 >= (uint32_t)resolution.x ? 0 : t + 1;

			normals.push_back( srcNormals[ bottomCenter ] );
			normals.push_back( srcNormals[ bottomCenter ] );
			normals.push_back( srcNormals[ bottomCenter ] );

			positions.push_back( srcPositions[ bottomCenter ] );
			positions.push_back( srcPositions[ n ] );
			positions.push_back( srcPositions[ t ] );

			texCoords.push_back( srcTexCoords[ bottomCenter ] );
			texCoords.push_back( srcTexCoords[ bottomCenter ] );
			texCoords.push_back( srcTexCoords[ bottomCenter ] );
		}
	}

	for ( uint32_t i = 0; i < positions.size(); ++i ) {
		indices.push_back( i );
	}
			
	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );
		
	indices.clear();
	normals.clear();
	positions.clear();
	srcNormals.clear();
	srcPositions.clear();
	srcTexCoords.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createRingTriMesh( const Vec2i &resolution, float secondRadius )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	Vec3f norm0( 0.0f, 0.0f, 1.0f );

	float delta = ( (float)M_PI * 2.0f ) / (float)resolution.x;
	float width	= 1.0f - secondRadius;
	float step	= width / (float)resolution.y;

	uint32_t p = 0;
	for ( float phi = 0.0f; p < (uint32_t)resolution.y; ++p, phi += step ) {

		float innerRadius = phi + 0.0f + secondRadius;
		float outerRadius = phi + step + secondRadius;

		uint32_t t = 0;
		for ( float theta = 0.0f; t < (uint32_t)resolution.x; ++t, theta += delta ) {

			float ct	= math<float>::cos( theta );
			float st	= math<float>::sin( theta );
			float ctn	= math<float>::cos( theta + delta );
			float stn	= math<float>::sin( theta + delta );

			Vec3f vert0 = Vec3f( ct, st, 0.0f ) * innerRadius;
			Vec3f vert1 = Vec3f( ctn, stn, 0.0f ) * innerRadius;
			Vec3f vert2 = Vec3f( ct, st, 0.0f ) * outerRadius;
			Vec3f vert3 = Vec3f( ctn, stn, 0.0f ) * outerRadius;
			if ( t >= (uint32_t)resolution.x - 1 ) {
				ctn		= math<float>::cos( 0.0f );
				stn		= math<float>::sin( 0.0f );
				vert1	= Vec3f( ctn, stn, 0.0f ) * innerRadius;
				vert3	= Vec3f( ctn, stn, 0.0f ) * outerRadius;
			}

			Vec2f texCoord0 = ( vert0.xy() + Vec2f::one() ) * 0.5f;
			Vec2f texCoord1 = ( vert1.xy() + Vec2f::one() ) * 0.5f;
			Vec2f texCoord2 = ( vert2.xy() + Vec2f::one() ) * 0.5f;
			Vec2f texCoord3 = ( vert3.xy() + Vec2f::one() ) * 0.5f;

			positions.push_back( vert0 );
			positions.push_back( vert2 );
			positions.push_back( vert1 );
			positions.push_back( vert1 );
			positions.push_back( vert2 );
			positions.push_back( vert3 );

			texCoords.push_back( texCoord0 );
			texCoords.push_back( texCoord2 );
			texCoords.push_back( texCoord1 );
			texCoords.push_back( texCoord1 );
			texCoords.push_back( texCoord2 );
			texCoords.push_back( texCoord3 );
		}
	}

	for ( uint32_t i = 0; i < positions.size(); ++i ) {
		indices.push_back( i );
		normals.push_back( norm0 );
	}

	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createSphereTriMesh( const Vec2i &resolution )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	float step = (float)M_PI / (float)resolution.y;
	float delta = ((float)M_PI * 2.0f) / (float)resolution.x;

	uint32_t p = 0;
	for ( float phi = 0.0f; p <= (uint32_t)resolution.y; p++, phi += step ) {
		uint32_t t = 0;
		for ( float theta = delta; t < (uint32_t)resolution.x; t++, theta += delta ) {
			float sinP = math<float>::sin( phi );
			Vec3f position(
				sinP * math<float>::cos( theta ),
				sinP * math<float>::sin( theta ),
				-math<float>::cos( phi ) );
			positions.push_back( position );

			Vec3f normal = position.normalized();
			normals.push_back( normal );

			texCoords.push_back( ( normal.xy() + Vec2f::one() ) * 0.5f ); 

			uint32_t n = t + 1 >= (uint32_t)resolution.x ? 0 : t + 1;
			indices.push_back( p * resolution.x + t );
			indices.push_back( ( p + 1 ) * resolution.x + t );
			indices.push_back( p * resolution.x + n );
			indices.push_back( p * resolution.x + n );
			indices.push_back( ( p + 1 ) * resolution.x + t );
			indices.push_back( ( p + 1 ) * resolution.x + n );
		}
	}

	for ( vector<uint32_t>::iterator iter = indices.begin(); iter != indices.end(); ) {
		if ( *iter < positions.size() ) {
			++iter;
		} else {
			iter = indices.erase( iter );
		}
	}

	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createSquareTriMesh( const Vec2i &resolution )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	Vec3f norm0( 0.0f, 0.0f, 1.0f ); 

	Vec2f scale( 1.0f / math<float>::max( (float)resolution.x, 1.0f ), 1.0f / math<float>::max( (float)resolution.y, 1.0f ) );
	size_t index = 0;
	for ( int32_t y = 0; y < resolution.y; ++y ) {
		for ( int32_t x = 0; x < resolution.x; ++x, ++index ) {

			float x1 = (float)x * scale.x;
			float y1 = (float)y * scale.y;
			float x2 = (float)( x + 1 ) * scale.x;
			float y2 = (float)( y + 1 ) * scale.y;

			Vec3f vert0( x1 - 0.5f, y1 - 0.5f, 0.0f );
			Vec3f vert1( x2 - 0.5f, y1 - 0.5f, 0.0f );
			Vec3f vert2( x1 - 0.5f, y2 - 0.5f, 0.0f );
			Vec3f vert3( x2 - 0.5f, y2 - 0.5f, 0.0f );
				
			Vec2f texCoord0( x1, y1 );
			Vec2f texCoord1( x2, y1 );
			Vec2f texCoord2( x1, y2 );
			Vec2f texCoord3( x2, y2 );

			positions.push_back( vert2 );
			positions.push_back( vert1 );
			positions.push_back( vert0 );
			positions.push_back( vert1 );
			positions.push_back( vert2 );
			positions.push_back( vert3 );

			texCoords.push_back( texCoord2 );
			texCoords.push_back( texCoord1 );
			texCoords.push_back( texCoord0 );
			texCoords.push_back( texCoord1 );
			texCoords.push_back( texCoord2 );
			texCoords.push_back( texCoord3 );

			for ( size_t i = 0; i < 6; ++i ) {
				indices.push_back( index * 6 + i );
				normals.push_back( norm0 );
			}
		}
	}

	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;

}

#if ! defined( CINDER_COCOA_TOUCH )

gl::VboMesh MeshHelper::createVboMesh( const vector<uint32_t> &indices, const vector<Vec3f> &positions, 
	const vector<Vec3f> &normals, const vector<Vec2f> &texCoords, GLenum primitiveType )
{
	gl::VboMesh::Layout layout;
	if ( indices.size() > 0 ) {
		layout.setStaticIndices();
	}
	if ( normals.size() > 0 ) {
		layout.setStaticNormals();
	}
	if ( positions.size() > 0 ) {
		layout.setStaticPositions();
	}
	if ( texCoords.size() > 0 ) {
		layout.setStaticTexCoords2d();
	}

	gl::VboMesh mesh( positions.size(), indices.size(), layout, primitiveType );
	if ( indices.size() > 0 ) {
		mesh.bufferIndices( indices );
	}
	if ( normals.size() > 0 ) {
		mesh.bufferNormals( normals );
	}
	if ( positions.size() > 0 ) {
		mesh.bufferPositions( positions );
	}
	if ( texCoords.size() > 0 ) {
		mesh.bufferTexCoords2d( 0, texCoords );
	}

	return mesh;
}

gl::VboMesh MeshHelper::createCircleVboMesh( const Vec2i &resolution )
{
	TriMesh mesh = createCircleTriMesh( resolution );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createCubeVboMesh( const Vec3i &resolution )
{
	TriMesh mesh = createCubeTriMesh( resolution );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createCylinderVboMesh( const Vec2i &resolution, float topRadius, float baseRadius, bool closeTop, bool closeBase )
{
	TriMesh mesh = createCylinderTriMesh( resolution, topRadius, baseRadius, closeTop, closeBase );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createRingVboMesh( const Vec2i &resolution, float secondRadius )
{
	TriMesh mesh = createRingTriMesh( resolution, secondRadius );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createSphereVboMesh( const Vec2i &resolution)
{
	TriMesh mesh = createSphereTriMesh( resolution );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createSquareVboMesh( const Vec2i &resolution )
{
	TriMesh mesh = createSquareTriMesh( resolution );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

#endif
