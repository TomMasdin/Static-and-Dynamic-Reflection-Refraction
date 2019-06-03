
#include "cubemapscene.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "defines.h"
#include "Fbo.h"

using glm::vec3;
#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>


namespace imat3111
{
/////////////////////////////////////////////////////////////////////////////////////////////
// Default constructor
/////////////////////////////////////////////////////////////////////////////////////////////
CubeMapScene::CubeMapScene()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
//Initialise the scene
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::initScene(QuatCamera camera)
{
	
	previousShaderState = switchingShader();
    //|Compile and link the shader 
	compileAndLinkShader("phongReflection", phongProgram);
	compileAndLinkShader("reflection", reflectionProgram);
	compileAndLinkShader("refraction", refractionProgram);
	compileAndLinkShader("skybox", skyBoxProg);
	compileAndLinkShader("phong", planeProg);
	
	refractiveIndex = 1.52;

    gl::Enable(gl::DEPTH_TEST);

	setLightParams(phongProgram);
	setLightParams( reflectionProgram);
	setLightParams( refractionProgram);
	setLightParams( planeProg);

	
	//Create the skybox
	skyBox = new VBOCube(50.0);

	//A matrix to move the teapot lid upwards
	glm::mat4 lid = glm::mat4(1.0);
	//lid *= glm::translate(vec3(0.0,1.0,0.0));

	//Create the teapot with translated lid
	teapot = new VBOTeapot(16,lid);

	plane = new VBOPlane(10.0, 10.0, 10.0, 10.0);

	//Creating the cube map fbo with pointer to current scene
	cubeMapFBO = new Fbo(this, vec3(0, 2, 0), 1024);

	currentRotation = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Update not used at present
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::update(QuatCamera camera, float t )
{
	if (m_animate)
	{
		skyBox->update(t);
		//std::cout << "Animated" << endl;
	}
	dynamicChanger = t * 2;
	
	currentShader();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Set up the lighting variables in the shader
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::setLightParams(GLSLProgram& shaderProgram)
{
	shaderProgram.use();
	vec3 worldLight = vec3(10.0f,10.0f,10.0f);
	//Light material
	shaderProgram.setUniform("Ld", 0.9f, 0.9f, 0.9f);
	shaderProgram.setUniform("La", 0.3f, 0.3f, 0.3f);
	shaderProgram.setUniform("Ls", 0.3f, 0.3f, 0.3f);
	shaderProgram.setUniform("specularExponent", 8.f);
	shaderProgram.setUniform("specularIntensity", 10.f);
	//Surface material
	shaderProgram.setUniform("Kd", 0.9f, 0.9f, 0.9f);
	shaderProgram.setUniform("Ka", 0.9f, 0.9f, 0.9f);
	shaderProgram.setUniform("Ks", 0.3f, 0.3f, 0.3f);
	
	
	//Light Position
	shaderProgram.setUniform("lightPos", worldLight );
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Render the scene
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::render(QuatCamera camera)
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	cubeMapFBO->bindFrameBuffer();
	cubeMapFBO->fboCubeMap();
	cubeMapFBO->unbindFrameBuffer();

	skyBoxProg.use();
	skyBox->bindTextureUnits(skyBoxProg.getHandle());
	setMatrices(camera, skyBoxProg);
	skyBox->render();

	mat4 rotation;
	mat4 translate;

	switch (currentShaderProgram)
	{
	case Phong:
		phongProgram.use();
		model = mat4(1.0f);
		teapot->bindTextureUnits(phongProgram.getHandle(), cubeMapFBO->getCubeMapTexture());
		setMatrices(camera, phongProgram);
		teapot->render();

		planeProg.use();
		model = mat4(1.0f);
		rotation = glm::rotate(model, glm::degrees(dynamicChanger), vec3(0, 1, 0));
		translate = glm::translate(model, vec3(0, -1, 0));
		model = translate * rotation;
		setMatrices(camera, planeProg);
		plane->render();
		break;
	case Reflection:
		reflectionProgram.use();
		model = mat4(1.0f);
		reflectionProgram.setUniform("skybox", 0);
		reflectionProgram.setUniform("skybox2", 1);
		setMatrices(camera, reflectionProgram);
		teapot->render();
		break;
	case Refraction:
		refractionProgram.use();
		model = mat4(1.0f);
		teapot->bindTextureUnits(refractionProgram.getHandle(), cubeMapFBO->getCubeMapTexture());
		setMatrices(camera, refractionProgram);
		teapot->render();
		break;
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////
// Render the scene for the FBO
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::renderFBOObjects(CubeMapCamera camera)
{

	planeProg.use();
	model = mat4(1.0f);
	mat4 rotation = glm::rotate(model, glm::degrees(dynamicChanger), vec3(0, 1, 0));
	mat4 translate = glm::translate(model, vec3(0, -1, 0));
	model = translate * rotation;
	setMatrices(camera, planeProg);
	plane->render();

	
	skyBoxProg.use();
	skyBox->bindTextureUnits(skyBoxProg.getHandle());
	setMatrices(camera, skyBoxProg);
	skyBox->render();

	
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Send the MVP matrices to the GPU
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::setMatrices(QuatCamera camera, GLSLProgram& shaderProgram)
{
	shaderProgram.use();
	shaderProgram.setUniform("M", model);
	shaderProgram.setUniform("V", camera.view());
	shaderProgram.setUniform("P", camera.projection());

	mat4 mv = camera.view() * model;
	shaderProgram.setUniform("ModelViewMatrix", mv);
	shaderProgram.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	shaderProgram.setUniform("MVP", camera.projection() * mv);
	shaderProgram.setUniform("cameraPos", camera.position());

	glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.view()));
	shaderProgram.setUniform("view_matrix", viewMatrix);
	shaderProgram.setUniform("projection_matrix", camera.projection());

	shaderProgram.setUniform("blendFactor", skyBox->getBlendFactor());
	shaderProgram.setUniform("refractiveIndex", refractiveIndex);

	setLightParams(shaderProgram);
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Send the MVP matrices to the GPU (For Objects to be rendered in FBOs)
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::setMatrices(CubeMapCamera camera, GLSLProgram& shaderProgram)
{
	shaderProgram.use();
	shaderProgram.setUniform("M", model);
	shaderProgram.setUniform("V", camera.getViewMatrix());
	shaderProgram.setUniform("P", camera.getProjectionMatrix());

	mat4 mv = camera.getViewMatrix() * model;
	shaderProgram.setUniform("ModelViewMatrix", mv);
	shaderProgram.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	shaderProgram.setUniform("MVP", camera.getProjectionMatrix() * mv);
	shaderProgram.setUniform("cameraPos", camera.getPosition());

	glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.getViewMatrix()));
	shaderProgram.setUniform("view_matrix", viewMatrix);
	shaderProgram.setUniform("projection_matrix", camera.getProjectionMatrix());

	shaderProgram.setUniform("blendFactor", skyBox->getBlendFactor());
	setLightParams(shaderProgram);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// resize the viewport
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::resize(QuatCamera camera,int w, int h)
{
    gl::Viewport(0,0,w,h);
    width = w;
    height = h;
	camera.setAspectRatio((float)w/h);

}


/////////////////////////////////////////////////////////////////////////////////////////////
// Cycle through active shader programs
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::currentShader()
{
	if (switchingShader() != previousShaderState)
	{
		currentShaderProgram = static_cast<ShaderType>(static_cast<int>(currentShaderProgram) + 1);
		if (currentShaderProgram == ShaderType::END)
		{
			currentShaderProgram = ShaderType::Phong;
		}
	}
	previousShaderState = switchingShader();
	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Handle key inputs directly related to the scene (Key functions explained in report)
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::keyHandler(QuatCamera* camera, int keycode)
{
	if (keycode == '2')
	{
		refractiveIndex += 0.01;
	}
	if (keycode == '3')
	{
		refractiveIndex -= 0.01;
	}
	if (keycode == 'R')
	{
		refractiveIndex = 1.52;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Compile and link the shader
/////////////////////////////////////////////////////////////////////////////////////////////
void CubeMapScene::compileAndLinkShader(std::string progName, GLSLProgram& shaderProg)
{
   
	try {
		std::string vert = "Shaders/" + progName + ".vert";
		std::string frag = "Shaders/" + progName + ".frag";
		shaderProg.compileShader(vert.c_str());
		shaderProg.compileShader(frag.c_str());
		shaderProg.link();
		shaderProg.validate();
    } catch(GLSLProgramException & e) {
 		cerr << e.what() << endl;
 		exit( EXIT_FAILURE );
    }
}

}
