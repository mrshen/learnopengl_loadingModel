#include "my_camera.h"
#include <GLFW\glfw3.h>

MyCamera::MyCamera(glm::vec3 camPos, glm::vec3 targetPos)
{
	this->camPosition = camPos;
	this->camDirection = glm::normalize(targetPos - camPos);
	this->camRight = glm::normalize(glm::cross(this->worldUp, this->camDirection));
	this->camUp = glm::normalize(glm::cross(this->camDirection, this->camRight));
}

MyCamera::MyCamera(glm::vec3 camPos, float pitch, float yaw)
{
	this->camPosition = camPos;
	this->updateCamDirection(pitch, yaw);
	this->camRight = glm::normalize(glm::cross(this->worldUp, this->camDirection));
	this->camUp = glm::normalize(glm::cross(this->camDirection, this->camRight));
}

glm::mat4 MyCamera::getViewMat4()
{
	return glm::lookAt(this->camPosition, this->camPosition + this->camDirection, this->worldUp);
}

void MyCamera::updateCamPosition(int directionFlag, float sensitivity)
{
	glm::vec3 offset;
	switch (directionFlag)
	{
	case GLFW_KEY_W:
		offset = this->camDirection;
		break;
	case GLFW_KEY_S:
		offset = -this->camDirection;
		break;
	case GLFW_KEY_A:
		offset = this->camRight;
		break;
	case GLFW_KEY_D:
		offset = -this->camRight;
		break;
	case GLFW_KEY_Q:
		offset = this->camUp;
		break;
	case GLFW_KEY_E:
		offset = -this->camUp;
		break;
	default:
		break;
	}

	this->camPosition += offset * sensitivity;
}

void MyCamera::updateCamDirection(float pitch, float yaw)
{
	this->pitch = pitch;
	this->yaw = yaw;
	glm::vec3 camDir;
	camDir.y = sin(glm::radians(pitch));
	camDir.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	camDir.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	this->camDirection = glm::normalize(camDir);

	this->camRight = glm::normalize(glm::cross(this->worldUp, this->camDirection));
	this->camUp = glm::normalize(glm::cross(this->camDirection, this->camRight));
}

glm::vec3 MyCamera::getCamPosition()
{
	return this->camPosition;
}

glm::vec3 MyCamera::getCamDirection()
{
	return this->camDirection;
}