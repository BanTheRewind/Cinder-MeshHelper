uniform float		attenuation;
uniform vec2		size;
uniform sampler2D	tex;

varying vec2		uv;

void main()
{
	vec4 sum = texture2D( tex, uv );
	sum *= 0.086826196862124602;
	sum += texture2D( tex, uv + -10.0 * size ) * 0.009167927656011385;
	sum += texture2D( tex, uv +  -9.0 * size ) * 0.014053461291849008;
	sum += texture2D( tex, uv +  -8.0 * size ) * 0.020595286319257878;
	sum += texture2D( tex, uv +  -7.0 * size ) * 0.028855245532226279;
	sum += texture2D( tex, uv +  -6.0 * size ) * 0.038650411513543079;
	sum += texture2D( tex, uv +  -5.0 * size ) * 0.049494378859311142;
	sum += texture2D( tex, uv +  -4.0 * size ) * 0.060594058578763078;
	sum += texture2D( tex, uv +  -3.0 * size ) * 0.070921288047096992;
	sum += texture2D( tex, uv +  -2.0 * size ) * 0.079358891804948081;
	sum += texture2D( tex, uv +  -1.0 * size ) * 0.084895951965930902;
	sum += texture2D( tex, uv +   1.0 * size ) * 0.084895951965930902;
	sum += texture2D( tex, uv +   2.0 * size ) * 0.079358891804948081;
	sum += texture2D( tex, uv +   3.0 * size ) * 0.070921288047096992;
	sum += texture2D( tex, uv +   4.0 * size ) * 0.060594058578763078;
	sum += texture2D( tex, uv +   5.0 * size ) * 0.049494378859311142;
	sum += texture2D( tex, uv +   6.0 * size ) * 0.038650411513543079;
	sum += texture2D( tex, uv +   7.0 * size ) * 0.028855245532226279;
	sum += texture2D( tex, uv +   8.0 * size ) * 0.020595286319257878;
	sum += texture2D( tex, uv +   9.0 * size ) * 0.014053461291849008;
	sum += texture2D( tex, uv +  10.0 * size ) * 0.009167927656011385;

	gl_FragColor	= attenuation * sum;
}