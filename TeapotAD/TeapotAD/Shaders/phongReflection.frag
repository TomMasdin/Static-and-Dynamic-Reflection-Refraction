#version 430

in vec3 vertPos;
in vec3 N;
in vec3 lightPos;
in vec3 normal;
in vec3 Position;


uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ld;            // Diffuse light intensity
uniform vec3 Ka;			// Ambient Reflectivity
uniform vec3 La;			// Ambient light intensity
uniform vec3 Ks;			// Specular reflectivity
uniform vec3 Ls;			// Specular light intensity

uniform vec3 cameraPos;		//View Matrix

uniform float specularExponent;
uniform float specularIntensity;

uniform samplerCube skybox;

layout( location = 0 ) out vec4 FragColour;

void main() {

   vec3 normalN = normalize(N);
   //Calculate the light vectors
   vec3 surfaceToLight = normalize(lightPos - vertPos);  
   vec3 viewDir = normalize(cameraPos - vertPos);
   vec3 surfaceToView = normalize(-vertPos);
   vec3 halfwayView = normalize(surfaceToView + surfaceToLight);
   vec3 reflectDir = reflect(-surfaceToLight, normalN);

   //Ambient
   vec3 ambient = La * Ka;
    
   //calculate Diffuse Light Intensity making sure it is not negative 
   //and is clamped 0 to 1  
   
   float dot_prod = dot(normalN, surfaceToLight);
   dot_prod = max(0.0, dot_prod);
   vec3 Id = Ld * dot_prod;
   Id = clamp(Id, 0.0, 1.0);     
   vec3 diffuse = Kd * Id;

   //Specular
  
   float dotProdSpecular = max(dot(viewDir, reflectDir), 0.0);
   float specularFactor = specularIntensity * pow(dotProdSpecular, specularExponent);

   vec3 specular = Ls * Ks * specularFactor;

	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(normal));

   //Multiply the Reflectivity by the Diffuse intensity
   vec3 color = ambient + diffuse + specular;
   vec4 texture1 = texture(skybox, R);
  
   FragColour = vec4(mix(color, texture1.rgb, 0.5), 1.0);
   

}
