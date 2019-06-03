#version 430 core

out vec4 FragColor;



in vec3 normal; //Transformed normal
in vec3 lightPos; //Light position in eye coords
in vec3 Position;


uniform vec3 cameraPos;

uniform samplerCube skybox;
uniform samplerCube skybox2;

uniform float blendFactor;

void main()
{
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(normal));
	vec4 texture1 = texture(skybox, R);
    vec4 texture2 = texture(skybox2, R);
    vec4 finalTexture = mix(texture1, texture2, blendFactor);
   
   FragColor = vec4(finalTexture.rgb, 1.0);
 }