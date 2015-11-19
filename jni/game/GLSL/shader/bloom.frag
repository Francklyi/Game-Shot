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

void main(void) {

	gl_FragColor=texture2D(Tex1, texCoord)+texture2D(Tex2, texCoord);


}
);