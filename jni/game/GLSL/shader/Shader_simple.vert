const char* GLSL::VertexShader_simple =STRINGIFY(
//precision mediump float;
attribute vec3 VertexPosition;
    
uniform mat4 MVP;
varying vec4 pos;

void main()
{
	gl_Position =MVP * vec4(VertexPosition,1.0);//
	pos=gl_Position;
}

);