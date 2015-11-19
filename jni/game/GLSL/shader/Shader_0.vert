const char* GLSL::VertexShader =STRINGIFY(

    attribute vec3 VertexPosition;
    attribute vec3 VertexNormal;
    attribute vec2 VertexTexCoord;

    //varying vec3 LightIntensity;

    varying vec2 texCoord;
    varying vec3 normal;

    varying vec3 eyeNorm;

    varying vec4 eyePosition;

    varying vec4 lPosition;

    uniform mat4 MVP;
    uniform mat4 ModelViewMatrix;
    uniform mat3 NormalMatrix;
    //uniform mat4 ProjectionMatrix;

    const float SpecularContribution = 0.5;
    const float DiffuseContribution = 0.5;//1.0 - SpecularContribution;

    void main()
    {

      normal=VertexNormal;
      texCoord= VertexTexCoord;

      eyeNorm  =(ModelViewMatrix * vec4(VertexNormal,0.0)).xyz; //normalize( NormalMatrix * VertexNormal);
      eyePosition = ModelViewMatrix * vec4(VertexPosition,1.0);// = ModelViewMatrix * vec4(VertexPosition,1.0);
      gl_Position =MVP * vec4(VertexPosition,1.0);//
      
      
    }

);

