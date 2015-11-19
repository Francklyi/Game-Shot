STRINGIFY(

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

varying vec2 texCoord;

uniform sampler2D Tex1;

void main(void) {
	vec4 rgb = texture2D ( Tex1 , texCoord ) ;
	
	// plus 0.0001 avoid the lum is too small.
	float lum = 0.27 * rgb.x + 0.67 * rgb.y + 0.06 * rgb.z + 0.0001 ;
	//float numLog = log ( lum ) ;
    gl_FragColor =vec4(vec3(lum),1.0); //texture2D(Tex1,texCoord);//vec4(lum,lum,lum,1.0);
}
);