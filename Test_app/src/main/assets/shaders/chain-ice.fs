uniform float agk_time;
varying mediump vec2 uvVarying;
uniform vec2 agk_spritesize; 

void main()
{
    vec4 color = vec4(1.0,1.0,1.0,0.0);
	vec2 uv = uvVarying;
	uv.x *= agk_spritesize.x;
	
	float pi2 		= 3.1415926535 * 2.;
    float waves 	= 0.08;
    float speed_x 	= 100.00;
    float speed_y   = 10.1;
	float light_speed = 1.0;
	
	float height	= 0.20;// * (1.0 + 0.5*sin(20.0*uv.x));
	
	float x_offset = -speed_x * agk_time;
	
	float y = sin((uv.x + x_offset) * pi2 * waves) * height; //normal sine moving in x dir
	
	//y*= (1.0 + 0.25*sin(agk_time * speed_y));				 //time varying amplitude of sine curve 
	y*= (1.0 + 0.8*sin(uv.x * 0.1));					 	 //x-axis varying amplitude along x axis
	
	float y_max = uv.y  - 0.5;
    float y_min = y_max - 0.2;
	
	if(y < y_max && y > y_min){
		color = vec4(0.0,1.0,1.0,1.0);
	
		float light_x = agk_spritesize.x*0.5*(1.0 + sin(agk_time * light_speed));
		
		if(uv.x > light_x && uv.x < (light_x + 5.))
			color.xyz = vec3(1.0, 1.0, 1.0);
	}
	
	gl_FragColor = color;
}
