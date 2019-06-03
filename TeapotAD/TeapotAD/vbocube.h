#pragma once
#include "drawable.h"
#include <vector>
#include "glutils.h"
#include "gl_core_4_3.hpp"
#include "Bitmap.h"



class VBOCube : public Drawable
{
private:

	unsigned int vaoHandle;
	unsigned int vboHandle;

	//Texture holders
	unsigned int m_textureID;
	unsigned int m_textureID2;
	unsigned int m_textureSwitch1;
	unsigned int m_textureSwitch2;

	//Size of the Cube
	float cubeSize;

	//Mix factor for blending the two textures
	float blendFactor;


	//vector texture and orientation
	std::vector<std::string> m_skyboxTextureList1;
	std::vector<std::string> m_skyboxTextureList2;
	std::vector<unsigned int> m_skyBoxOrient;


public:

	VBOCube(float);

	void render() const; // Renders the skybox to the scene
	void update(float t); // Update to switch skybox images based on t (delta time)
	void bindTextureUnits(int programHandle); //Binds the current textures to the corresponding texture units for shaders
	void createCubeMapTexture(std::vector<std::string> textureList, unsigned int &textureID, unsigned int colourFormat); //Generate a cubemap texture using images, texture ID and colourFormat

	// Getters
	int getTextureID();
	float getBlendFactor();
};



