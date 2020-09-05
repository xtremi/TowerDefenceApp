#define pi 3.1415926535898
float xRandom(float x){
	return mod(x*7241.6465+2130.465521, 64.984131);
	//return fract(sin(dot(xy ,vec2(12.9898,78.233))) * 43758.5453);

}
float hash2(in vec2 p)
{
	return fract(dot(sin(p.x * 591.32 + p.y * 154.077), cos(p.x * 391.32 + p.y * 49.077)));
}
float hash( vec2 p) { 
    vec2 pos = fract(p / 128.) * 128. + vec2(-64.340622, -72.465622);  
    return fract(dot(pos.xyx * pos.xyy, vec3(20.390625, 60.703125, 2.4281209)));  
}
float noise( float y, float t)
{
   	vec2 fl = vec2(floor(y), floor(t));
	vec2 fr = vec2(fract(y), fract(t));
	float a = mix(hash(fl + vec2(0.0,0.0)), hash(fl + vec2(1.0,0.0)),fr.x);
	float b = mix(hash(fl + vec2(0.0,1.0)), hash(fl + vec2(1.0,1.0)),fr.x);
	return mix(a,b,fr.y);
}
float noise2( float y, float t)
{
   	vec2 fl = vec2(floor(y), floor(t));
	vec2 fr = vec2(fract(y), fract(t));
	float a = mix(hash2(fl + vec2(0.0,0.0)), hash2(fl + vec2(1.0,0.0)),fr.x);
	float b = mix(hash2(fl + vec2(0.0,1.0)), hash2(fl + vec2(1.0,1.0)),fr.x);
	return mix(a,b,fr.y);
}
float line(vec2 uv,float width, float center)
{    
    float b		=	(1.- smoothstep(.0, width, abs(uv.y-center)))*1.;
    float b2	=	(1.- smoothstep(.0, 5.*width, abs(uv.y-center)))*.8;
    return b+b2;
}

uniform float agk_time;
varying mediump vec2 uvVarying;
uniform vec2 agk_spritesize; 

void main()
{
	vec2 uv = uvVarying;
	uv.x *= agk_spritesize.x;    
    uv -=.5;
	//uv.y	*=	iResolution.y/iResolution.x;
	
	float Line_Smooth = 1.0;
    
    
    float Factor_T    =floor(agk_time*15.0);
    float Factor_X    =xRandom(uv.x*1.0e-5 + 1.0);
    float Amplitude1  =0.5000 * (-1.0 + 2.0*noise(Factor_X/1.0, Factor_T))
         			  +0.2500 * (-1.0 + 2.0*noise(Factor_X/2.0, Factor_T))
          			  +0.1250 * (-1.0 + 2.0*noise(Factor_X/4.0, Factor_T))
    	  			  +0.0625 * (-1.0 + 2.0*noise(Factor_X/8.0, Factor_T));
	Factor_X    =xRandom(uv.x*1.3e-5 + 1.0);
    float Amplitude2  =0.5000 * (-1.0 + 2.0*noise2(Factor_X/1.0, Factor_T))
         			  +0.2500 * (-1.0 + 2.0*noise2(Factor_X/2.0, Factor_T))
          			  +0.1250 * (-1.0 + 2.0*noise2(Factor_X/4.0, Factor_T))
    	  			  +0.0625 * (-1.0 + 2.0*noise2(Factor_X/8.0, Factor_T));
    
	Amplitude1*=2.0;
	Amplitude2*=2.0;
	
	
    vec4 Light_Color1=vec4(.3, .45, 1., 1.);	
    vec4 Light_Color2=vec4(.752, .164, .842, 1.);
    
    
    float Light_Track1  = line(vec2(uv.x,uv.y+(Amplitude1-.5)*.12*Line_Smooth), .06, .0);
   	float Light_Track2  = line(vec2(uv.x,uv.y+(Amplitude2-.5)*.16*Line_Smooth), .06, .0);
   
   
	vec4 line1 =  vec4(Light_Track1)*Light_Color1;
    vec4 line2 =  vec4(Light_Track2)*Light_Color2;
    
	//vec4 col = vec4(0.2, 0.2, 0.2, 0.4);
    
    
    gl_FragColor =(line1+line2);
}