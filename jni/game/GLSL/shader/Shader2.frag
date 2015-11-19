const char* GLSL::FragmentShader_S =STRINGIFY(

//#ifdef GL_ES
//precision lowp float;//mediump
//#endif

varying vec4 pos;
void main() 
{
//gl_FragDepth = gl_FragCoord.z;
  gl_FragColor=vec4( vec3( gl_FragCoord.z/gl_FragCoord.w* 100.0 ), 1.0 ); // vec4(1.0);//pos;//gl_FragCoord.z;pos;//
  //vec4(1.0,0.0,0.0,1.0);//
  
}

);
