varying vec2 uv;

void main()
{
	uv			= gl_MultiTexCoord0.st;
	gl_Position	= gl_ModelViewProjectionMatrix * gl_Vertex;
}
