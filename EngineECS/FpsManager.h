#pragma once

namespace engineECS
{
	class FpsManager
	{
	public:
		static constexpr int SavedDeltaTimesCount = 30;
		static constexpr double DefaultTargetFramePerSecond = 60.001;

		FpsManager(double(*const inGetTime)());

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

