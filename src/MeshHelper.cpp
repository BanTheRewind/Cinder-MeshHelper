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

TriMesh MeshHelper::createTriMesh( vector<size_t> &indices, const vector<Vec3f> &positions, 
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

TriMesh MeshHelper::createCircleTriMesh( size_t segments )
{
	vector<size_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	Vec3f norm0( 0.0f, 0.0f, 1.0f );

	Vec3f vert1		= Vec3f::zero();
	Vec2f texCoord1 = Vec2f::one() * 0.5f;

	float delta = ( (float)M_PI * 2.0f ) / (float)segments;
	float theta = delta;
	for ( size_t i = 0; i < segments; ++i, theta += delta ) {

		Vec3f vert0( math<float>::cos( theta ), math<float>::sin( theta ), 0.0f );
		Vec3f vert2 = Vec3f::zero();
		if ( i >= segments - 1 ) {
			vert2.x = math<float>::cos( delta );
			vert2.y = math<float>::sin( delta ); 
		} else {
			vert2.x = math<float>::cos( theta + delta );
			vert2.y = math<float>::sin( theta + delta );
		}
		Vec2f texCoord0 = ( vert0.xy() + Vec2f::one() ) * 0.5f;
		Vec2f texCoord2 = ( vert2.xy() + Vec2f::one() ) * 0.5f;

		positions.push_back( vert0 );
		positions.push_back( vert1 );
		positions.push_back( vert2 );

		texCoords.push_back( texCoord0 );
		texCoords.push_back( texCoord1 );
		texCoords.push_back( texCoord2 );

		for ( size_t j = 0; j < 3; ++j ) {
			indices.push_back( i * 3 + j );
			normals.push_back( norm0 );
		}

	}

	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createConeTriMesh( size_t segments, bool closeBase )
{
	vector<size_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec3f> srcPositions;
	vector<Vec2f> srcTexCoords;
	vector<Vec2f> texCoords;

	float delta = 1.0f / (float)segments;

	for ( size_t p = 0; p < 2; p++ ) {
		float radius = p == 0 ? 1.0f : 0.0f;

		size_t i = 0;
		for ( float theta = delta; i < segments; i++, theta += delta ) {

			float t = 2.0f * (float)M_PI * theta;
			float cosT = math<float>::cos( t );
			float sinT = math<float>::sin( t );
			Vec3f position( 
				cosT * radius, 
				(float)p, 
				sinT * radius 
				);
			srcPositions.push_back( position );

			Vec2f texCoord( theta, position.y );
			srcTexCoords.push_back( texCoord );
		}

	}

	srcPositions.push_back( Vec3f( 0.0f, 0.0f, 0.0f ) );
	srcPositions.push_back( Vec3f( 0.0f, 1.0f, 0.0f ) );
	srcTexCoords.push_back( Vec2f( 0.0f, 0.0f ) );
	srcTexCoords.push_back( Vec2f( 0.0f, 1.0f ) );
	int32_t bottomCenter = (int32_t)srcPositions.size() - 1;
	int32_t topCenter = bottomCenter - 1;

	Vec3f offset( 0.0f, -0.5f, 0.0f );

	for ( size_t t = 0; t < segments; t++ ) {
		size_t n = t + 1 >= segments ? 0 : t + 1;

		size_t index0 = t;
		size_t index1 = n;
		size_t index2 = segments + t;
		size_t index3 = segments + n;

		Vec3f vert0 = srcPositions[ index0 ];
		Vec3f vert1 = srcPositions[ index1 ];
		Vec3f vert2 = srcPositions[ index2 ];
		Vec3f vert3 = srcPositions[ index3 ];

		Vec2f texCoord0 = srcTexCoords[ index0 ];
		Vec2f texCoord1 = srcTexCoords[ index1 ];
		Vec2f texCoord2 = srcTexCoords[ index2 ];
		Vec2f texCoord3 = srcTexCoords[ index3 ];

		Vec3f norm0 = vert0.normalized();
		Vec3f norm1 = vert1.normalized();
		Vec3f norm2 = norm0;
		Vec3f norm3 = norm1;

		normals.push_back( norm0 );
		normals.push_back( norm2 );
		normals.push_back( norm1 );
		normals.push_back( norm1 );
		normals.push_back( norm2 );
		normals.push_back( norm3 );

		positions.push_back( vert0 + offset );
		positions.push_back( vert2 + offset );
		positions.push_back( vert1 + offset );
		positions.push_back( vert1 + offset );
		positions.push_back( vert2 + offset );
		positions.push_back( vert3 + offset );

		texCoords.push_back( texCoord0 );
		texCoords.push_back( texCoord2 );
		texCoords.push_back( texCoord1 );
		texCoords.push_back( texCoord1 );
		texCoords.push_back( texCoord2 );
		texCoords.push_back( texCoord3 );
	}

	if ( closeBase ) {
		Vec3f normal( 0.0f, -1.0f, 0.0f );
		for ( size_t t = 0; t < segments; t++ ) {
			size_t n = t + 1 >= segments ? 0 : t + 1;

			normals.push_back( normal );
			normals.push_back( normal );
			normals.push_back( normal );

			positions.push_back( srcPositions[ topCenter ] + offset );
			positions.push_back( srcPositions[ t ] + offset );
			positions.push_back( srcPositions[ n ] + offset );

			texCoords.push_back( srcTexCoords[ topCenter ] );
			texCoords.push_back( srcTexCoords[ topCenter ] );
			texCoords.push_back( srcTexCoords[ topCenter ] );
		}
	}

	for ( size_t i = 0; i < positions.size(); i++ ) {
		indices.push_back( i );
	}

	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );
		
	indices.clear();
	normals.clear();
	positions.clear();
	srcPositions.clear();
	srcTexCoords.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createCubeTriMesh()
{
	vector<size_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	Vec3f size = Vec3f::one() * 0.5f;
	Vec3f pos0 = Vec3f( 1.0f, 1.0f, 1.0f ) * size;
	Vec3f pos1 = Vec3f( 1.0f, -1.0f, 1.0f ) * size;
	Vec3f pos2 = Vec3f( 1.0f, -1.0f, -1.0f ) * size;
	Vec3f pos3 = Vec3f( 1.0f, 1.0f, -1.0f ) * size;
	Vec3f pos4 = Vec3f( -1.0f, 1.0f, -1.0f ) * size;
	Vec3f pos5 = Vec3f( -1.0f, 1.0f, 1.0f ) * size;
	Vec3f pos6 = Vec3f( -1.0f, -1.0f, -1.0f ) * size;
	Vec3f pos7 = Vec3f( -1.0f, -1.0f, 1.0f ) * size;

	Vec3f norm0( 1.0f, 0.0f, 0.0f );
	Vec3f norm1( 0.0f, 1.0f, 0.0f );
	Vec3f norm2( 0.0f, 0.0f, 1.0f );
	Vec3f norm3( -1.0f, 0.0f, 0.0f ); 
	Vec3f norm4( 0.0f, -1.0f, 0.0f ); 
	Vec3f norm5( 0.0f, 0.0f, -1.0f ); 

	positions.push_back( pos0 ); 
	positions.push_back( pos1 ); 	
	positions.push_back( pos2 ); 	
	positions.push_back( pos3 );

	positions.push_back( pos0 ); 
	positions.push_back( pos3 ); 	
	positions.push_back( pos4 ); 	
	positions.push_back( pos5 );

	positions.push_back( pos0 ); 	
	positions.push_back( pos5 ); 	
	positions.push_back( pos7 ); 	
	positions.push_back( pos1 );

	positions.push_back( pos5 ); 	
	positions.push_back( pos4 ); 	
	positions.push_back( pos6 ); 	
	positions.push_back( pos7 );

	positions.push_back( pos6 ); 	
	positions.push_back( pos2 ); 	
	positions.push_back( pos1 ); 	
	positions.push_back( pos7 );

	positions.push_back( pos2 ); 	
	positions.push_back( pos6 ); 	
	positions.push_back( pos4 ); 	
	positions.push_back( pos3 );

	for ( uint8_t i = 0; i < 4; i++ ) {
		normals.push_back( norm0 );
	}
	for ( uint8_t i = 0; i < 4; i++ ) {
		normals.push_back( norm1 );
	}
	for ( uint8_t i = 0; i < 4; i++ ) {
		normals.push_back( norm2 );
	}
	for ( uint8_t i = 0; i < 4; i++ ) {
		normals.push_back( norm3 );
	}
	for ( uint8_t i = 0; i < 4; i++ ) {
		normals.push_back( norm4 );
	}
	for ( uint8_t i = 0; i < 4; i++ ) {
		normals.push_back( norm5 );
	}

	Vec2f texCoord0( 0.0f, 0.0f );
	Vec2f texCoord1( 1.0f, 0.0f );
	Vec2f texCoord2( 1.0f, 1.0f );
	Vec2f texCoord3( 0.0f, 1.0f );

	texCoords.push_back( texCoord3 );
	texCoords.push_back( texCoord2 );
	texCoords.push_back( texCoord1 );
	texCoords.push_back( texCoord0 );

	texCoords.push_back( texCoord2 );
	texCoords.push_back( texCoord1 );
	texCoords.push_back( texCoord0 );
	texCoords.push_back( texCoord3 );

	texCoords.push_back( texCoord3 );
	texCoords.push_back( texCoord2 );
	texCoords.push_back( texCoord1 );
	texCoords.push_back( texCoord0 );

	texCoords.push_back( texCoord2 );
	texCoords.push_back( texCoord1 );
	texCoords.push_back( texCoord0 );
	texCoords.push_back( texCoord3 );

	texCoords.push_back( texCoord1 );
	texCoords.push_back( texCoord0 );
	texCoords.push_back( texCoord3 );
	texCoords.push_back( texCoord2 );
		
	texCoords.push_back( texCoord1 );
	texCoords.push_back( texCoord0 );			
	texCoords.push_back( texCoord3 );
	texCoords.push_back( texCoord2 );

	indices.push_back( 0 );
	indices.push_back( 1 );
	indices.push_back( 2 );
	indices.push_back( 0 );
	indices.push_back( 2 );
	indices.push_back( 3 );
		
	indices.push_back( 4 );
	indices.push_back( 5 );
	indices.push_back( 6 );
	indices.push_back( 4 );
	indices.push_back( 6 );
	indices.push_back( 7 );
		
	indices.push_back( 8 );
	indices.push_back( 9 );
	indices.push_back( 10 );
	indices.push_back( 8 );
	indices.push_back( 10 );
	indices.push_back( 11 );
		
	indices.push_back( 12 );
	indices.push_back( 13 );
	indices.push_back( 14 );
	indices.push_back( 12 );
	indices.push_back( 14 );
	indices.push_back( 15 );
		
	indices.push_back( 16 );
	indices.push_back( 17 );
	indices.push_back( 18 );
	indices.push_back( 16 );
	indices.push_back( 18 );
	indices.push_back( 19 );
		
	indices.push_back( 20 );
	indices.push_back( 21 );
	indices.push_back( 22 );
	indices.push_back( 20 );
	indices.push_back( 22 );
	indices.push_back( 23 );

	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createCylinderTriMesh( size_t segments )
{
	vector<size_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec3f> srcNormals;
	vector<Vec3f> srcPositions;
	vector<Vec2f> srcTexCoords;
	vector<Vec2f> texCoords;

	float delta = 1.0f / (float)segments;

	for ( size_t p = 0; p < 2; p++ ) {
		size_t t = 0;
		for ( float theta = delta; t < segments; t++, theta += delta ) {
			float t = 2.0f * (float)M_PI * theta;
			Vec3f position( 
				math<float>::cos( t ), 
				(float)p - 0.5f, 
				math<float>::sin( t ) 
				);
			srcPositions.push_back( position );

			Vec3f normal = position.normalized();
			normal.y = 0.0f;
			srcNormals.push_back( normal );

			Vec2f texCoord( theta, position.y + 0.5f );
			srcTexCoords.push_back( texCoord );
		}
	}

	srcNormals.push_back( Vec3f( 0.0f, -1.0f, 0.0f ) );
	srcNormals.push_back( Vec3f( 0.0f, 1.0f, 0.0f ) );
	srcPositions.push_back( Vec3f( 0.0f, -0.5f, 0.0f ) );
	srcPositions.push_back( Vec3f( 0.0f, 0.5f, 0.0f ) );
	srcTexCoords.push_back( Vec2f( 0.0f, 0.0f ) );
	srcTexCoords.push_back( Vec2f( 0.0f, 1.0f ) );
	int32_t bottomCenter = (int32_t)srcPositions.size() - 1;
	int32_t topCenter = bottomCenter - 1;

	for ( size_t t = 0; t < segments; t++ ) {
		size_t n = t + 1 >= segments ? 0 : t + 1;

		normals.push_back( srcNormals[ bottomCenter ] );
		normals.push_back( srcNormals[ bottomCenter ] );
		normals.push_back( srcNormals[ bottomCenter ] );

		positions.push_back( srcPositions[ bottomCenter ] );
		positions.push_back( srcPositions[ segments + t ] );
		positions.push_back( srcPositions[ segments + n ] );

		texCoords.push_back( srcTexCoords[ bottomCenter ] );
		texCoords.push_back( srcTexCoords[ bottomCenter ] );
		texCoords.push_back( srcTexCoords[ bottomCenter ] );
	}

	for ( size_t t = 0; t < segments; t++ ) {
		size_t n = t + 1 >= segments ? 0 : t + 1;
		
		size_t index0 = t;
		size_t index1 = n;
		size_t index2 = segments + t;
		size_t index3 = segments + n;

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

	for ( size_t t = 0; t < segments; t++ ) {
		size_t n = t + 1 >= segments ? 0 : t + 1;

		normals.push_back( srcNormals[ topCenter ] );
		normals.push_back( srcNormals[ topCenter ] );
		normals.push_back( srcNormals[ topCenter ] );

		positions.push_back( srcPositions[ topCenter ] );
		positions.push_back( srcPositions[ t ] );
		positions.push_back( srcPositions[ n ] );

		texCoords.push_back( srcTexCoords[ topCenter ] );
		texCoords.push_back( srcTexCoords[ topCenter ] );
		texCoords.push_back( srcTexCoords[ topCenter ] );
	}

	for ( size_t i = 0; i < positions.size(); i++ ) {
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

TriMesh MeshHelper::createRingTriMesh( size_t segments, float secondRadius )
{
	vector<size_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	Vec3f norm0( 0.0f, 0.0f, 1.0f );

	float delta = ( (float)M_PI * 2.0f ) / (float)segments;
	float theta = delta;
	for ( size_t i = 0; i < segments; ++i, theta += delta ) {

		Vec3f vert0( math<float>::cos( theta ), math<float>::sin( theta ), 0.0f );
		Vec3f vert1( math<float>::cos( theta + delta ), math<float>::sin( theta + delta ), 0.0f );
		Vec3f vert2 = vert0 * secondRadius;
		Vec3f vert3 = vert1 * secondRadius;
		if ( i >= segments - 1 ) {
			vert1.x = math<float>::cos( delta );
			vert1.y = math<float>::sin( delta ); 
			vert3	= vert1 * secondRadius;
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

		for ( size_t j = 0; j < 6; ++j ) {
			indices.push_back( i * 6 + j );
			normals.push_back( norm0 );
		}

	}

	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;
}

TriMesh MeshHelper::createSphereTriMesh( size_t segments )
{
	vector<size_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	size_t layers = segments / 2;
	float step = (float)M_PI / (float)layers;
	float delta = ((float)M_PI * 2.0f) / (float)segments;

	size_t p = 0;
	for ( float phi = 0.0f; p <= layers; p++, phi += step ) {
		size_t t = 0;
		for ( float theta = delta; t < segments; t++, theta += delta ) {
			float sinP = math<float>::sin( phi );
			Vec3f position(
				sinP * math<float>::cos( theta ),
				sinP * math<float>::sin( theta ),
				-math<float>::cos( phi ) );
			positions.push_back(position);

			Vec3f normal = position.normalized();
			normals.push_back( normal );

			texCoords.push_back( ( normal.xy() + Vec2f::one() ) * 0.5f ); 

			size_t n = t + 1 >= segments ? 0 : t + 1;
			indices.push_back( p * segments + t );
			indices.push_back( ( p + 1 ) * segments + t );
			indices.push_back( p * segments + n );
			indices.push_back( p * segments + n );
			indices.push_back( ( p + 1 ) * segments + t );
			indices.push_back( ( p + 1 ) * segments + n );
		}
	}

	for ( vector<size_t>::iterator iter = indices.begin(); iter != indices.end(); ) {
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

TriMesh MeshHelper::createSquareTriMesh()
{
	vector<size_t> indices;
	vector<Vec3f> normals;
	vector<Vec3f> positions;
	vector<Vec2f> texCoords;

	Vec3f size = Vec3f::one() * 0.5f;
	Vec3f pos0 = Vec3f( -1.0f, -1.0f, 0.0f ) * size;
	Vec3f pos1 = Vec3f( 1.0f, -1.0f, 0.0f ) * size;
	Vec3f pos2 = Vec3f( -1.0f, 1.0f, 0.0f ) * size;
	Vec3f pos3 = Vec3f( 1.0f, 1.0f, 0.0f ) * size;

	Vec3f norm0( 0.0f, 0.0f, 1.0f ); 

	Vec2f texCoord0( 0.0f, 0.0f );
	Vec2f texCoord1( 1.0f, 0.0f );
	Vec2f texCoord2( 0.0f, 1.0f );
	Vec2f texCoord3( 1.0f, 1.0f );

	positions.push_back( pos0 );
	positions.push_back( pos1 );
	positions.push_back( pos2 ); 	
	positions.push_back( pos3 );

	for ( uint8_t i = 0; i < 4; i++ ) {
		normals.push_back( norm0 );
	}

	texCoords.push_back( texCoord0 );
	texCoords.push_back( texCoord1 );
	texCoords.push_back( texCoord2 );
	texCoords.push_back( texCoord3 );

	indices.push_back( 0 );
	indices.push_back( 1 );
	indices.push_back( 2 );
	indices.push_back( 2 );
	indices.push_back( 1 );
	indices.push_back( 3 );

	TriMesh mesh = MeshHelper::createTriMesh( indices, positions, normals, texCoords );

	indices.clear();
	normals.clear();
	positions.clear();
	texCoords.clear();

	return mesh;

}

#if ! defined( CINDER_COCOA_TOUCH )

gl::VboMesh MeshHelper::createVboMesh( const vector<size_t> &indices, const vector<Vec3f> &positions, 
	const vector<Vec3f> &normals, const vector<Vec2f> &texCoords, GLenum primitiveType )
{
	ci::gl::VboMesh::Layout layout;
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

gl::VboMesh MeshHelper::createCircleVboMesh( size_t segments )
{
	TriMesh mesh = createCircleTriMesh( segments );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createConeVboMesh( size_t segments, bool closeBase )
{
	TriMesh mesh = createConeTriMesh( segments, closeBase );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createCubeVboMesh()
{
	TriMesh mesh = createCubeTriMesh();
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createCylinderVboMesh( size_t segments )
{
	TriMesh mesh = createCylinderTriMesh( segments );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createRingVboMesh( size_t segments, float secondRadius )
{
	TriMesh mesh = createRingTriMesh( segments, secondRadius );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createSphereVboMesh( size_t segments )
{
	TriMesh mesh = createSphereTriMesh( segments );
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

gl::VboMesh MeshHelper::createSquareVboMesh()
{
	TriMesh mesh = createSquareTriMesh();
	return createVboMesh( mesh.getIndices(), mesh.getVertices(), mesh.getNormals(), mesh.getTexCoords() );
}

#endif
