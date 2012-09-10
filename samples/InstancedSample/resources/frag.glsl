uniform vec3 eyePoint;
uniform sampler2D tex;

varying vec3 normal;
varying vec4 position;
varying vec2 uv;

void main( void )
{
	float shininess	= 500.0;
	vec4 color		= texture2D( tex, uv );

	vec3 eye		= normalize( -eyePoint );
	vec3 light		= normalize( gl_LightSource[ 0 ].position.xyz - position.xyz );   
	vec3 reflection = normalize( -reflect( light, normal ) );

	vec4 ambient	= gl_LightSource[ 0 ].ambient;
	vec4 diffuse	= clamp( gl_LightSource[ 0 ].diffuse * max( dot( normal.xyz, light ), 0.0 ), 0.0, 1.0 );     
	vec4 specular	= clamp( gl_LightSource[ 0 ].specular * pow( max( dot( reflection, eye ), 0.0 ), 0.3 * shininess ), 0.0, 1.0 ); 

	color			+= ambient + diffuse + specular;

	gl_FragColor	= color;
}
