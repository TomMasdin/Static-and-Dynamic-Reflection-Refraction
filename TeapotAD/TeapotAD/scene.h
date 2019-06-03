#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <glfw3.h>
#include "QuatCamera.h"

namespace imat3111
{

class Scene
{
public:
	Scene() : m_animate(true) { }
	virtual ~Scene() { }
    /**
      Load textures, initialize shaders, etc.
      */
    virtual void initScene(QuatCamera camera) = 0;

    /**
      This is called prior to every frame.  Use this
      to update your animation.
      */
    virtual void update(QuatCamera camera, float t ) = 0;

    /**
      Draw your scene.
      */
    virtual void render(QuatCamera camera) = 0;

    /**
      Called when screen is resized
      */
    virtual void resize(QuatCamera camera,int, int) = 0;

	virtual void keyHandler(QuatCamera *camera, int keycode) = 0;
    
    void animate( bool value ) { m_animate = value; }
    bool animating() { return m_animate; }

	void switchShader(bool value) { m_shaderSwitch = value; }
	bool switchingShader() { return m_shaderSwitch; }
    
protected:
	bool m_animate;
	bool m_shaderSwitch;
};

}

#endif // SCENE_H
