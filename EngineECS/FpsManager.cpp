#include "FpsManager.h"

#include <memory>
#include <iostream>

void engineECS::FpsManager::setDebugOptions(const bool isEnabled, const int inDebugDisplayInterval)
{
	debugEnabled = isEnabled;
	debugDisplayInterval = inDebugDisplayInterval;
}
bool engineECS::FpsManager::isDebugEnabled() const
{
	return debugEnabled;
}
int engineECS::FpsManager::getDebugDisplayInterval() const
{
	return debugDisplayInterval;
}
engineECS::FpsManager::FpsManager(double(*const inGetTime)(), void(*const inWaitingTargetFps)(const double estimatedWaitingTimeLeft)) : getTime(inGetTime), waitingTargetFps(inWaitingTargetFps), debugEnabled(false), debugDisplayInterval(200)
{
	startupTime = getTime();
	lastFrameTime = startupTime;
	totalFrames = 0;
	savedDeltaTimesCounter = 0;
	std::memset(savedDeltaTimes, 1, engineECS::FpsManager::SavedDeltaTimesCount * sizeof(float));
	currentDeltaTime = 0.f;
	currentFrameTime = 0.;
	targetFramePerSecond = engineECS::FpsManager::DefaultTargetFramePerSecond;
	totalFramePerSecond = 0.f;
	currentFramePerSecond = 0.f;
	savedFramePerSecond = 0.f;
	savedMinFramePerSecond = 0.f;
	savedMaxFramePerSecond = 0.f;
}
void engineECS::FpsManager::update()
{
	if (debugEnabled)
	{
		if (totalFrames % debugDisplayInterval == 0)
		{
			std::cout << "Current fps: " << currentFramePerSecond << ", saved: " << savedFramePerSecond << ", max: " << savedMaxFramePerSecond << ", min: " << savedMinFramePerSecond << ", total fps: " << totalFramePerSecond << " FPS." << std::endl;
		}
	}

	++totalFrames;

	lastFrameTime = currentFrameTime;

	currentFrameTime = getTime();
	currentDeltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
	currentFramePerSecond = 1.f / currentDeltaTime;
	//waiting for target fps if required
	while (targetFramePerSecond > 0. && currentFramePerSecond > targetFramePerSecond)
	{
		if (waitingTargetFps)
		{
			waitingTargetFps((lastFrameTime + targetFramePerSecond / 60.) - currentFrameTime);
		}

		currentFrameTime = getTime();
		currentDeltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
		currentFramePerSecond = 1.f / currentDeltaTime;
	}

	savedDeltaTimes[savedDeltaTimesCounter] = currentDeltaTime;
	if (++savedDeltaTimesCounter >= engineECS::FpsManager::SavedDeltaTimesCount)
	{
		savedDeltaTimesCounter = 0;
	}

	savedFramePerSecond = 0;
	savedMinFramePerSecond = FLT_MAX;
	savedMaxFramePerSecond = -FLT_MAX;
	for (int i = 0; i < engineECS::FpsManager::SavedDeltaTimesCount; ++i)
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
	savedFramePerSecond = 1 / (savedFramePerSecond / static_cast<float>(engineECS::FpsManager::SavedDeltaTimesCount));

	totalFramePerSecond = 1.f / static_cast<float>(((currentFrameTime - startupTime) / totalFrames));
}
double engineECS::FpsManager::getStartupTime() const
{
	return startupTime;
}
double engineECS::FpsManager::getLastFrameTime() const
{
	return lastFrameTime;
}
double engineECS::FpsManager::getCurrentFrameTime() const
{
	return currentFrameTime;
}
float engineECS::FpsManager::getDeltaTime() const
{
	return currentDeltaTime;
}
float engineECS::FpsManager::getCurrentFramePerSecond() const
{
	return currentFramePerSecond;
}
float engineECS::FpsManager::getSavedFramePerSecond() const
{
	return savedFramePerSecond;
}
float engineECS::FpsManager::getSavedMaxFramePerSecond() const
{
	return savedMaxFramePerSecond;
}
float engineECS::FpsManager::getSavedMinFramePerSecond() const
{
	return savedMinFramePerSecond;
}
long long unsigned int engineECS::FpsManager::getTotalFramesCount() const
{
	return totalFrames;
}
float engineECS::FpsManager::getTotalFramePerSecond() const
{
	return totalFramePerSecond;
}
double engineECS::FpsManager::getTargetFramePerSecond() const
{
	return targetFramePerSecond;
}
void engineECS::FpsManager::setTargetFramePerSecond(const double inTargetFramePerSecond)
{
	targetFramePerSecond = inTargetFramePerSecond;
}