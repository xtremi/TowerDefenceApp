uniform sampler2D texture0;
varying mediump vec2 uvVarying;
varying mediump vec4 colorVarying;
uniform float agk_time;

void main()
{
    vec2 uv = uvVarying;	
	gl_FragColor = texture2D(texture0, uvVarying);
	
	//gl_FragColor.r = 0.25;
	gl_FragColor.r *= colorVarying.r;
	gl_FragColor.g *= colorVarying.g;
	gl_FragColor.b *= colorVarying.b;
		
	gl_FragColor.a = clamp(gl_FragColor.a*2.5, 0.0, 1.0);
	
	
}