#pragma once

#include "System.h"

struct GLFWwindow;
namespace engineECS
{
	//initializes/deinitializes all opengl related stuff
	class OpenGLWrapper
	{
	public:
		OpenGLWrapper(const char* screenTitle = "Engine by Matteo Lorenzo Nasci\0", const int width = 1920, const int height = 1080);
		~OpenGLWrapper();

		OpenGLWrapper(const OpenGLWrapper&) = delete;
		OpenGLWrapper(OpenGLWrapper&&) = delete;
		OpenGLWrapper& operator=(const OpenGLWrapper&) = delete;
		OpenGLWrapper& operator=(OpenGLWrapper&&) = delete;

		bool shouldCloseWindow() const;
		void SetClearColor(const float r = 0, const float g = 0, const float b = 0, const float a = 1) const;

		void prepareFrame();
		void finalizeFrame();

		static double getTime();
	private:
		GLFWwindow* window;
	};

}
