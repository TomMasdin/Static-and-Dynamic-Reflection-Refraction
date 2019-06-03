#pragma once
#include "drawable.h"
#include <vector>
#include "glutils.h"
#include "gl_core_4_3.hpp"

class VBOQuad : public Drawable
{
private:

	unsigned int vaoHandle;
	unsigned int vboHandle;

public:

	VBOQuad();

	void render() const;

	void bindTextureUnits(int programHandle, int textureID); //Binds the current textures to the corresponding texture units

};

