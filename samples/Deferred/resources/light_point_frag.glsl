uniform vec3		eyePoint;
uniform vec4		lightAmbient;
uniform float		lightAttenuationConstant;
uniform float		lightAttenuationLinear;
uniform float		lightAttenuationQuadratic;
uniform vec4		lightDiffuse;
uniform vec3		lightPosition;
uniform vec4		lightSpecular;
uniform vec4		materialAmbient;
uniform vec4		materialDiffuse;
uniform vec4		materialSpecular;
uniform float		materialShininess;
uniform sampler2D	texAlbedo;
uniform sampler2D	texGlow;
uniform sampler2D	texNormal;
uniform sampler2D	texPosition;
uniform vec2		windowSize;

varying float		radius;

void main( void )
{
	vec4 color		= vec4( 0.0 );
	vec2 uv			= gl_FragCoord.xy / windowSize;

	vec3 position	= texture2D( texPosition,	uv ).xyz;
	vec4 glow		= texture2D( texGlow,		uv );
	if ( position.z > -0.0001 || glow.r > 0.0 ) {
		discard;
	}
	
	vec4 albedo		= texture2D( texAlbedo,		uv );
	vec3 normal		= texture2D( texNormal,		uv ).xyz;

	vec4 ambient	= materialAmbient	* lightAmbient;
	vec4 diffuse	= materialDiffuse	* lightDiffuse;
	vec4 specular	= materialSpecular	* lightSpecular;

	vec3 lightDir	= lightPosition * vec3( 1.0, 1.0, -1.0 ) - position;
	
	vec3 e			= normalize( -position );
	vec3 l			= normalize( lightDir );
	vec3 n			= normalize( normal );
	vec3 r			= normalize( reflect( -l, n ) );

	float d			= length( lightDir );
	float NdotL		= max( dot( n, l ), 0.0 );
	float denom		= ( 
						lightAttenuationConstant			+ 
						lightAttenuationLinear		* d		+ 
						lightAttenuationQuadratic	* d * d 
					);
	float att		= 1.0 / ( denom == 0.0 ? 1.0 : denom );
	vec3 halfV 		= normalize( l + e );
	float NdotHV	= max( dot( n, halfV ), 0.0 );
	specular		*= pow( NdotHV, materialShininess );
	
	float falloff	= 1.0 - clamp( radius / d, 0.0, 1.0 );

	color			= albedo;
	color			*= att * ( diffuse * NdotL + ambient );
	color			+= att * specular;

	//color			*= falloff;
	color.a			= 1.0;

	gl_FragColor	= color;
}
