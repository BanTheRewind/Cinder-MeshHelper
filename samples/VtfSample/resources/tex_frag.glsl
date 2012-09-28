uniform float theta;

varying vec2 uv;
const float twoPi = 3.1415926 * 2.0;

void main( void )
{
	vec2 offset		= vec2( cos( theta * twoPi ), sin( theta * twoPi ) ) * 0.25 + vec2( 0.5, 0.5 );
	float dist		= distance( uv, offset );
	gl_FragColor	= vec4( dist, 0.0, 0.0, 1.0 );
}
