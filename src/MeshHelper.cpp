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

TriMesh MeshHelper::create( vector<uint32_t> &indices, const vector<Vec3f> &positions, 
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

TriMesh MeshHelper::createCircle( const Vec2i &resolution )
{
	return createRing( resolution, 0.0f );
}

TriMesh MeshHelper::createCube( const Vec3i &resolution )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	ci::TriMesh front	= createSquare( Vec2i( resolution.x, resolution.y ) );
	ci::TriMesh left	= createSquare( Vec2i( resolution.z, resolution.y ) );
	ci::TriMesh top		= createSquare( Vec2i( resolution.x, resolution.z ) );
	
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

	for ( uint32_t i = 0; i < positions.size(); ++i ) {
		indices.push_back( i );
	}

	TriMesh mesh = MeshHelper::create( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createCylinder( const Vec2i &resolution, float topRadius, float baseRadius, bool closeTop, bool closeBase )
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
	float ud	= 1.0f / (float)resolution.x;

	int32_t p = 0;
	for ( float phi = 0.0f; p <= resolution.y; ++p, phi += step ) {
		int32_t t	= 0;
		float u		= 0.0f;
		for ( float theta = 0.0f; t < resolution.x; ++t, u += ud, theta += delta ) {

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
		for ( int32_t t = 0; t < resolution.x; ++t ) {
			int32_t n = t + 1 >= resolution.x ? 0 : t + 1;

			normals.push_back( srcNormals[ topCenter ] );
			normals.push_back( srcNormals[ topCenter ] );
			normals.push_back( srcNormals[ topCenter ] );

			positions.push_back( srcPositions[ topCenter ] );
			positions.push_back( srcPositions[ ( resolution.x * resolution.y ) + n ] );
			positions.push_back( srcPositions[ ( resolution.x * resolution.y ) + t ] );

			texCoords.push_back( srcTexCoords[ topCenter ] );
			texCoords.push_back( srcTexCoords[ topCenter ] );
			texCoords.push_back( srcTexCoords[ topCenter ] );
		}
	}

	for ( int32_t p = 0; p < resolution.y; ++p ) {
		for ( int32_t t = 0; t < resolution.x; ++t ) {
			int32_t n = t + 1 >= resolution.x ? 0 : t + 1;
		
			int32_t index0 = ( p + 0 ) * resolution.x + t;
			int32_t index1 = ( p + 0 ) * resolution.x + n;
			int32_t index2 = ( p + 1 ) * resolution.x + t;
			int32_t index3 = ( p + 1 ) * resolution.x + n;

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
		for ( int32_t t = 0; t < resolution.x; ++t ) {
			int32_t n = t + 1 >= resolution.x ? 0 : t + 1;

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
			
	TriMesh mesh = MeshHelper::create( indices, positions, normals, texCoords );
		
	indices.clear();
	normals.clear();
	positions.clear();
	srcNormals.clear();
	srcPositions.clear();
	srcTexCoords.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createIcosahedron( uint32_t division )
{
	vector<Vec3f> positions;
	vector<Vec3f> normals;
	vector<Vec2f> texCoords;
	vector<uint32_t> indices;

	const float t	= 0.5f + 0.5f * math<float>::sqrt( 5.0f );
	const float one	= 1.0f / math<float>::sqrt( 1.0f + t * t );
	const float tau	= t * one;
	const float pi	= (float)M_PI;

	normals.push_back( Vec3f(  one, 0.0f,  tau) );	
	normals.push_back( Vec3f(  one, 0.0f, -tau) );	
	normals.push_back( Vec3f( -one, 0.0f, -tau) );		
	normals.push_back( Vec3f( -one, 0.0f,  tau) );		

	normals.push_back( Vec3f(  tau,  one, 0.0f ) );
	normals.push_back( Vec3f( -tau,  one, 0.0f ) );	
	normals.push_back( Vec3f( -tau, -one, 0.0f ) );
	normals.push_back( Vec3f(  tau, -one, 0.0f ) );

	normals.push_back( Vec3f( 0.0f,  tau,  one) );
	normals.push_back( Vec3f( 0.0f, -tau,  one) );	
	normals.push_back( Vec3f( 0.0f, -tau, -one) );		
	normals.push_back( Vec3f( 0.0f,  tau, -one) );		

	for ( size_t i = 0; i < 12; ++i ) { 
		positions.push_back( normals[ i ] * 0.5f );
	}

	static const size_t numIndices = 60;
	uint32_t indexArray[ numIndices ] = { 
		0, 8, 3,	0, 3, 9, 
		1, 2, 11,	1, 10, 2, 
		4, 0, 7,	4, 7, 1, 
		6, 3, 5,	6, 5, 2, 
		8, 4, 11,	8, 11, 5, 
		9, 10, 7,	9, 6, 10, 
		8, 0, 4,	11, 4, 1, 
		0, 9, 7,	1, 7, 10, 
		3, 8, 5,	2, 5, 11, 
		3, 6, 9,	2, 10, 6 
	};
	for ( size_t i = 0; i < numIndices; ++i ) {
		indices.push_back( indexArray[ i ] );
	}

	for ( vector<Vec3f>::const_iterator iter = normals.begin(); iter != normals.end(); ++iter ) {
		float u = 0.5f + 0.5f * math<float>::atan2( iter->x, iter->z ) / pi;
		float v = 0.5f - math<float>::asin( iter->y ) / pi;
		Vec2f texCoord( u, v );
		texCoords.push_back( texCoord );
	}

	TriMesh mesh = MeshHelper::create( indices, positions, normals, texCoords );

	if ( division > 1 ) {
		mesh = subdivide( mesh, division, true );
	}

	// TODO create star

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createRing( const Vec2i &resolution, float ratio )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	Vec3f norm0( 0.0f, 0.0f, 1.0f );

	float delta = ( (float)M_PI * 2.0f ) / (float)resolution.x;
	float width	= 1.0f - ratio;
	float step	= width / (float)resolution.y;

	int32_t p = 0;
	for ( float phi = 0.0f; p < resolution.y; ++p, phi += step ) {

		float innerRadius = phi + 0.0f + ratio;
		float outerRadius = phi + step + ratio;

		int32_t t = 0;
		for ( float theta = 0.0f; t < resolution.x; ++t, theta += delta ) {
			float ct	= math<float>::cos( theta );
			float st	= math<float>::sin( theta );
			float ctn	= math<float>::cos( theta + delta );
			float stn	= math<float>::sin( theta + delta );

			Vec3f pos0 = Vec3f( ct, st, 0.0f ) * innerRadius;
			Vec3f pos1 = Vec3f( ctn, stn, 0.0f ) * innerRadius;
			Vec3f pos2 = Vec3f( ct, st, 0.0f ) * outerRadius;
			Vec3f pos3 = Vec3f( ctn, stn, 0.0f ) * outerRadius;
			if ( t >= resolution.x - 1 ) {
				ctn		= math<float>::cos( 0.0f );
				stn		= math<float>::sin( 0.0f );
				pos1	= Vec3f( ctn, stn, 0.0f ) * innerRadius;
				pos3	= Vec3f( ctn, stn, 0.0f ) * outerRadius;
			}

			Vec2f texCoord0 = ( pos0.xy() + Vec2f::one() ) * 0.5f;
			Vec2f texCoord1 = ( pos1.xy() + Vec2f::one() ) * 0.5f;
			Vec2f texCoord2 = ( pos2.xy() + Vec2f::one() ) * 0.5f;
			Vec2f texCoord3 = ( pos3.xy() + Vec2f::one() ) * 0.5f;

			positions.push_back( pos0 );
			positions.push_back( pos2 );
			positions.push_back( pos1 );
			positions.push_back( pos1 );
			positions.push_back( pos2 );
			positions.push_back( pos3 );

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

	TriMesh mesh = MeshHelper::create( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createSphere( const Vec2i &resolution )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	float step = (float)M_PI / (float)resolution.y;
	float delta = ((float)M_PI * 2.0f) / (float)resolution.x;

	int32_t p = 0;
	for ( float phi = 0.0f; p <= resolution.y; p++, phi += step ) {
		int32_t t = 0;

		uint32_t a = (uint32_t)( ( p + 0 ) * resolution.x );
		uint32_t b = (uint32_t)( ( p + 1 ) * resolution.x );

		for ( float theta = delta; t < resolution.x; t++, theta += delta ) {
			float sinPhi = math<float>::sin( phi );
			float x = sinPhi * math<float>::cos( theta );
			float y = sinPhi * math<float>::sin( theta );
			float z = -math<float>::cos( phi );
			Vec3f position( x, y, z );
			Vec3f normal = position.normalized();
			Vec2f texCoord = ( normal.xy() + Vec2f::one() ) * 0.5f;

			normals.push_back( normal );
			positions.push_back( position );
			texCoords.push_back( texCoord ); 

			uint32_t n = (uint32_t)( t + 1 >= resolution.x ? 0 : t + 1 );
			indices.push_back( a + t );
			indices.push_back( b + t );
			indices.push_back( a + n );
			indices.push_back( a + n );
			indices.push_back( b + t );
			indices.push_back( b + n );
		}
	}

	for ( vector<uint32_t>::iterator iter = indices.begin(); iter != indices.end(); ) {
		if ( *iter < positions.size() ) {
			++iter;
		} else {
			iter = indices.erase( iter );
		}
	}

	TriMesh mesh = MeshHelper::create( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createSquare( const Vec2i &resolution )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	Vec3f norm0( 0.0f, 0.0f, 1.0f ); 

	Vec2f scale( 1.0f / math<float>::max( (float)resolution.x, 1.0f ), 1.0f / math<float>::max( (float)resolution.y, 1.0f ) );
	uint32_t index = 0;
	for ( int32_t y = 0; y < resolution.y; ++y ) {
		for ( int32_t x = 0; x < resolution.x; ++x, ++index ) {

			float x1 = (float)x * scale.x;
			float y1 = (float)y * scale.y;
			float x2 = (float)( x + 1 ) * scale.x;
			float y2 = (float)( y + 1 ) * scale.y;

			Vec3f pos0( x1 - 0.5f, y1 - 0.5f, 0.0f );
			Vec3f pos1( x2 - 0.5f, y1 - 0.5f, 0.0f );
			Vec3f pos2( x1 - 0.5f, y2 - 0.5f, 0.0f );
			Vec3f pos3( x2 - 0.5f, y2 - 0.5f, 0.0f );
				
			Vec2f texCoord0( x1, y1 );
			Vec2f texCoord1( x2, y1 );
			Vec2f texCoord2( x1, y2 );
			Vec2f texCoord3( x2, y2 );

			positions.push_back( pos2 );
			positions.push_back( pos1 );
			positions.push_back( pos0 );
			positions.push_back( pos1 );
			positions.push_back( pos2 );
			positions.push_back( pos3 );

			texCoords.push_back( texCoord2 );
			texCoords.push_back( texCoord1 );
			texCoords.push_back( texCoord0 );
			texCoords.push_back( texCoord1 );
			texCoords.push_back( texCoord2 );
			texCoords.push_back( texCoord3 );

			for ( uint32_t i = 0; i < 6; ++i ) {
				indices.push_back( index * 6 + i );
				normals.push_back( norm0 );
			}
		}
	}

	TriMesh mesh = MeshHelper::create( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;

}

TriMesh MeshHelper::createTorus( const Vec2i &resolution, float ratio )
{
	vector<uint32_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec3f> srcNormals;
	vector<Vec3f> srcPositions;
	vector<Vec2f> srcTexCoords;
	vector<Vec2f> texCoords;

	float pi			= (float)M_PI;
	float delta			= ( 2.0f * pi ) / (float)resolution.y;
	float step			= ( 2.0f * pi ) / (float)resolution.x;
	float ud			= 1.0f / (float)resolution.y;
	float vd			= 1.0f / (float)resolution.x;

	float outerRadius	= 0.5f / (1.0f + ratio);
	float innerRadius	= outerRadius * ratio;
	
	int32_t p			= 0;
	float v				= 0.0f;
	for ( float phi = 0.0f; p < resolution.x; ++p, v += vd, phi += step ) {
		float cosPhi = math<float>::cos( phi - pi );
		float sinPhi = math<float>::sin( phi - pi );

		int32_t t = 0;
		float u = 0.0f;
		for ( float theta = 0.0f; t < resolution.y; ++t, u += ud, theta += delta ) {
			float cosTheta = math<float>::cos( theta );
			float sinTheta = math<float>::sin( theta );

			float rct	= outerRadius + innerRadius * cosTheta;
			float x		= cosPhi * rct;
			float y		= sinPhi * rct;
			float z		= sinTheta * innerRadius;
			
			Vec3f normal( -cosTheta * cosTheta, sinPhi * cosTheta, sinTheta );
			Vec3f position( x, y, z );
			Vec2f texCoord( u, v );

			positions.push_back( position );
			normals.push_back( normal );
			texCoords.push_back( texCoord );
		}
	}

	for ( p = 0; p < resolution.x; ++p ) {
		int32_t a = ( p + 0 ) * resolution.y;
		int32_t b = ( p + 1 >= resolution.x ? 0 : p + 1 ) * resolution.y;

		for ( int32_t t = 0; t < resolution.y; ++t ) {
			int32_t n = t + 1 >= resolution.y ? 0 : t + 1;

			uint32_t index0 = (uint32_t)( a + t );
			uint32_t index1 = (uint32_t)( a + n );
			uint32_t index2 = (uint32_t)( b + t );
			uint32_t index3 = (uint32_t)( b + n );

			indices.push_back( index0 );
			indices.push_back( index2 );
			indices.push_back( index1 );
			indices.push_back( index1 );
			indices.push_back( index2 );
			indices.push_back( index3 );
		}
	}

	TriMesh mesh = MeshHelper::create( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	srcNormals.clear();
	srcPositions.clear();
	srcTexCoords.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::subdivide( vector<uint32_t> &indices, const vector<Vec3f> &positions, 
	const vector<Vec3f> &normals, const vector<Vec2f> &texCoords, uint32_t division, bool normalize )
{
	TriMesh mesh = create( indices, positions, normals, texCoords );
	return subdivide( mesh, division, normalize );
}

TriMesh MeshHelper::subdivide( const ci::TriMesh &triMesh, uint32_t division, bool normalize )
{
	if ( division <= 1 || triMesh.getNumIndices() == 0 || triMesh.getNumVertices() == 0 ) {
		return triMesh;
	}

	vector<uint32_t> indices	= triMesh.getIndices();
	vector<Vec3f> normals		= triMesh.getNormals();
	vector<Vec3f> positions		= triMesh.getVertices();
	vector<Vec2f> texCoords		= triMesh.getTexCoords();

	vector<uint32_t> indicesBuffer( indices );
	indices.clear();
	indices.reserve( indicesBuffer.size() * 4 );
 
	uint32_t index0;
	uint32_t index1;
	uint32_t index2;
	uint32_t index3;
	uint32_t index4;
	uint32_t index5;
	for ( vector<uint32_t>::const_iterator iter = indicesBuffer.begin(); iter != indicesBuffer.end(); ) {
		index0 = *iter;
		++iter;
		index1 = *iter;
		++iter;
		index2 = *iter;
		++iter;

		if ( normalize ) {
			index3 = positions.size();
			positions.push_back( positions.at( index0 ).lerp( 0.5f, positions.at( index1 ) ).normalized() * 0.5f );
			index4 = positions.size();
			positions.push_back( positions.at( index1 ).lerp( 0.5f, positions.at( index2 ) ).normalized() * 0.5f );
			index5 = positions.size();
			positions.push_back( positions.at( index2 ).lerp( 0.5f, positions.at( index0 ) ).normalized() * 0.5f );
		} else {
			index3 = positions.size(); 
			positions.push_back( positions.at( index0 ).lerp( 0.5f, positions.at( index1 ) ) );
			index4 = positions.size();
			positions.push_back( positions.at( index1 ).lerp( 0.5f, positions.at( index2 ) ) );
			index5 = positions.size();
			positions.push_back( positions.at( index2 ).lerp( 0.5f, positions.at( index0 ) ) );
		}
	
		if ( !normals.empty() ) {
			normals.push_back( normals.at( index0 ).lerp( 0.5f, normals.at( index1 ) ) );
			normals.push_back( normals.at( index1 ).lerp( 0.5f, normals.at( index2 ) ) );
			normals.push_back( normals.at( index2 ).lerp( 0.5f, normals.at( index0 ) ) );
		}

		if ( !texCoords.empty() ) {
			texCoords.push_back( texCoords.at( index0 ).lerp( 0.5f, texCoords.at( index1 ) ) );
			texCoords.push_back( texCoords.at( index1 ).lerp( 0.5f, texCoords.at( index2 ) ) );
			texCoords.push_back( texCoords.at( index2 ).lerp( 0.5f, texCoords.at( index0 ) ) );
		}

		indices.push_back( index0 ); 
		indices.push_back( index3 ); 
		indices.push_back( index5 );
		
		indices.push_back( index3 ); 
		indices.push_back( index1 );
		indices.push_back( index4 );
		
		indices.push_back( index5 ); 
		indices.push_back( index4 ); 
		indices.push_back( index2 );
		
		indices.push_back( index3 ); 
		indices.push_back( index4 ); 
		indices.push_back( index5 );
	}

	ci::TriMesh mesh = create( indices, positions, normals, texCoords );
	
	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return subdivide( mesh, division - 1, normalize );
}
