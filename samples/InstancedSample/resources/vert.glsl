#extension GL_EXT_draw_instanced : enable

uniform vec2 size;
uniform vec2 spacing;

void main()
{
	float x			= mod( gl_InstanceID, size.x );
	float y			= floor( gl_InstanceID / size.x ) * size.y;
	vec4 offset		= vec4( x * spacing.x, -y * spacing.y, 0.0, 0.0 );
	offset.x		-= x * spacing.x * 0.5;
	vec4 position	= gl_Vertex + offset;
	gl_Position		= gl_ModelViewProjectionMatrix * position;
}
