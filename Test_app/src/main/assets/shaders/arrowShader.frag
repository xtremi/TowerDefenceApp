uniform sampler2D texture0;
varying mediump vec2 uvVarying;
varying mediump vec4 colorVarying;
uniform float agk_time;

float M = 0.75;

void main()
{
	vec2 uv = uvVarying.xy;

	//This modify the time to a value between 0.0 and 1.0, 
	//With a sequence such as 0.0, 0.1, 0.2, ... 0.99, 0.0
	//To give a sort of linear stepping:
	float modTime = M*agk_time - floor(M*agk_time);
	
	//R makes the time non-linear:
	float R = modTime*modTime;
	
	float colr = 0.0;
	
	//The red color at uv.x before uv.x = R, increases linearly
	//The red color before uv.x = 1.0, decreases linearly:
	if( uv.x < R){
		colr = uv.x/R;
		}
	else{
		colr = (1.0/(R-1.0))*(uv.x - 1.0);
	}
	
	//Just for more intese in the center:
	colr = colr*colr;
	
	float colb = 0.0;
	float colg = 0.0;
	
	//Transparancy follows the red color:
	float alpha = colr*colr;
	
	//Setting transparancy lower (higher alpha) near end of uv.x	
	alpha = alpha * (0.2 + 0.8*uv.x);
	
	//Setting transparancy higher (lower alpha) near uv.y = 0.0 and uv.y = 1.0
	alpha = alpha * sin(3.14*uv.y);
	
	/*if (uv.x > 0.8){
		alpha /= 2.0;
	}*/
	
	gl_FragColor = vec4(colr, colr, colr/2.0, alpha);
	
	
}