#include "TimeDate.h"

Engine::Time::TimePoint Engine::Time::engineStart = Time::Clock::now();

unsigned long long Engine::Time::TimeBetween(const TimePoint& x, const TimePoint& y)
{
	return std::abs(std::chrono::duration_cast<Engine::Time::Precision>(x - y).count());
}

unsigned long long Engine::Time::EngineStartTime()
{
	return std::chrono::duration_cast<Engine::Time::Precision>(std::chrono::time_point_cast<Engine::Time::Precision>(Engine::Time::engineStart).time_since_epoch()).count();
}

unsigned long long Engine::Time::Now()
{
	return std::chrono::duration_cast<Engine::Time::Precision>(Engine::Time::Clock::now().time_since_epoch() - Engine::Time::engineStart.time_since_epoch()).count();
}

Engine::Time::TimePoint Engine::Time::TimePointNow()
{
	return Engine::Time::Clock::now();
}

std::string Engine::Time::ToString()
{
	return std::format("{:%F %T}", Engine::Time::Clock::now());
}

std::string Engine::Time::ToString(const TimePoint& timePoint)
{
	return std::format("{:%F %T}", timePoint);
}

