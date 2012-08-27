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

#if ! defined( CINDER_COCOA_TOUCH )
	#include "cinder/gl/Vbo.h"
#endif
#include "cinder/TriMesh.h"
#include <map>

class MeshHelper 
{
public:
	//! Create TriMesh from vectors of vertex data
	static ci::TriMesh		createTriMesh( std::vector<size_t> &indices, const std::vector<ci::Vec3f> &positions, 
								const std::vector<ci::Vec3f> &normals, const std::vector<ci::Vec2f> &texCoords );

	//! Create circle TriMesh with a radius of 1.0 and \a segments
	static ci::TriMesh		createCircleTriMesh( size_t segments );
	//! Create cone TriMesh with a radius and height of 1.0 and \a segments
	static ci::TriMesh		createConeTriMesh( size_t segments );
	//! Create cube TriMesh with an edge length of 1.0
	static ci::TriMesh		createCubeTriMesh();
	//! Create cylinder TriMesh with a radius and height of 1.0 and \a segments
	static ci::TriMesh		createCylinderTriMesh( size_t segments );
	//! Create sphere TriMesh with a radius of 1.0 and \a segments
	static ci::TriMesh		createSphereTriMesh( size_t segments );
	//! Create square TriMesh with an edge length of 1.0
	static ci::TriMesh		createSquareTriMesh();

#if ! defined( CINDER_COCOA_TOUCH )
	//! Create VboMesh from vectors of vertex data
	static ci::gl::VboMesh	createVboMesh( const std::vector<size_t> &indices, const std::vector<ci::Vec3f> &positions, 
								const std::vector<ci::Vec3f> &normals, const std::vector<ci::Vec2f> &texCoords, 
								GLenum primitiveType = GL_TRIANGLES );
	
	//! Create circle VboMesh with a radius of 1.0 and \a segments
	static ci::gl::VboMesh	createCircleVboMesh( size_t segments );
	//! Create cone VboMesh with a radius and height of 1.0 and \a segments
	static ci::gl::VboMesh	createConeVboMesh( size_t segments );
	//! Create cube VboMesh with an edge length of 1.0
	static ci::gl::VboMesh	createCubeVboMesh();
	//! Create cylinder VboMesh with a radius and height of 1.0 and \a segments
	static ci::gl::VboMesh	createCylinderVboMesh( size_t segments );
	//! Create sphere VboMesh with a radius of 1.0 and \a segments
	static ci::gl::VboMesh	createSphereVboMesh( size_t segments );
	//! Create square VboMesh with an edge length of 1.0
	static ci::gl::VboMesh	createSquareVboMesh();
#endif
};
