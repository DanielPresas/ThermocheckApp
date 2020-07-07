#include "vdpch.h"
#include "Core/Logging.h"

#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>

Ref<spdlog::logger> Logger::_logger;

void Logger::init() {
	spdlog::set_pattern("%^[Level: %l] (Time: %H:%M:%S.%e)\n----------------------------------------\n%v%$");
	_logger = spdlog::stdout_color_mt("LOGGER");
	_logger->set_level(spdlog::level::trace);
}

Ref<spdlog::logger> Logger::getLogger() { return _logger; }
