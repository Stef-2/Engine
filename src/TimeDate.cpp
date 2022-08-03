#include "TimeDate.h"

Engine::TimeDate::TimePoint Engine::TimeDate::engineStart = TimeDate::Clock::now();

unsigned long long Engine::TimeDate::TimeBetween(const TimePoint& x, const TimePoint& y)
{
	return std::abs(std::chrono::duration_cast<Engine::TimeDate::Precision>(x - y).count());
}

unsigned long long Engine::TimeDate::EngineStartTime()
{
	return std::chrono::duration_cast<Engine::TimeDate::Precision>(std::chrono::time_point_cast<Engine::TimeDate::Precision>(Engine::TimeDate::engineStart).time_since_epoch()).count();
}

unsigned long long Engine::TimeDate::Now()
{
	return std::chrono::duration_cast<Engine::TimeDate::Precision>(Engine::TimeDate::Clock::now().time_since_epoch() - Engine::TimeDate::engineStart.time_since_epoch()).count();
}

Engine::TimeDate::TimePoint Engine::TimeDate::TimePointNow()
{
	return Engine::TimeDate::Clock::now();
}

std::string Engine::TimeDate::ToString()
{
	return std::format("{:%F %T}", Engine::TimeDate::Clock::now());
}

std::string Engine::TimeDate::ToString(const TimePoint& timePoint)
{
	return std::format("{:%F %T}", timePoint);
}

