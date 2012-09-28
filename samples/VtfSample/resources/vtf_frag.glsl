uniform bool		lightingEnabled;
uniform vec3		eyePoint;
uniform sampler2D	tex;
uniform bool		textureEnabled;

varying vec3		normal;
varying vec4		position;
varying vec2		uv;

void main( void )
{
	vec4 color			= vec4( 0.0, 0.0, 0.0, 0.0 );
	if ( textureEnabled ) {
		color			= texture2D( tex, uv );
	}
	if ( lightingEnabled ) {
		float shininess	= 20.0;
		vec3 eye		= normalize( -eyePoint );
		vec3 light		= normalize( gl_LightSource[ 0 ].position.xyz - position.xyz );   
		vec3 reflection = normalize( -reflect( light, normal ) );

		vec4 ambient	= gl_LightSource[ 0 ].ambient;
		vec4 diffuse	= clamp( gl_LightSource[ 0 ].diffuse * max( dot( normal.xyz, light ), 0.0 ), 0.0, 1.0 );     
		vec4 specular	= clamp( gl_LightSource[ 0 ].specular * pow( max( dot( reflection, eye ), 0.0 ), 0.3 * shininess ), 0.0, 1.0 ); 

		color			+= ambient + diffuse + specular;
	} else {
		color			= vec4( 1.0, 1.0, 1.0, 1.0 );
	}

	gl_FragColor	= color;
}
