#include "Fbo.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900



	Fbo::Fbo(int width, int height, int depthBufferType)
	{
		this->width = width;
		this->height = height;
		initFrameBuffer(depthBufferType);
	}

	Fbo::Fbo(imat3111::Scene *currentScene, glm::vec3 position, float size)
	{

		this->newScene = (dynamic_cast<CubeMapScene*>(currentScene));
		cubeMapCamera.setPosition(position);
		
		width = size;
		height = size;

		gl::GenTextures(1, &cubeMapTexture);
		gl::BindTexture(gl::TEXTURE_CUBE_MAP, cubeMapTexture);
		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE);

		// Allocate space for each side of the cube map
		for (GLuint i = 0; i < 6; ++i)
		{
			gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gl::RGBA, width,
				height, 0, gl::RGBA, gl::UNSIGNED_BYTE, NULL);
		}

		gl::GenFramebuffers(1, &frameBuffer);
		gl::BindFramebuffer(gl::FRAMEBUFFER, frameBuffer);
		gl::DrawBuffer(gl::COLOR_ATTACHMENT0);

		gl::GenRenderbuffers(1, &depthBuffer);
		gl::BindRenderbuffer(gl::RENDERBUFFER, depthBuffer);
		gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH_COMPONENT24, width, height);
		gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, depthBuffer);

		gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
	

	}

	void Fbo::fboCubeMap()
	{
		gl::Viewport(0.0, 0.0, width, height);
		for (int i = 0; i < 6; i++)
		{
			gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_CUBE_MAP_POSITIVE_X + i,
				cubeMapTexture, 0);
			gl::Clear(gl::DEPTH_BUFFER_BIT);
			cubeMapCamera.switchToFace(i);
			newScene->renderFBOObjects(cubeMapCamera);
		}
	}


	void Fbo::cleanUp()
	{
		gl::DeleteFramebuffers(1, &frameBuffer);
		gl::DeleteTextures(1, &colourTexture);
		gl::DeleteTextures(1, &depthTexture);
		gl::DeleteTextures(1, &cubeMapTexture);
		gl::DeleteRenderbuffers(1, &depthBuffer);
		gl::DeleteRenderbuffers(1, &colourBuffer);
		gl::DeleteRenderbuffers(1, &colourBuffer2);
	}

	void Fbo::bindFrameBuffer()
	{
		gl::BindFramebuffer(gl::FRAMEBUFFER, frameBuffer);
		gl::Viewport(0, 0, width, height);
	}

	void Fbo::unbindFrameBuffer()
	{
		gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
		gl::Viewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	void Fbo::bindToRead()
	{
		gl::BindTexture(gl::TEXTURE_2D, 0);
		gl::BindFramebuffer(gl::READ_FRAMEBUFFER, frameBuffer);
		gl::ReadBuffer(gl::COLOR_ATTACHMENT0);
	}

	void Fbo::resolveToFbo(int readBuffer, Fbo outputFbo)
	{
		gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, outputFbo.frameBuffer);
		gl::BindFramebuffer(gl::READ_FRAMEBUFFER, this->frameBuffer);
		gl::ReadBuffer(readBuffer);
		gl::BlitFramebuffer(0, 0, width, height, 0, 0, outputFbo.width, outputFbo.height,
			gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT, gl::NEAREST);
		this->unbindFrameBuffer();
	}

	void Fbo::resolveToScreen()
	{
		gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, 0);
		gl::BindFramebuffer(gl::READ_FRAMEBUFFER, this->frameBuffer);
		gl::ReadBuffer(gl::BACK);
		gl::BlitFramebuffer(0, 0, width, height, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
			gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT, gl::NEAREST);
		this->unbindFrameBuffer();
	}


	void Fbo::initFrameBuffer(int type)
	{
		createFrameBuffer();
		createTextureAttachment();

		if (type == DEPTH_RENDER_BUFFER)
		{
			createDepthBufferAttachment();
		}

		else if (type == DEPTH_TEXTURE)
		{
			createDepthTextureAttachment();
		}

		unbindFrameBuffer();
	}

	void Fbo::createFrameBuffer()
	{
		gl::GenFramebuffers(1, &frameBuffer);
		gl::BindFramebuffer(gl::FRAMEBUFFER, frameBuffer);
		gl::DrawBuffer(gl::COLOR_ATTACHMENT0);

	}

	void Fbo::createTextureAttachment()
	{
		gl::GenTextures(1, &colourTexture);
		gl::BindTexture(gl::TEXTURE_2D, colourTexture);
		gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA8, width, height, 0, gl::RGBA, gl::UNSIGNED_BYTE, (GLubyte *)NULL);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
		gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, colourTexture, 0);
	}

	void Fbo::createDepthTextureAttachment()
	{
		gl::GenTextures(1, &depthTexture);
		gl::BindTexture(gl::TEXTURE_2D, depthTexture);
		gl::TexImage2D(gl::TEXTURE_2D, 0, gl::DEPTH_COMPONENT24, width, height, 0, gl::DEPTH_COMPONENT,
			gl::FLOAT, (GLubyte *)NULL);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::TEXTURE_2D, depthTexture, 0);
	}

	void Fbo::createDepthBufferAttachment()
	{
		gl::GenRenderbuffers(1, &depthBuffer);
		gl::BindRenderbuffer(gl::RENDERBUFFER, depthBuffer);
		gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH_COMPONENT24, width, height);
		gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, depthBuffer);
	}

	int Fbo::getColourTexture()
	{
		return colourTexture;
	}

	int Fbo::getDepthTexture()
	{
		return depthTexture;
	}

	unsigned int Fbo::getCubeMapTexture()
	{
		return cubeMapTexture;
	}
