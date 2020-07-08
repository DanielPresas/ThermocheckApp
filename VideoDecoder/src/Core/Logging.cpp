#include "vdpch.h"
#include "Core/Logging.h"

#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>

Ref<spdlog::logger> Logger::_logger = nullptr;

void Logger::init()
{
#if _DEBUG || _RELEASE
	
	spdlog::set_pattern("%^[%H:%M:%S.%e - %l]: %v%$");
	_logger = spdlog::stdout_color_mt("LOGGER");
	_logger->set_level(spdlog::level::trace);
	
#endif
}

Ref<spdlog::logger> Logger::getLogger() { return _logger; }
