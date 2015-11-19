STRINGIFY(

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

varying vec2 texCoord;

uniform sampler2D Tex1;

uniform int imgH;

void main(void) {
    float d = 1.0/float(imgH);
    vec4 color = vec4(0.0,0.0,0.0,0.0);
	color += texture2D(Tex1,texCoord+vec2(0.0,-5.0*d)) * 0.1;
	color += texture2D(Tex1,texCoord+vec2(0.0,-4.0*d)) * 0.22;
	color += texture2D(Tex1,texCoord+vec2(0.0,-3.0*d)) * 0.358;
	color += texture2D(Tex1,texCoord+vec2(0.0,-2.0*d)) * 0.563;
	color += texture2D(Tex1,texCoord+vec2(0.0,-1.0*d)) * 0.873;
	color += texture2D(Tex1,texCoord	) * 1.0;
	color += texture2D(Tex1,texCoord+vec2(0.0, 1.0*d)) * 0.873;
	color += texture2D(Tex1,texCoord+vec2(0.0, 2.0*d)) * 0.563;
	color += texture2D(Tex1,texCoord+vec2(0.0, 3.0*d)) * 0.358;
	color += texture2D(Tex1,texCoord+vec2(0.0, 4.0*d)) * 0.22;
	color += texture2D(Tex1,texCoord+vec2(0.0, 5.0*d)) * 0.1;
	color /= 5.0;
    color.a=1.0;
    gl_FragColor = color;

}
);