STRINGIFY(

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

    attribute vec3 VertexPosition;
    attribute vec3 VertexNormal;
    attribute vec3 VertexTexCoord;
    
    uniform mat4 MVP[30];
    
    uniform mat4 ModelViewMatrix[30];

	//uniform mat3 NormalMatrix[20];

	uniform float funcType;
	
	uniform float funcSpec;


uniform vec4 LPosition; // =vec4(15.0, 30.0, 3.5, 1.0)Light position in eye coords.
vec3 La=vec3(0.5, 0.5, 0.5); // Ambient light intensity=vec3(0.5,0.5,0.3)
vec3 Ld=vec3(0.65, 0.65, 0.65); // Diffuse light intensity= vec3(0.4,0.0,0.0)
vec3 Ls=vec3(2.0, 2.0, 2.0); // Specular light intensity

uniform vec3 Ka ; // Ambient reflectivity
uniform vec3 Kd ; // Diffuse reflectivity
uniform vec3 Ks ; // Specular reflectivity
uniform float Shininess; // Specular shininess factor

uniform float hasTexture;

varying vec3 LightIntensity;
varying vec2 texCoord;

    void main()
    {
      texCoord= VertexTexCoord.xy;
	  LightIntensity=Kd*0.5+ Ka;
	  
	  if(funcType==1.0)// phong
	  {
	  	vec3 eyeNorm  =normalize( (ModelViewMatrix[int(VertexTexCoord.z)] * vec4(VertexNormal,0.0)).xyz);
	  	 //normalize( NormalMatrix[int(VertexTexCoord.z)]* VertexNormal);
      	vec4 eyePosition = ModelViewMatrix[int(VertexTexCoord.z)] * vec4(VertexPosition,1.0);
      	
		vec3 s = normalize(vec3(LPosition - eyePosition));
		vec3 v = normalize(-eyePosition.xyz);
		vec3 r = reflect( -s, eyeNorm );
		vec3 ambient=Ka*La;// ;+ + 0.3  
		float sDotN = max( dot(s,eyeNorm), 0.0 );
		vec3 diffuse =Kd *Ld * sDotN; //* attr1 +
 		if(hasTexture==0.0) diffuse+=Kd*0.5;
		vec3 spec = vec3(0.0);
		if( sDotN > 0.0 )
		spec =Ks * Ls * pow( max( dot(r,v), 0.0 ), 8.0 );// *attr2 
      	
	  	LightIntensity =spec+ diffuse+ ambient;
	  	if(hasTexture==1.0) LightIntensity =spec;
	  }
	  

      gl_Position =MVP[int(VertexTexCoord.z)] * vec4(VertexPosition,1.0);//
      
    }

);

