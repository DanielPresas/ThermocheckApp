#ifndef IMGUI_CONSOLE_H
#define IMGUI_CONSOLE_H

class ImGuiConsole {
	using SpdlogLevel = spdlog::level::level_enum;
	
public:

	static void sink(std::string&& msg, SpdlogLevel level);
	static void flush();

private:

	
	
};

#endif