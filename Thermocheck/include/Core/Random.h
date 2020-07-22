#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random {
	
	using MTEngine = std::mt19937_64;
	using IntDistribution = std::uniform_int_distribution<MTEngine::result_type>;

public:

	static void newSeed();

	static bool Bool();
	
	static int8_t  Char (int8_t  low = 0, int8_t  high = 10);
	static int16_t Short(int16_t low = 0, int16_t high = 10);
	static int32_t Int  (int32_t low = 0, int32_t high = 10);
	static int64_t Long (int64_t low = 0, int64_t high = 10);

	static uint8_t  UChar (uint8_t  low = 0u, uint8_t  high = 10u);
	static uint16_t UShort(uint16_t low = 0u, uint16_t high = 10u);
	static uint32_t UInt  (uint32_t low = 0u, uint32_t high = 10u);
	static uint64_t ULong (uint64_t low = 0u, uint64_t high = 10u);
	
	static float  Float (float  low = 0.0f, float  high = 1.0f);
	static double Double(double low = 0.0,  double high = 1.0);

private:

	static MTEngine _randomEngine;
	static IntDistribution _intDistribution;
	
};

#endif
