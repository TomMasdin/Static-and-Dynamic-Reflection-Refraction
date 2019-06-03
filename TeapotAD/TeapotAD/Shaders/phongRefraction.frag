#version 430

in vec3 vertPos;
in vec3 N;
in vec3 lightPos;


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
uniform samplerCube skybox2;

uniform float blendFactor;

layout( location = 0 ) out vec4 FragColour;

void main() {

   vec3 normal = normalize(N);
   //Calculate the light vectors
   vec3 surfaceToLight = normalize(lightPos - vertPos);  
   vec3 viewDir = normalize(cameraPos - vertPos);
   vec3 surfaceToView = normalize(-vertPos);
   vec3 halfwayView = normalize(surfaceToView + surfaceToLight);
   vec3 reflectDir = reflect(-surfaceToLight, normal);

   //Ambient
   vec3 ambient = La * Ka;
    
   //calculate Diffuse Light Intensity making sure it is not negative 
   //and is clamped 0 to 1  
   
   float dot_prod = dot(normal, surfaceToLight);
   dot_prod = max(0.0, dot_prod);
   vec3 Id = Ld * dot_prod;
   Id = clamp(Id, 0.0, 1.0);     
   vec3 diffuse = Kd * Id;

   //Specular
  
   float dotProdSpecular = max(dot(viewDir, reflectDir), 0.0);
   float specularFactor = specularIntensity * pow(dotProdSpecular, specularExponent);

   vec3 specular = Ls * Ks * specularFactor;

	float ratio = 1.00 / 1.52;
	vec3 I = normalize(vertPos - cameraPos);
	vec3 R = refract(I, normalize(normal), ratio);

   //Multiply the Reflectivity by the Diffuse intensity
   vec3 color = ambient + diffuse + specular;

   vec4 texture1 = texture(skybox, R);
   vec4 texture2 = texture(skybox2, R);
   vec4 finalTexture = mix(texture1, texture2, blendFactor);
   
   FragColour = vec4(mix(color, finalTexture.rgb, 0.5), 1.0);

}
