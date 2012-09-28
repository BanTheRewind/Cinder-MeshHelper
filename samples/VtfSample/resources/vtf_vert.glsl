uniform float		height;
uniform sampler2D	displacement;
uniform vec3		scale;

varying vec3		normal;
varying vec4		position;
varying vec2		uv;

void main( void )
{
	normal			= gl_Normal;
	uv				= gl_MultiTexCoord0.st;
	vec4 color		= texture2D( displacement, uv );
	float offset	= color.r * height;
	position		= gl_Vertex * vec4( scale, 1.0 );
	position.xyz	+= normal * scale * offset;
	gl_Position		= gl_ModelViewProjectionMatrix * position;
}
