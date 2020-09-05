uniform float agk_time;
varying mediump vec2 uvVarying;
uniform vec2 agk_spritesize; 

vec4 white  = vec4(1.0,1.0,1.0, 1.0);
vec4 yellow = vec4(1.0,1.0,0.0, 1.0);
vec4 orange = vec4(1.0,0.5,0.0, 1.0);
vec4 red    = vec4(1.0,0.0,0.0, 1.0);

void main()
{
    vec4 color = vec4(0.4,0.2,0.2,0.0);
	vec2 uv = uvVarying;
	uv.x *= agk_spritesize.x;
	
	float pi2 		  = 3.1415926535 * 2.;
    float waves 	  = 0.64;
    float speed_x 	  = 50.00;
    float speed_y     = 10.1;
	float light_speed = 1.0;
	
	float height	= 0.4;// * (1.0 + 0.5*sin(20.0*uv.x));
	
	float x_offset = -speed_x * agk_time;
	
	float y = 0.5;
	float h1 = 0.1*height;
	float h2 = 0.2*height * pow(sin(uv.x * 0.15),1.0);
	float h3 = 0.7*height * pow(sin(uv.x * 0.05),1.0);
	
	y += pow(sin((uv.x + x_offset*2.0) * pi2 * waves/1.0),1.0) * h1;
	y += pow(sin((uv.x + x_offset*0.2) * pi2 * waves/2.0),1.0) * h2;
	y += pow(sin((uv.x + x_offset*0.5) * pi2 * waves/4.0),1.0) * h3;
	
	
	float thk = 0.3 * (1.5 + 0.8*sin(uv.x * 0.04));
	float y_max = (uv.y) + thk/2.0;
    float y_min = y_max - thk;
	
	if(y < y_max && y > y_min){
		float ds = abs(uv.y - y);
		float dsn = ds/(thk/2.0);		
		
		if(dsn < 0.25)
			color = mix(white, yellow, dsn/0.25); 			//0.00 - 0.10 (0.10)
		
		else if(dsn < 0.75)
			color = mix(yellow, orange, (dsn - 0.25) / 0.50);		//0.10 - 0.75 (0.65)
		else
			color = mix(orange, red, (dsn - 0.75) / 0.25);	//0.75 - 1.00 (0.25)
		
		//float alph = 0.7 + 0.3*sin(sin(1.0*uv.x) + cos(1.0*uv.y) + sin(1.0*agk_time));
		//color.a = alph;
	
		float light_x = agk_spritesize.x*0.5*(1.0 + sin(agk_time * light_speed));
		
		if(uv.x > light_x && uv.x < (light_x + 1.25))
			color.xyz = vec3(1.0, 1.0, 1.0);
	}
	
	gl_FragColor = color;
}
