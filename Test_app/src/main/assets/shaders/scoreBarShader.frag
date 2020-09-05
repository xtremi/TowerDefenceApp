uniform sampler2D texture0;
varying mediump vec2 uvVarying;
varying mediump vec4 colorVarying;
uniform float agk_time;
uniform float scoreP;

float M = 0.75;

void main()
{
	vec2 uv = uvVarying.xy;

	//float y = 1.0 - uv.y;
	float x = uv.x;
	
	vec3 col = vec3(0.0, 0.0, 0.0);
	float alpha = 0.0;
	
	if(x < scoreP)
	{
		col = vec3((1.0 - scoreP), scoreP, 0.0);
		alpha = 0.2 + 0.8*(scoreP - x);
	}
	
	

	gl_FragColor = vec4(col, alpha);	
}