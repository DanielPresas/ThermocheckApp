#ifndef IMGUI_CONSOLE_H
#define IMGUI_CONSOLE_H

#include <queue>

class ConsoleMessage {

	using SpdlogLevel = spdlog::level::level_enum;
	
public:

	ConsoleMessage(std::string&& message, const SpdlogLevel level) :
		_message(std::move(message)), _level(level) {}

	const std::string& getMessage() const { return _message; }
	SpdlogLevel getLevel()          const { return _level; }
	
	static ConsoleMessage emptyMessage() { return { "", SpdlogLevel::off }; }
	
private:

	std::string _message;
	SpdlogLevel _level = SpdlogLevel::off;
	
};

class ImGuiConsole {

	using SpdlogLevel = spdlog::level::level_enum;

	enum Flags : uint32_t {
		AUTOSCROLL = 1 << 0,
	};
	
public:

	static void init();
	static void shutdown();
	
	static void sink(ConsoleMessage&& message) {
		_consoleBuffer.emplace_back(message);
		_flags |= AUTOSCROLL;
	}
	
	static void drawImGui();
	
	static void setLevel(const SpdlogLevel level) { _consoleLevel = level;  }
	
	//static void setAutoScroll(const bool scroll) { if(scroll) _flags |= AUTOSCROLL; else _flags &= ~AUTOSCROLL; }

private:

	static std::vector<ConsoleMessage> _consoleBuffer;
	
	static inline SpdlogLevel _consoleLevel = SpdlogLevel::trace;
	static inline uint32_t _flags;
};

#endif