#pragma once

#include "chrono"
#include "string"
#include "format"
#include "type_traits"

namespace Engine
{
	// static class which provides easy access to time measurment
	// all time is calculated and provided in milliseconds
	class Time
	{
		using Clock = std::chrono::system_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		using Precision = std::chrono::milliseconds;

	public:
		Time() = delete;

		static unsigned long long TimeBetween(const TimePoint&, const TimePoint&);
		static unsigned long long EngineStartTime();
		static unsigned long long Now();

		static TimePoint TimePointNow();

		static void StartStopWatch();
		static unsigned long long StopStopWatch();

		static std::string ToString();
		static std::string ToString(const TimePoint&);

	private:
		static TimePoint stopWatchStart;
		static TimePoint engineStart;
	};
}

