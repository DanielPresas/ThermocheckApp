#include "tcpch.h"

Random::MTEngine Random::_randomEngine = MTEngine(std::random_device()());
Random::IntDistribution Random::_intDistribution;

void Random::newSeed() {
	_randomEngine.seed(std::random_device()());
}

bool Random::Bool() {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return randGen >= 0.5f;
}

int8_t Random::Char(const int8_t low, const int8_t high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return static_cast<int8_t>(randGen * static_cast<float>(high) + static_cast<float>(low));
}

int16_t Random::Short(const int16_t low, const int16_t high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return static_cast<int16_t>(randGen * static_cast<float>(high) + static_cast<float>(low));
}

int32_t Random::Int(const int32_t low, const int32_t high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return static_cast<int32_t>(randGen * static_cast<float>(high) + static_cast<float>(low));
}
int64_t Random::Long(const int64_t low, const int64_t high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return static_cast<int64_t>(randGen * static_cast<float>(high) + static_cast<float>(low));
}

uint8_t Random::UChar(const uint8_t low, const uint8_t high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return static_cast<uint8_t>(randGen * static_cast<float>(high) + static_cast<float>(low));
}

uint16_t Random::UShort(const uint16_t low, const uint16_t high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return static_cast<uint16_t>(randGen * static_cast<float>(high) + static_cast<float>(low));
}

uint32_t Random::UInt(const uint32_t low, const uint32_t high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return static_cast<uint32_t>(randGen * static_cast<float>(high) + static_cast<float>(low));
}

uint64_t Random::ULong(const uint64_t low, const uint64_t high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return static_cast<uint64_t>(randGen * static_cast<float>(high) + static_cast<float>(low));
}

float Random::Float(const float low, const float high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return randGen * high + low;
}

double Random::Double(const double low, const double high) {
	const float randGen = static_cast<float>(_intDistribution(_randomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
	return static_cast<double>(randGen) * high + low;
}
