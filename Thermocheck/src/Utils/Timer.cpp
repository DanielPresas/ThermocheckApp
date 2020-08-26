#include "tcpch.h"

Timer::Timer(std::string&& name, const bool start) :
	_name(std::move(name))
{
	if(start) {
		this->start();
	}
}

Timer::~Timer() {
	if(!_stopped) {
		stop();
	}
}

void Timer::start() {
	if(_stopped) {
		_startTimePoint = std::chrono::steady_clock::now();
		Logger::debug("Timer \"{}\" started", _name);
		_stopped = false;
	}
	else {
		Logger::trace("Timer \"{}\" already started, continuing timer", _name);
	}
}
void Timer::stop() {
	const auto endTimePoint = std::chrono::steady_clock::now();
	const std::chrono::microseconds elapsed = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(_startTimePoint).time_since_epoch();

	Logger::debug("Timer \"{}\" stopped (Final time: {:03.3f} ms)", _name, static_cast<float>(elapsed.count()) / 1000.0f);
	_stopped = true;
}
