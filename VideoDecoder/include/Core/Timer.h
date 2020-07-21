#pragma once

#include <chrono>
#include <cstdint>
#include <string>

class Timer {
	using SCTP = std::chrono::steady_clock::time_point;
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;
	
public:

	Timer(std::string&& name = "Timer" + std::to_string(_id++), bool start = false);
	~Timer();

	void start();
	void stop();
	
private:

	SCTP _startTimePoint;
	bool _stopped = true;
	std::string _name;

	static uint32_t _id;
};
