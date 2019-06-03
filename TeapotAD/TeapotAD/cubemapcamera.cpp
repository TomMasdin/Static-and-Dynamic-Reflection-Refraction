#include "cubemapcamera.h"

//Creates a default projection matrix
void CubeMapCamera::createProjectionMatrix()
{
	float y_scale = (float)((1.f / glm::tan(glm::radians(FOV / 2.f))));
	float x_scale = y_scale / ASPECT_RATIO;
	float frustum_length = FAR_PLANE - NEAR_PLANE;

	projectionMatrix[0][0] = x_scale;
	projectionMatrix[1][1] = y_scale;
	projectionMatrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	projectionMatrix[2][3] = -1;
	projectionMatrix[3][2] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
	projectionMatrix[3][3] = 0;
}
//Updates the view matrix
void CubeMapCamera::updateViewMatrix()
{
	viewMatrix = glm::mat4(1.0);
	viewMatrix = glm::rotate(viewMatrix, 180.f, glm::vec3(0, 0, 1));
	viewMatrix = glm::rotate(viewMatrix, pitch, glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, yaw,   glm::vec3(0, 1, 0));
	glm::vec3 negativeCameraPos = glm::vec3(-centerPoint.x, -centerPoint.y, -centerPoint.z);
	viewMatrix = glm::translate(viewMatrix, negativeCameraPos);
	
	projectionViewMatrix = glm::matrixCompMult(projectionMatrix, viewMatrix);
}

CubeMapCamera::CubeMapCamera()
{
	centerPoint = glm::vec3(0, 0, 0);
	createProjectionMatrix();
}
//Switches to a certain direction for the cube map
void CubeMapCamera::switchToFace(int faceIndex)
{
	switch (faceIndex) {
	case 0:
		pitch = 0;
		yaw = 90;
		break;
	case 1:
		pitch = 0;
		yaw = -90;
		break;
	case 2:
		pitch = -90;
		yaw = 180;
		break;
	case 3:
		pitch = 90;
		yaw = 180;
		break;
	case 4:
		pitch = 0;
		yaw = 180;
		break;
	case 5:
		pitch = 0;
		yaw = 0;
		break;
	}
	updateViewMatrix();
}

//Basic getters and setters
void CubeMapCamera::setPosition(glm::vec3 position)
{
	centerPoint = position;
}

glm::vec3 CubeMapCamera::getPosition()
{
	return centerPoint;
}

glm::mat4 CubeMapCamera::getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 CubeMapCamera::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 CubeMapCamera::getProjectionViewMatrix()
{
	return projectionViewMatrix;
}
