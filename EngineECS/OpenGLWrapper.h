#pragma once

#include <memory>

#include "System.h"

struct GLFWwindow;
namespace engineECS
{
	//initializes/deinitializes all sdl related stuff
	class OpenGLWrapper
	{
	public:
		OpenGLWrapper(const char* screenTitle = "Engine by Matteo Lorenzo Nasci\0", const int width = 1920, const int height = 1080);
		~OpenGLWrapper();

		OpenGLWrapper(const OpenGLWrapper&) = delete;
		OpenGLWrapper(OpenGLWrapper&&) = delete;
		OpenGLWrapper& operator=(const OpenGLWrapper&) = delete;
		OpenGLWrapper& operator=(OpenGLWrapper&&) = delete;

		static constexpr double DefaultTargetFramePerSecond = 60.001;

		bool shouldCloseWindow() const;
		void SetClearColor(const float r = 0, const float g = 0, const float b = 0, const float a = 1);

		void prepareFrame();
		void finalizeFrame();

		double getStartupTime() const;
		double getLastFrameTime() const;
		double getCurrentFrameTime() const;
		double getTargetFramePerSecond() const;
		void setTargetFramePerSecond(const double targetFramePerSecond);
		float getDeltaTime() const;
		float getCurrentFramePerSecond() const;
		float getSavedFramePerSecond() const;
		float getSavedMaxFramePerSecond() const;
		float getSavedMinFramePerSecond() const;
		float getTotalFramePerSecond() const;
		long long unsigned int getTotalFramesCount() const;
	private:
		double targetFramePerSecond;

		static constexpr int SavedDeltaTimesCount = 30;

		double startupTime;
		double currentFrameTime;
		long long unsigned int totalFrames;
		float totalFramePerSecond;

		double lastFrameTime;
		float currentDeltaTime;
		float currentFramePerSecond;


		float savedDeltaTimes[SavedDeltaTimesCount];
		int savedDeltaTimesCounter;
		float savedFramePerSecond;
		float savedMinFramePerSecond;
		float savedMaxFramePerSecond;

		GLFWwindow* window;
	};

}
