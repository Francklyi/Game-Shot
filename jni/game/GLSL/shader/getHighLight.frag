STRINGIFY(

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform float hasTexture;

varying vec2 texCoord;

uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform float AveLum;
uniform int imgH;
uniform int imgW;

void main(void) {
	//float dx = 1.0/float(imgW);
	//float dy = 1.0/float(imgH);
	//对Tex1进行采样
	vec4 color = texture2D(Tex1,texCoord);
	//color += texture2D(Tex1,texCoord+vec2(dx*3.0,0.0));

	//color += texture2D(Tex1,texCoord+vec2(0.0,dy));
	//color += texture2D(Tex1,texCoord+vec2(dx*3.0,dy));

	//color += texture2D(Tex1,texCoord+vec2(0.0,dy*2.0));
	//color += texture2D(Tex1,texCoord+vec2(dx*3.0,dy*2.0));

	//color += texture2D(Tex1,texCoord+vec2(0.0,dy*3.0));
	//color += texture2D(Tex1,texCoord+vec2(dx*3.0,dy*3.0));
	//color /= 8.0;
	//计算该像素在Tone Mapping之后的亮度值，如果依然很大，则该像素将产生光晕
	
	float ave1 = texture2D(Tex2,vec2(0.0,0.0)).x+texture2D(Tex2,vec2(0.0,0.25)).x+texture2D(Tex2,vec2(0.0,0.5)).x+texture2D(Tex2,vec2(0.0,0.75)).x
	+texture2D(Tex2,vec2(0.25,0.0)).x+texture2D(Tex2,vec2(0.25,0.25)).x+texture2D(Tex2,vec2(0.25,0.5)).x+texture2D(Tex2,vec2(0.25,0.75)).x
	+texture2D(Tex2,vec2(0.5,0.0)).x+texture2D(Tex2,vec2(0.5,0.25)).x+texture2D(Tex2,vec2(0.5,0.5)).x+texture2D(Tex2,vec2(0.5,0.75)).x
	+texture2D(Tex2,vec2(0.75,0.0)).x+texture2D(Tex2,vec2(0.75,0.25)).x+texture2D(Tex2,vec2(0.75,0.5)).x+texture2D(Tex2,vec2(0.75,0.75)).x;
	ave1 /=16.0;
	
	vec4 cout = vec4(0.0,0.0,0.0,1.0);
	float lum = color.x * 0.27 + color.y *0.67 + color.z * 0.06;
	vec4 p = color*(lum/ave1);//AveLum
	p /= vec4(vec4(1.0,1.0,1.0,0.0)+p);
	float luml = (p.x+p.y+p.z)/3.0;
	if (luml > 0.55)
	{
		cout = p;
	}
	gl_FragColor = cout;


}
);