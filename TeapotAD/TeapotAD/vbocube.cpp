#include "vbocube.h"
#include <iostream>



VBOCube::VBOCube(float tmp_skyBoxSize)
{
	this->cubeSize = tmp_skyBoxSize;

	float points[] = {
		-cubeSize,  cubeSize, -cubeSize,
		-cubeSize, -cubeSize, -cubeSize,
		cubeSize, -cubeSize, -cubeSize,
		cubeSize, -cubeSize, -cubeSize,
		cubeSize,  cubeSize, -cubeSize,
		-cubeSize,  cubeSize, -cubeSize,

		-cubeSize, -cubeSize,  cubeSize,
		-cubeSize, -cubeSize, -cubeSize,
		-cubeSize,  cubeSize, -cubeSize,
		-cubeSize,  cubeSize, -cubeSize,
		-cubeSize,  cubeSize,  cubeSize,
		-cubeSize, -cubeSize,  cubeSize,

		cubeSize, -cubeSize, -cubeSize,
		cubeSize, -cubeSize,  cubeSize,
		cubeSize,  cubeSize,  cubeSize,
		cubeSize,  cubeSize,  cubeSize,
		cubeSize,  cubeSize, -cubeSize,
		cubeSize, -cubeSize, -cubeSize,

		-cubeSize, -cubeSize,  cubeSize,
		-cubeSize, cubeSize,   cubeSize,
		cubeSize,  cubeSize,   cubeSize,
		cubeSize,  cubeSize,   cubeSize,
		cubeSize, -cubeSize,  cubeSize,
		-cubeSize, -cubeSize,  cubeSize,

		-cubeSize,  cubeSize, -cubeSize,
		cubeSize,  cubeSize, -cubeSize,
		cubeSize,  cubeSize,  cubeSize,
		cubeSize, cubeSize,  cubeSize,
		-cubeSize,  cubeSize,  cubeSize,
		-cubeSize,  cubeSize, -cubeSize,

		-cubeSize, -cubeSize, -cubeSize,
		-cubeSize, -cubeSize,  cubeSize,
		cubeSize, -cubeSize, -cubeSize,
		cubeSize, -cubeSize, -cubeSize,
		-cubeSize, -cubeSize,  cubeSize,
		cubeSize, -cubeSize,  cubeSize
	};

	gl::GenVertexArrays(1, &vaoHandle);
	gl::GenBuffers(1, &vboHandle);
	gl::BindVertexArray(vaoHandle);
	gl::BindBuffer(gl::ARRAY_BUFFER, vboHandle);
	gl::BufferData(gl::ARRAY_BUFFER, 108 * sizeof(gl::FLOAT), &points, gl::STATIC_DRAW);
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 3 * sizeof(gl::FLOAT), (GLvoid*)0);
	
	//Bottom negy
	m_skyboxTextureList1.push_back("Textures/skybox/ny.png");
	m_skyboxTextureList2.push_back("Textures/skybox/ny_1.png");
	m_skyBoxOrient.push_back(gl::TEXTURE_CUBE_MAP_NEGATIVE_Y);
	//Front negz
	m_skyboxTextureList1.push_back("Textures/skybox/nz.png");
	m_skyboxTextureList2.push_back("Textures/skybox/nz_1.png");
	m_skyBoxOrient.push_back(gl::TEXTURE_CUBE_MAP_NEGATIVE_Z);
	//Back posz
	m_skyboxTextureList1.push_back("Textures/skybox/pz.png");
	m_skyboxTextureList2.push_back("Textures/skybox/pz_1.png");
	m_skyBoxOrient.push_back(gl::TEXTURE_CUBE_MAP_POSITIVE_Z);
	//Right posx
	m_skyboxTextureList1.push_back("Textures/skybox/px.png");
	m_skyboxTextureList2.push_back("Textures/skybox/px_1.png");
	m_skyBoxOrient.push_back(gl::TEXTURE_CUBE_MAP_POSITIVE_X);
	//Top posy
	m_skyboxTextureList1.push_back("Textures/skybox/py.png");
	m_skyboxTextureList2.push_back("Textures/skybox/py_1.png");
	m_skyBoxOrient.push_back(gl::TEXTURE_CUBE_MAP_POSITIVE_Y);
	//Left negx
	m_skyboxTextureList1.push_back("Textures/skybox/nx.png");
	m_skyboxTextureList2.push_back("Textures/skybox/nx_1.png");
	m_skyBoxOrient.push_back(gl::TEXTURE_CUBE_MAP_NEGATIVE_X);

	createCubeMapTexture(m_skyboxTextureList1, m_textureID, gl::RGBA);
	createCubeMapTexture(m_skyboxTextureList2, m_textureID2, gl::RGBA);
}



void VBOCube::bindTextureUnits(int programHandle)
{
	
	GLint location = gl::GetUniformLocation(programHandle, "skybox");
	gl::Uniform1i(location, 0);
	location = gl::GetUniformLocation(programHandle, "skybox2");
	gl::Uniform1i(location, 1);

	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, m_textureSwitch1);

	gl::ActiveTexture(gl::TEXTURE1);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, m_textureSwitch2);

	
}

void VBOCube::createCubeMapTexture(std::vector<std::string> textureList, unsigned int &textureID, unsigned int colourFormat)
{
	gl::GenTextures(1, &textureID);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, textureID);


	for (int i = 0; i < textureList.size(); i++)
	{
		Bitmap bmp = Bitmap::bitmapFromFile(textureList[i]);

		gl::TexImage2D(m_skyBoxOrient[i], 0, colourFormat, bmp.width(), bmp.height(),
			0, colourFormat, gl::UNSIGNED_BYTE, bmp.pixelBuffer());
	}

	gl::TexParameterf(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::TexParameterf(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::TexParameterf(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
	gl::TexParameterf(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
	gl::TexParameterf(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE);
}

void VBOCube::render() const
{

	gl::BindVertexArray(vaoHandle);
	gl::DrawArrays(gl::TRIANGLES, 0, 36);
	gl::BindVertexArray(0);
}

void VBOCube::update(float t)
{
	t = t * 1000;
	t = (int)t % 24000;


	if (t >= 0 && t < 5000)
	{
		blendFactor = (t - 0) / (5000 - 0);
		m_textureSwitch1 = m_textureID2;
		m_textureSwitch2 = m_textureID2;
	}
	else if(t >=5000 && t < 8000)
	{
		blendFactor = (t - 5000) / (8000 - 5000);
		m_textureSwitch1 = m_textureID2;
		m_textureSwitch2 = m_textureID;
	}
	else if (t >= 8000 && t < 21000)
	{
		blendFactor = (t - 8000) / (21000 - 8000);
		m_textureSwitch1 = m_textureID;
		m_textureSwitch2 = m_textureID;
	}
	else
	{
		blendFactor = (t - 21000) / (24000 - 21000);
		m_textureSwitch1 = m_textureID;
		m_textureSwitch2 = m_textureID2;
	}

	

}


int VBOCube::getTextureID()
{
	return m_textureID2;
}

float VBOCube::getBlendFactor()
{
	return blendFactor;
}
