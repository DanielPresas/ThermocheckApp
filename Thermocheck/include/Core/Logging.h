#ifndef LOGGING_H
#define LOGGING_H

#if PLATFORM_LINUX
#	include <signal.h>
#endif

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Logger {
public:

	static void init();
	static void shutdown();
	
	template<typename FormatString, typename ...Args>
	static void critical(const FormatString& message, const Args&... args) {
		_logger->critical(message, args...);
	}
	
	template<typename FormatString, typename ...Args>
	static void error(const FormatString& message, const Args&... args) {
		_logger->error(message, args...);
	}
	
	template<typename FormatString, typename ...Args>
	static void warning(const FormatString& message, const Args&... args) {
		_logger->warn(message, args...);
	}
	
	template<typename FormatString, typename ...Args>
	static void info(const FormatString& message, const Args&... args) {
		_logger->info(message, args...);
	}

	template<typename FormatString, typename ...Args>
	static void debug(const FormatString& message, const Args&... args) {
#if TC_DEBUG || TC_RELEASE
		_logger->debug(message, args...);
#endif
	}
	
	template<typename FormatString, typename ...Args>
	static void trace(const FormatString& message, const Args&... args) {
		_logger->trace(message, args...);
	}
	
	template<typename FormatString, typename ...Args>
	static void logAssert(const bool checkIfTrue, const FormatString& message, const Args&... args) {
#if TC_ENABLE_ASSERTS
		if(!checkIfTrue) {
			_logger->critical("ASSERTION FAILED!");
			_logger->error(message, args...);
#	if PLATFORM_WINDOWS
			__debugbreak();
#	elif PLATFORM_LINUX
			raise(SIGTRAP);
#	endif
		}
#endif
	}

private:

	static std::shared_ptr<spdlog::logger> _logger;
};

#endif