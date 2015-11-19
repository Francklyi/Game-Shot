const char* GLSL::FragmentShader=STRINGIFY(

//layout(location = 0) out float fragmentdepth;

uniform vec4 LPosition; // Light position in eye coords.
vec3 La=vec3(0.2,0.2,0.2); // Ambient light intensity=vec3(0.5,0.5,0.3)
vec3 Ld=vec3(0.5,0.5,0.5); // Diffuse light intensity= vec3(0.4,0.0,0.0)
vec3 Ls=vec3(1.0,1.0,1.0); // Specular light intensity

uniform vec3 Ka ; // Ambient reflectivity
uniform vec3 Kd ; // Diffuse reflectivity
uniform vec3 Ks ; // Specular reflectivity
uniform float Shininess; // Specular shininess factor

//varying vec3 LightIntensity;
varying vec3 eyeNorm;
varying vec4 eyePosition;
varying vec2 texCoord;
varying vec3 normal;

varying float isShadow;

uniform sampler2D Tex1;

vec3 phongModel(in vec4 position, in vec3 norm )
{
vec3 s = normalize(vec3(LPosition - position));
float distance = length(LPosition - position);
float attr1=1.0/(1.0 + ( 0.001 * distance * distance ));
vec3 v = normalize(-position.xyz);
vec3 r = reflect( -s, norm );
vec3 ambient=Ka* La;// ;+ + 0.3
float sDotN = max( dot(s,norm), 0.0 );
vec3 diffuse =Kd * Ld * sDotN; //* attr1 +
float attr2=1.0/(0.1 + ( 0.1* distance *distance));
//diffuse = diffuse ;
vec3 spec = vec3(0.0);
if( sDotN > 0.0 )
spec =Ks * vec3(0.7,0.6,0.5) * pow( max( dot(r,v), 0.0 ), Shininess );// *attr2
return spec + ambient + diffuse;//
}

void main() {

  if(isShadow == 1.0) gl_FragColor= vec4( 0.2, 0.2, 0.2, 1.0);
  else
  {
  //vec3 LightIntensity1=normalize(LightIntensity);
  vec3 LightIntensity = phongModel( eyePosition, eyeNorm );
  if(true)
  gl_FragColor=mix( vec4(LightIntensity,1.0) , texture2D(Tex1, texCoord) , 0.5 );
  else 
  gl_FragColor= vec4(LightIntensity,1.0) + texture2D(Tex1, texCoord);
  //vec4(0.4,0.4,0.4,0.1)+vec4(normal,1.0)+color;//texture2D(Tex1, texCoord);//*
  
  //fragmentdepth = gl_FragColor.z;
  
  }
  
}

);
