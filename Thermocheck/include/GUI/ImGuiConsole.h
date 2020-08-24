#ifndef IMGUI_CONSOLE_H
#define IMGUI_CONSOLE_H

#include <imgui.h>

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
	
public:
	
	static void sink(ConsoleMessage&& message) { _consoleBuffer.emplace_back(message); }
	static void flush() { _flushConsole = true; }
	
	static void drawImGui();
	
	static void setLevel(const SpdlogLevel level) { _consoleLevel = level; }

private:

	static inline std::vector<ConsoleMessage> _consoleBuffer;
	static inline SpdlogLevel _consoleLevel = SpdlogLevel::trace;
	static inline bool _flushConsole = false;
	
};

#endif