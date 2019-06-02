#include <iostream>
#include "OpenGLWrapper.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

double engineECS::OpenGLWrapper::getStartupTime() const
{
	return startupTime;
}
double engineECS::OpenGLWrapper::getLastFrameTime() const
{
	return lastFrameTime;
}
double engineECS::OpenGLWrapper::getCurrentFrameTime() const
{
	return currentFrameTime;
}
float engineECS::OpenGLWrapper::getDeltaTime() const
{
	return currentDeltaTime;
}
float engineECS::OpenGLWrapper::getCurrentFramePerSecond() const
{
	return currentFramePerSecond;
}
float engineECS::OpenGLWrapper::getSavedFramePerSecond() const
{
	return savedFramePerSecond;
}
float engineECS::OpenGLWrapper::getSavedMaxFramePerSecond() const
{
	return savedMaxFramePerSecond;
}
float engineECS::OpenGLWrapper::getSavedMinFramePerSecond() const
{
	return savedMinFramePerSecond;
}
long long unsigned int engineECS::OpenGLWrapper::getTotalFramesCount() const
{
	return totalFrames;
}
float engineECS::OpenGLWrapper::getTotalFramePerSecond() const
{
	return totalFramePerSecond;
}
double engineECS::OpenGLWrapper::getTargetFramePerSecond() const
{
	return targetFramePerSecond;
}
void engineECS::OpenGLWrapper::setTargetFramePerSecond(const double inTargetFramePerSecond)
{
	targetFramePerSecond = inTargetFramePerSecond;
}
void engineECS::OpenGLWrapper::prepareFrame()
{
	++totalFrames;

	lastFrameTime = currentFrameTime;

	do
	{
		currentFrameTime = glfwGetTime();
		currentDeltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
		currentFramePerSecond = 1.f / currentDeltaTime;
	} while (targetFramePerSecond > 0. && currentFramePerSecond > targetFramePerSecond);
	
	savedDeltaTimes[savedDeltaTimesCounter] = currentDeltaTime;
	if (++savedDeltaTimesCounter >= engineECS::OpenGLWrapper::SavedDeltaTimesCount)
	{
		savedDeltaTimesCounter = 0;
	}

	savedFramePerSecond = 0;
	savedMinFramePerSecond = FLT_MAX;
	savedMaxFramePerSecond = -FLT_MAX;
	for (int i = 0; i < engineECS::OpenGLWrapper::SavedDeltaTimesCount; ++i)
	{
		const float value = savedDeltaTimes[i];
		const float framePerSecond = 1.f / value;

		if (framePerSecond > savedMaxFramePerSecond)
		{
			savedMaxFramePerSecond = framePerSecond;
		}
		if (framePerSecond < savedMinFramePerSecond)
		{
			savedMinFramePerSecond = framePerSecond;
		}

		savedFramePerSecond += value;
	}
	savedFramePerSecond = 1 / (savedFramePerSecond / static_cast<float>(engineECS::OpenGLWrapper::SavedDeltaTimesCount));

	totalFramePerSecond = 1.f / static_cast<float>(((currentFrameTime - startupTime) / totalFrames));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
engineECS::OpenGLWrapper::OpenGLWrapper(const char* screenTitle, const int width, const int height) : targetFramePerSecond(engineECS::OpenGLWrapper::DefaultTargetFramePerSecond)
{
	/* Initialize the library */
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize glfw library" << std::endl;
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
		std::cerr << "Failed to create opengl context" << std::endl;
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	SetClearColor(0, 0, 0, 1);

	startupTime = glfwGetTime();
	lastFrameTime = startupTime;
	totalFrames = 0;
	savedDeltaTimesCounter = 0;
	std::memset(savedDeltaTimes, 1, engineECS::OpenGLWrapper::SavedDeltaTimesCount * sizeof(float));
	currentDeltaTime = 0.f;
}
void engineECS::OpenGLWrapper::SetClearColor(const float r, const float g, const float b, const float a)
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