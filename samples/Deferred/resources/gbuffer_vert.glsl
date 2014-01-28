varying vec3 normal;
varying vec4 position;
varying vec2 uv;

void main( void )
{
	normal		= normalize( gl_NormalMatrix * gl_Normal );
	position	= gl_ModelViewMatrix * gl_Vertex;
	uv			= gl_MultiTexCoord0.st;
	gl_Position	= gl_ModelViewProjectionMatrix * gl_Vertex;
}
 