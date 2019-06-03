#pragma once

#include "gl_core_4_3.hpp"

#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//!< Basic camera setup for Cube Mapping using FBOs
class CubeMapCamera
{
private: 
	//Fixed variables
	float NEAR_PLANE = 0.1f;
	float FAR_PLANE = 200.f;
	float FOV = 90;
	float ASPECT_RATIO = 1;

	//Position and rotation
	glm::vec3 centerPoint;
	float pitch = 0;
	float yaw = 0;

	//Transform Matrices
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionViewMatrix;

	//Updaters
	void createProjectionMatrix();
	void updateViewMatrix();

public:

	//Position is set at construction
	CubeMapCamera(); 

	//Used to move the camera uniformally for a cube map
	void switchToFace(int faceIndex);
	void setPosition(glm::vec3 position);

	//Basic getters
	glm::vec3 getPosition();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getProjectionViewMatrix();

};