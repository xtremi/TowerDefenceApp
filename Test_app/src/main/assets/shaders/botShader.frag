uniform sampler2D texture0;
varying mediump vec2 uvVarying;
varying mediump vec4 colorVarying;

uniform vec2 agk_spritepos;
uniform vec2 agk_spritesize;
uniform float agk_time;

uniform vec2 vel;

uniform vec2 points[4];
uniform vec2 directions[4];
uniform vec2 dir_normals[4];
uniform vec2 point_normals[4];

const float maxvel = 50.0;

void main()
{
    vec2 uv = uvVarying;	
	gl_FragColor = texture2D(texture0, uvVarying);
	

	//THE BALL:
	float vmag = length(vel);
	float dist_from_center = length(uv - 0.5);

	gl_FragColor.b = clamp(1.0 - vmag/maxvel, 0.0, 1.0);
	gl_FragColor.r = clamp(vmag/maxvel, 0.0, 1.0);
	
	//gl_FragColor.a = 0.0;// - pow(dist_from_center*beta, 1.0);	
	//if(dist_from_center < 0.05){
		//gl_FragColor.a = 1.0;// - pow(dist_from_center*beta, 1.0);	
		// - dist_from_center/(sqrt(2.0));	
	//}	
	//vec2 nvel = vel / maxvel;	
	//float beta = clamp(vmag/maxvel, 0.4, 1.0);
	
	//THE TAIL:
	vec2 uv_w;	
	uv_w.x = agk_spritepos.x + agk_spritesize.x*uv.x;
	uv_w.y = agk_spritepos.y + agk_spritesize.y*uv.y;
	
	float d1 = length(uv_w - points[0]);
	float d2 = length(uv_w - points[1]);
	float d3 = length(uv_w - points[2]);
	float d4 = length(uv_w - points[3]);
		
	float D = 0.1;
	if (d1 < D || d2 < D || d3 < D || d4 < D)
	{
		gl_FragColor.a = 1.0;
		gl_FragColor.r = 1.0;
	}
		
	
	
}