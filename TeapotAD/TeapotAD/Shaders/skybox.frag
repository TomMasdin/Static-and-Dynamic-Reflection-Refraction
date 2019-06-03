#version 430

in vec3 texture_coordinates;
out vec4 color;
uniform samplerCube skybox;
uniform samplerCube skybox2;
uniform float blendFactor;


void main() 
{
	float factor = blendFactor;
	vec4 texture1 = texture(skybox, texture_coordinates);
	vec4 texture2 = texture(skybox2, texture_coordinates);
	vec4 finalColour = mix(texture1, texture2, blendFactor);


	color = finalColour;
 
}