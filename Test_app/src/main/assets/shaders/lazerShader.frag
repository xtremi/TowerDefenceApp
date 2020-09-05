uniform sampler2D texture0;
varying mediump vec2 uvVarying;
varying mediump vec4 colorVarying;
uniform float agk_time;

float M = 3.50;

void main()
{
	vec2 uv = uvVarying.xy;


	float modTime = sin(M*agk_time);
	
	
	//R makes the time non-linear:
	float R = modTime*modTime;
	
	//The red color at uv.x before uv.x = R, increases linearly
	//The red color before uv.x = 1.0, decreases linearly:
	float colr 	= 0.0;
	float colg  = 0.0;
	float colb	= 0.0;
	float alpha = 0.0;
	
	//distance from R and center:
	float dr = pow(abs(uv.x - R)/0.5, 2.0);	
	float dy = pow(abs(uv.y - 0.5)/0.5, 1.0);
	
	dy += 0.05*sin(50.0*R) + 0.02*cos(500.0*R);
	
	//Higher color when dr is small:
	colr = clamp(1.0 - dr, 0.0, 1.0);
	
	//Higher blue and green when far from center:
	colg = clamp(dy, 0.0, 1.0);
	colb = clamp(dy, 0.0, 1.0);	
	colg = pow(colg, 1.0);
	colb = pow(colb, 1.0);
	
	//Just for more intese in the center:
	colr = colr*colr;
		
	//Transparancy follows the red color:
	alpha = colr*colr;
	
	//Transparancy follows invert of colb & colg:
	alpha = alpha*(1.0 - 0.5*(colb + colg));
	
	//Final color:
	gl_FragColor = vec4(colr, colg, colb, alpha);
}