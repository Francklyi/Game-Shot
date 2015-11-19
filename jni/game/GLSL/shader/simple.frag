STRINGIFY(

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

varying vec2 texCoord;

uniform sampler2D Tex1;

void main(void) {
	gl_FragColor=texture2D(Tex1, texCoord);

}
);