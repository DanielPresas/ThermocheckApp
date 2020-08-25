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

	using SpdlogLevel = spdlog::level::level_enum;
	using ConsoleMessageBuffer = std::vector<ConsoleMessage>;

	enum Flags {
		AUTOSCROLL = 1
	};
	
public:

	static void init();
	static void shutdown();
	
	static void sink(ConsoleMessage&& message) {
		if (message.getLevel() >= _consoleLevel){
			_consoleBuffer.emplace_back(message);
			if(_autoscroll) _flags |= AUTOSCROLL;
		}
	}
	
	static void drawImGui(bool* show = nullptr);
	
	static void setLevel(const SpdlogLevel level)     { _consoleLevel = level; }
	static void setMessageLimit(const uint32_t limit) { _messageLimit = limit; _consoleBuffer.reserve(limit + 5); }
	static void enableAutoScroll(const bool scroll)   { _autoscroll   = scroll; }

private:

	static ConsoleMessageBuffer _consoleBuffer;
	
	static inline SpdlogLevel _consoleLevel = SpdlogLevel::trace;
	static inline uint32_t    _messageLimit = 100;
	static inline bool        _autoscroll   = true;
	
	static inline uint32_t _flags;
};

#endif