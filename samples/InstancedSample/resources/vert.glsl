//#extension GL_ARB_draw_instanced	: enable
#extension GL_EXT_gpu_shader4		: enable // Uncomment if ARB doesn't work
#extension GL_EXT_draw_instanced	: enable // Uncomment if ARB doesn't work 

uniform vec2 size;
uniform vec2 spacing;

varying vec2 uv;
varying vec3 normal;
varying vec4 position;

void main()
{
	normal			= gl_Normal;
	position		= gl_Vertex;
	uv				= gl_MultiTexCoord0.st;

	float x			= float( mod( float( gl_InstanceID ), size.x ) ) * spacing.x;
	float z			= float( floor( gl_InstanceID / size.x ) ) * -spacing.y;
	x				-= spacing.x * size.x * 0.5;
	z				+= spacing.y * size.y * 0.5;
	vec3 offset		= vec3( x, -1.0, z );
	position.xyz	+= offset;

	gl_Position		= gl_ModelViewProjectionMatrix * position;
}
