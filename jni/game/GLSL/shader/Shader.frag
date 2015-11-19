STRINGIFY(

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform float hasTexture;

varying vec2 texCoord;

uniform sampler2D Tex1;

varying vec3 LightIntensity;

void main(void) {
	vec4 tex= texture2D(Tex1, texCoord);
	gl_FragColor=vec4(LightIntensity+ tex.rgb ,tex.a);

if(hasTexture!=1.0) 
	gl_FragColor=vec4(LightIntensity ,1.0);

}
)