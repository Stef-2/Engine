#pragma once

#include "chrono"
#include "string"
#include "format"
#include "type_traits"

namespace Engine
{
	// static class which provides easy access to time measurment
	// all time is calculated and provided in milliseconds
	static class TimeDate
	{
		using Clock = std::chrono::system_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		using Precision = std::chrono::milliseconds;

	public:
		TimeDate() = delete;

		static unsigned long long TimeBetween(const TimePoint&, const TimePoint&);
		static unsigned long long EngineStartTime();
		static unsigned long long Now();

		static TimePoint TimePointNow();

		static std::string ToString();
		static std::string ToString(const TimePoint&);

	private:
		static TimePoint engineStart;
	};
}

