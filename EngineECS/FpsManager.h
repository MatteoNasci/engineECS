#pragma once

namespace engineECS
{
	class FpsManager
	{
	public:
		static constexpr int SavedDeltaTimesCount = 30;
		static constexpr double DefaultTargetFramePerSecond = 60.001;

		//inWaitingTargetFps is called if it is a valid pointer and if the manager update is waiting for the target fps, allowing to not waste the waiting time. The function will only be called if there is a need to wait for the target frame rate, in which case it will be called continuosly until the waiting is over. DO NOT execute, directly or indirectly, code that requires either the current deltatime or current frame per second and AVOID to execute heavy code, it may cause noticeable fps drops.
		FpsManager(double(*const inGetTime)(), void(*const inWaitingTargetFps)(const double estimatedWaitingTimeLeft) = nullptr);

		void update();
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

		void setDebugOptions(const bool isEnabled, const int debugDisplayInterval = 200);
		bool isDebugEnabled() const;
		int getDebugDisplayInterval() const;
	private:
		bool debugEnabled;
		int debugDisplayInterval;

		double(*getTime)();
		void(*const waitingTargetFps)(const double estimatedWaitingTimeLeft);

		double targetFramePerSecond;

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
	};
}

