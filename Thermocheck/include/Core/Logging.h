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
	static bool logAssert(const bool checkIfTrue, const char* file, const int line, const FormatString& message, const Args&... args) {
		if(!checkIfTrue) {
			_logger->critical("ASSERTION FAILED!");
			_logger->error("FILE: {}", file);
			_logger->error("LINE: {}", line);
			_logger->error(message, args...);
			
#	if PLATFORM_WINDOWS
			__debugbreak();
#	elif PLATFORM_LINUX
			raise(SIGTRAP);
#	endif

		}

		return checkIfTrue;
	}

private:

	static std::shared_ptr<spdlog::logger> _logger;
};


#define TC_LOG_TRACE(message, ...)       Logger::trace   (message, ##__VA_ARGS__)
#define TC_LOG_DEBUG(message, ...)       Logger::debug   (message, ##__VA_ARGS__)
#define TC_LOG_INFO(message, ...)        Logger::info    (message, ##__VA_ARGS__)
#define TC_LOG_WARNING(message, ...)     Logger::warning (message, ##__VA_ARGS__)
#define TC_LOG_ERROR(message, ...)       Logger::error   (message, ##__VA_ARGS__)
#define TC_LOG_CRITICAL(message, ...)    Logger::critical(message, ##__VA_ARGS__)


#if TC_ENABLE_ASSERTS
#	define TC_ASSERT(checkIfTrue, message, ...) Logger::logAssert(checkIfTrue, __FILE__, __LINE__, message, ##__VA_ARGS__)
#else
#	define TC_ASSERT(checkIfTrue, message, ...) checkIfTrue
#endif

#endif