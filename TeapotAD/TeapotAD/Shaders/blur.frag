#version 430

out vec4 out_colour;

in vec2 vblurTextureCoords[11];
in vec2 hblurTextureCoords[11];
in vec2 textureCoords;

uniform sampler2D originalTexture;

void main(void){
	
	//out_colour = vec4(0.0);
	out_colour = texture(originalTexture, textureCoords);

	/*out_colour += texture(originalTexture, hblurTextureCoords[0]) * 0.0093;
    out_colour += texture(originalTexture, hblurTextureCoords[1]) * 0.028002;
    out_colour += texture(originalTexture, hblurTextureCoords[2]) * 0.065984;
    out_colour += texture(originalTexture, hblurTextureCoords[3]) * 0.121703;
    out_colour += texture(originalTexture, hblurTextureCoords[4]) * 0.175713;
    out_colour += texture(originalTexture, hblurTextureCoords[5]) * 0.198596;
    out_colour += texture(originalTexture, hblurTextureCoords[6]) * 0.175713;
    out_colour += texture(originalTexture, hblurTextureCoords[7]) * 0.121703;
    out_colour += texture(originalTexture, hblurTextureCoords[8]) * 0.065984;
    out_colour += texture(originalTexture, hblurTextureCoords[9]) * 0.028002;
    out_colour += texture(originalTexture, hblurTextureCoords[10]) * 0.0093;

	out_colour += texture(originalTexture, vblurTextureCoords[0]) * 0.0093;
    out_colour += texture(originalTexture, vblurTextureCoords[1]) * 0.028002;
    out_colour += texture(originalTexture, vblurTextureCoords[2]) * 0.065984;
    out_colour += texture(originalTexture, vblurTextureCoords[3]) * 0.121703;
    out_colour += texture(originalTexture, vblurTextureCoords[4]) * 0.175713;
    out_colour += texture(originalTexture, vblurTextureCoords[5]) * 0.198596;
    out_colour += texture(originalTexture, vblurTextureCoords[6]) * 0.175713;
    out_colour += texture(originalTexture, vblurTextureCoords[7]) * 0.121703;
    out_colour += texture(originalTexture, vblurTextureCoords[8]) * 0.065984;
    out_colour += texture(originalTexture, vblurTextureCoords[9]) * 0.028002;
    out_colour += texture(originalTexture, vblurTextureCoords[10]) * 0.0093;*/
										   
}