#ifndef IMGUI_CONSOLE_H
#define IMGUI_CONSOLE_H

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

	using ConsoleMessageBuffer = std::vector<ConsoleMessage>;

	enum Flags {
		AUTOSCROLL = 1
	};
	
public:

	using SpdlogLevel = spdlog::level::level_enum;

	static void init();
	static void shutdown();
	
	static void sink(ConsoleMessage&& message) {
		_consoleBuffer.emplace_back(message);
		if(_autoscroll) _flags |= AUTOSCROLL;
	}
	
	static void drawImGui(bool* show = nullptr);
	
	static void setLevel(const SpdlogLevel level)     { _consoleLevel = level; }
	static void setMessageLimit(const uint32_t limit) { _messageLimit = (int32_t)limit; _consoleBuffer.reserve(limit + 5); }
	static void enableAutoScroll(const bool scroll)   { _autoscroll   = scroll; }

private:

	static ConsoleMessageBuffer _consoleBuffer;
	
	static SpdlogLevel _consoleLevel;
	static int32_t     _messageLimit;
	static uint32_t    _flags;
	
	static bool        _autoscroll;
};

#endif
