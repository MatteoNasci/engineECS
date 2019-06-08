#include "Camera.h"



engineECS::Camera::Camera()
{
	view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0), glm::vec3(0, 1, 0));

	projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.01f, 1000.0f);
}
glm::mat4& engineECS::Camera::getView()
{
	return view;
}
glm::mat4& engineECS::Camera::getProjection()
{
	return projection;
}

engineECS::Camera::~Camera()
{
}
