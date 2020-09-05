uniform sampler2D texture0;
varying mediump vec2 uvVarying;
varying mediump vec4 colorVarying;
uniform float agk_time;
uniform float scoreP;

void main()
{
    vec2 uv = uvVarying;
	
	gl_FragColor = texture2D(texture0, uvVarying) * colorVarying;
	
	//gl_FragColor.r = 0.85 + 0.1*(sin(agk_time*1.0) + 1.0)/2.0;
	
	/*gl_FragColor.r = 1.0 - scoreP;
	gl_FragColor.g = 0.80 + 0.1*(cos(agk_time*1.0) + 1.0)/2.0;
	gl_FragColor.b = 0.85 + 0.1*(cos(agk_time*5.0) + 1.0)/2.0;*/
	
	gl_FragColor.r = 0.4;
	gl_FragColor.g = 0.6;
	gl_FragColor.b = 0.95;
	
	float ylim = 0.7 + 0.1*sin(uv.x*10.0 + agk_time);
	ylim += 0.05*sin(uv.x*50.0 + 2.0*agk_time);
	if(uv.y > ylim){
		
		
		float d = abs(ylim - uv.y);
		//gl_FragColor.r = mix(1.0, gl_FragColor.r, 1.0 - 2.0*d / ylim);
		
	}
	
}