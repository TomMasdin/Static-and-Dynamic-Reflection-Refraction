#pragma once

#include "gl_core_4_3.hpp"

#include <glfw3.h>
#include "glslprogram.h"

#include <glm.hpp>
#include "vboteapot.h"
#include "QuatCamera.h"

#include "vbocube.h"
#include "scene.h"
#include "cubemapscene.h"
#include "cubemapcamera.h"
#include <iostream>


using namespace imat3111;

class Fbo
{

	 private:
		int width, height; // Width and height of the FBO
		GLuint frameBuffer; // Framebuffer ID
		CubeMapScene *newScene = nullptr; // Scene pointer for rendering
		CubeMapCamera cubeMapCamera; // Camera for generating cubemap fbo images


		bool multisampleMultiTarget = false; // For multi textured objects

		unsigned int colourTexture; //Colour of the scene
		unsigned int colourBuffer; // Buffer for rendering
		unsigned int colourBuffer2; // Buffer for rendering

		unsigned int depthTexture; // Depth of the scene
		unsigned int depthBuffer; // Buffer for rendering

		unsigned int cubeMapTexture; // Texture ID

		

		void initFrameBuffer(int type); // Initialises the framebuffer
		void createFrameBuffer(); // Creates the framebuffer object
		void createTextureAttachment(); // Creates and adds the texture attachment (Colour)
		void createDepthTextureAttachment(); // Creates and adds the texture attachment (Depth)
		void createDepthBufferAttachment(); // Creates and adds the buffer for rendering

	public:
		static const int NONE = 0;
		static const int DEPTH_TEXTURE = 1;
		static const int DEPTH_RENDER_BUFFER = 2;

		Fbo(int width, int height, int depthBufferType);
		Fbo(Scene *currentScene, glm::vec3 position, float size);

		void cleanUp(); //Clean up the FBO after close
		void bindFrameBuffer(); // Binds the frame buffer, setting it as the current render target. Anything rendered after this call will be rendered to fbo
		void unbindFrameBuffer(); //Unbinds the frame buffer, everything rendered after this will be rendered to the screen

		//Getters
		int getColourTexture();
		int getDepthTexture();
		unsigned int getCubeMapTexture();

		void bindToRead(); //Binds the current fbo to be read from
		void resolveToFbo(int readBuffer, Fbo outputFbo); // To pass between FBOs
		void resolveToScreen(); // To render to the screen
		void fboCubeMap(); //Basic automatic fbo for cubemap textures - takes camera and records surroundings

};
