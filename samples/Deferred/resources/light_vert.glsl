uniform vec3	lightPosition;

varying float	radius;

void main( void )
{
	vec4 p		= gl_ModelViewProjectionMatrix * gl_Vertex;
	radius		= length( p - lightPosition );
	gl_Position	= p;
}
