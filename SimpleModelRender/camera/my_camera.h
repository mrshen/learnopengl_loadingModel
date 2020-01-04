#pragma once
#ifndef MY_CAMERA_H
#define MY_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyCamera
{
public:
	MyCamera(glm::vec3 camPos, glm::vec3 targetPos);
	MyCamera(glm::vec3 camPos, float pitch, float yaw);
	glm::mat4 getViewMat4();
	void updateCamPosition(int directionFlag, float sensitivity);
	void updateCamDirection(float pitch, float yaw);
	float getPitch() { return this->pitch; }
	float getYaw() { return this->yaw; }
	glm::vec3 getCamPosition();
	glm::vec3 getCamDirection();

private:
	float pitch = -90.f;
	float yaw = .0f;

	glm::vec3 camPosition;
	glm::vec3 camRight;
	glm::vec3 camUp;
	glm::vec3 camDirection;
	glm::mat4 viewMat4;

	glm::vec3 worldUp = glm::vec3(0.f, 1.f, 0.f);
};

#endif