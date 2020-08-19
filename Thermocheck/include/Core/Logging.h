#ifndef LOGGING_H
#define LOGGING_H

#if PLATFORM_LINUX
#include <signal.h>
#endif

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>


class Logger {
public:

	static void init();
	static void shutdown();
	
	template<typename FormatString, typename ...Args>
	static void critical(const FormatString& message, const Args&... args) {
#if TC_DEBUG || TC_RELEASE
		_logger->critical(message, args...);
#endif
	}
	
	template<typename FormatString, typename ...Args>
	static void error(const FormatString& message, const Args&... args) {
#if TC_DEBUG || TC_RELEASE
		_logger->error(message, args...);
#endif
	}
	
	template<typename FormatString, typename ...Args>
	static void warning(const FormatString& message, const Args&... args) {
#if TC_DEBUG || TC_RELEASE
		_logger->warn(message, args...);
#endif
	}
	
	template<typename FormatString, typename ...Args>
	static void info(const FormatString& message, const Args&... args) {
#if TC_DEBUG || TC_RELEASE
		_logger->info(message, args...);
#endif
	}
	
	template<typename FormatString, typename ...Args>
	static void trace(const FormatString& message, const Args&... args) {
#if TC_DEBUG || TC_RELEASE
		_logger->trace(message, args...);
#endif
	}
	
	template<typename FormatString, typename ...Args>
	static void logAssert(const bool checkIfTrue, const FormatString& message, const Args&... args) {
#if TC_ENABLE_ASSERTS
		if(!checkIfTrue) {
			_logger->critical("ASSERTION FAILED!");
			_logger->error(message, args...);
#if PLATFORM_WINDOWS
			__debugbreak();
#elif PLATFORM_LINUX
			raise(SIGTRAP);
#endif
		}
#endif
	}

private:

	static std::shared_ptr<spdlog::logger> _logger;
};

#endif