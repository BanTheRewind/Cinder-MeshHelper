varying vec3		normal;
varying vec4		position;
varying vec2		uv;

uniform bool		glow;
uniform sampler2D	tex;

void main( void )
{
	gl_FragData[ 0 ] = vec4( 1.0 ) * texture2D( tex, uv );
	gl_FragData[ 1 ] = vec4( glow ? 1.0 : 0.0 );
	gl_FragData[ 2 ] = vec4( normal, 1.0 );
	gl_FragData[ 3 ] = position;
}
