#include <iostream>
#include "OpenGLWrapper.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <exception>

void engineECS::OpenGLWrapper::prepareFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
double engineECS::OpenGLWrapper::getTime()
{
	return glfwGetTime();
}
bool engineECS::OpenGLWrapper::shouldCloseWindow() const
{
	if (!window)
	{
		return true;
	}
	return glfwWindowShouldClose(window);
}
void engineECS::OpenGLWrapper::finalizeFrame()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
}
engineECS::OpenGLWrapper::OpenGLWrapper(const char* screenTitle, const int width, const int height) : window(nullptr)
{
	/* Initialize the library */
	if (!glfwInit())
	{
		throw std::exception("Failed to initialize glfw library");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, screenTitle, nullptr, nullptr);
	if (!window)
	{
		throw std::exception("Failed to create opengl context");
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	SetClearColor(0, 0, 1, 1);

	glEnable(GL_DEPTH_TEST);
}
void engineECS::OpenGLWrapper::SetClearColor(const float r, const float g, const float b, const float a) const
{
	glClearColor(r, g, b, a);
}

engineECS::OpenGLWrapper::~OpenGLWrapper()
{
	if (window)
	{
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}