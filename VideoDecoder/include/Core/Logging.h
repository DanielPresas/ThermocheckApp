#ifndef LOGGING_H
#define LOGGING_H

#include "Core/Memory.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Logger {
public:

	static void init();
	static void shutdown();
	static Ref<spdlog::logger> getLogger();

private:

	static Ref<spdlog::logger> _logger;
};

#endif

#if _DEBUG || _RELEASE

#define LOG_CRITICAL(message, ...)	::Logger::getLogger()->critical(message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...)		::Logger::getLogger()->error(message, ##__VA_ARGS__)
#define LOG_WARNING(message, ...)	::Logger::getLogger()->warn(message, ##__VA_ARGS__)
#define LOG_INFO(message, ...)		::Logger::getLogger()->info(message, ##__VA_ARGS__)
#define LOG_TRACE(message, ...)		::Logger::getLogger()->trace(message, ##__VA_ARGS__)


#define LOG_ASSERT(checkIfTrue, ...) \
if(!(checkIfTrue)) {\
	::Logger::getLogger()->critical("ASSERTION FAILED: {0}", #checkIfTrue);\
	::Logger::getLogger()->error(##__VA_ARGS__);\
	__debugbreak();\
}

#else

#define LOG_CRITICAL(message, ...)
#define LOG_ERROR(message, ...)
#define LOG_WARNING(message, ...)
#define LOG_INFO(message, ...)
#define LOG_TRACE(message, ...)
#define LOG_ASSERT(checkIfTrue, ...)

#endif