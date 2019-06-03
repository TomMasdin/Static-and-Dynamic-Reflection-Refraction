#version 430

layout (location = 0) in vec2 position;

out vec2 hblurTextureCoords[11];
out vec2 vblurTextureCoords[11];

out vec2 textureCoords;

uniform float targetWidth;
uniform float targetHeight;

void main(void){

	gl_Position = vec4(position, 0.0, 1.0);
	textureCoords = position * 0.5 + 0.5;
	vec2 centerTexCoords = position * 0.5 + 0.5;
	float hpixelSize = 1.0 / targetWidth;
	float vpixelSize = 1.0 / targetHeight;


	for(int i = -5; i<=5; i++)
	{
		hblurTextureCoords[i+5] = centerTexCoords + vec2(hpixelSize * i, 0.0);
		vblurTextureCoords[i+5] = centerTexCoords + vec2(0.0, vpixelSize * i);
	}
	
}