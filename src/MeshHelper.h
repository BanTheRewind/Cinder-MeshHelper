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

#pragma once

#include "cinder/TriMesh.h"

class MeshHelper 
{
public:
	//! Create TriMesh from vectors of vertex data.
	static ci::TriMesh		create( std::vector<uint32_t> &indices, const std::vector<ci::Vec3f> &positions,
									const std::vector<ci::Vec3f> &normals, const std::vector<ci::Vec2f> &texCoords );
	/*! Subdivide vectors of vertex data into a TriMesh \a division times. Division less 
		than 2 returns the original mesh. */
	static ci::TriMesh		subdivide( std::vector<uint32_t> &indices, const std::vector<ci::Vec3f> &positions,
								const std::vector<ci::Vec3f> &normals, const std::vector<ci::Vec2f> &texCoords, 
								uint32_t division = 2, bool normalize = false );
	//! Subdivide a TriMesh \a division times. Division less than 2 returns the original mesh. 
	static ci::TriMesh		subdivide( const ci::TriMesh &triMesh, uint32_t division = 2, bool normalize = false );

	//! Create circle TriMesh with a radius of 1.0 and \a resolution segments.
	static ci::TriMesh		createCircle( const ci::Vec2i &resolution = ci::Vec2i( 12, 1 ) );
	//! Create cube TriMesh with an edge length of 1.0 divided into \a resolution segments.
	static ci::TriMesh		createCube( const ci::Vec3i &resolution = ci::Vec3i::one() );
	/*! Create cylinder TriMesh with a height of 1.0, top radius of \a topRadius, base radius 
		of \a baseRadius and \a resolution segments. Top and base are closed with \a closeTop and 
		\a closeBase flags. */
	static ci::TriMesh		createCylinder( const ci::Vec2i &resolution = ci::Vec2i( 12, 6 ), 
		float topRadius = 1.0f, float baseRadius = 1.0f, bool closeTop = true, bool closeBase = true );
	//! Creates icosahedron where each face is subdivided \b division times.
	static ci::TriMesh		createIcosahedron( uint32_t division = 1 );
	/*! Create ring TriMesh with a radius of 1.0, \a resolution segments, and second radius 
		of \a ratio. */
	static ci::TriMesh		createRing( const ci::Vec2i &resolution = ci::Vec2i( 12, 1 ), 
		float ratio = 0.5f );
	//! Create sphere TriMesh with a radius of 1.0 and \a resolution segments.
	static ci::TriMesh		createSphere( const ci::Vec2i &resolution = ci::Vec2i( 12, 6 ) );
	//! Create square TriMesh with an edge length of 1.0 divided into \a resolution segments.
	static ci::TriMesh		createSquare( const ci::Vec2i &resolution = ci::Vec2i::one() );
	/*! Create torus TriMesh with a radius of 1.0, \a resolution segments, and second radius 
		of \a ratio. */
	static ci::TriMesh		createTorus( const ci::Vec2i &resolution = ci::Vec2i( 12, 6 ), 
		float ratio = 0.5f );

/*private:

	// TODO use to generate icosahedron star
	struct VertexDistance
	{
		float		mDistance;
		uint32_t	mIndex;

		bool		operator==( const VertexDistance &rhs ) { return mDistance == rhs.mDistance; }
		bool		operator!=( const VertexDistance &rhs ) { return mDistance != rhs.mDistance; }
		bool		operator<( const VertexDistance &rhs ) { return mDistance < rhs.mDistance; }
		bool		operator<=( const VertexDistance &rhs ) { return mDistance <= rhs.mDistance; }
		bool		operator>( const VertexDistance &rhs ) { return mDistance > rhs.mDistance; }
		bool		operator>=( const VertexDistance &rhs ) { return mDistance >= rhs.mDistance; }
	};*/
};
