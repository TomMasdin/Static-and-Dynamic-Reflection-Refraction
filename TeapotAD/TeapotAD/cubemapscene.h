#ifndef SCENEDIFFUSE_H
#define SCENEDIFFUSE_H

#include "gl_core_4_3.hpp"

#include <glfw3.h>
#include "scene.h"
#include "glslprogram.h"

#include "vboteapot.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vboquad.h"
#include "cubemapcamera.h"

#include <glm.hpp>

class Fbo;

using glm::mat4;

enum ShaderType
{
	Phong,
	Reflection,
	Refraction,
	END
};

namespace imat3111
{

class CubeMapScene : public Scene
{
private:
	/*
		Shader programs used in the scene
	*/
	GLSLProgram phongProgram;
	GLSLProgram reflectionProgram;
	GLSLProgram refractionProgram;
	GLSLProgram skyBoxProg;
	GLSLProgram planeProg;

	// Indicator for which shader program (for teapot) being used
	ShaderType currentShaderProgram;
	bool previousShaderState;
	

    int width, height; //Width height values of the screen
	float refractiveIndex; // Value for the refractive ratio 
	float dynamicChanger; // Quick value updated for dynamic reflections
	int currentRotation; // Indicator for current set rotation (for static movement)

 
	VBOTeapot *teapot;  //Teapot VBO
	VBOCube *skyBox; // Skybox
	VBOPlane *plane; // Flat quad
	Fbo *cubeMapFBO; // Framebuffer object to hold the cubemap scene

    mat4 model; //Model matrix


    void setMatrices(QuatCamera camera, GLSLProgram& shaderProgram); //Set the camera matrices
	void setMatrices(CubeMapCamera camera, GLSLProgram& shaderProgram); //Set the matrices for FBO reflected objects
    void compileAndLinkShader(std::string progAddress, GLSLProgram& shaderProg); //Compile and link the shader


public:
    CubeMapScene(); //Constructor

	void setLightParams(GLSLProgram& shaderProgram); //Setup the lighting

    void initScene(QuatCamera camera); //Initialise the scene

    void update(QuatCamera camera, float t ); //Update the scene

    void render(QuatCamera camera);	//Render the scene

	void renderFBOObjects(CubeMapCamera camera);

    void resize(QuatCamera camera, int, int); //Resize

	void currentShader(); //Shader switcher

	void keyHandler(QuatCamera *camera, int keycode);
};

}

#endif // SCENEDIFFUSE_H
