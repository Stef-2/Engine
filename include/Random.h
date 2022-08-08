#pragma once

#include "random"
#include "TimeDate.h"

namespace Engine
{
	class Random
	{
	public:
		Random() = delete;

		static float Generate(float min, float max);
		static double Generate(double min, double max);

		static int Generate(int min, int max);
		static long Generate(long min, long max);

		static const unsigned char GetDoomRandom(unsigned short);
		static const unsigned char GetDoomRandom();

	private:
		// doom random numbers
		static const unsigned char rndtable[256];

		// RN Generator
		// testing showed that minstd_rand produces fastest results
		static std::minstd_rand generator;
	};
}

