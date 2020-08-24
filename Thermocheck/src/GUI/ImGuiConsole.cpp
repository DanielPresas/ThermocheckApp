#include "tcpch.h"
#include "GUI/ImGuiConsole.h"

#include <imgui/imgui.h>

std::vector<ConsoleMessage> ImGuiConsole::_consoleBuffer;

static ImVec4 getMessageColor(const spdlog::level::level_enum level) {
	using SpdlogLevel = spdlog::level::level_enum;
	
	switch(level) {
		case SpdlogLevel::trace:       return { 0.75f, 0.75f, 0.75f, 1.0f }; // Light gray
		case SpdlogLevel::info:        return { 0.0f,  1.0f,  0.0f,  1.0f }; // Green
		case SpdlogLevel::debug:       return { 0.0f,  1.0f,  1.0f,  1.0f }; // Cyan
		case SpdlogLevel::warn:        return { 1.0f,  1.0f,  0.0f,  1.0f }; // Yellow
		case SpdlogLevel::err:         return { 0.7f,  0.2f,  0.3f,  1.0f }; // Dark red
		case SpdlogLevel::critical:    return { 1.0f,  0.0f,  0.0f,  1.0f }; // Bright red
		default:                       return { 1.0f,  1.0f,  1.0f,  1.0f }; // White
	}
}

void ImGuiConsole::init() {
	_consoleBuffer.reserve(110);
}

void ImGuiConsole::shutdown() {
	_consoleBuffer.clear();
}

void ImGuiConsole::drawImGui() {

	while(_consoleBuffer.size() > 100) {
		_consoleBuffer.erase(_consoleBuffer.begin());
	}

	const ImGuiWindowFlags flags =
		ImGuiWindowFlags_HorizontalScrollbar;
	
	ImGui::BeginChild("Scroll Region", { 0, 0 }, true, flags);
	{
		
		for(const auto& message : _consoleBuffer) {
			ImGui::PushStyleColor(ImGuiCol_Text, getMessageColor(message.getLevel()));
			ImGui::TextUnformatted(message.getMessage().c_str());
			ImGui::PopStyleColor();
		}

		if(_flags & AUTOSCROLL && ImGui::GetScrollMaxY() > 0) {
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
			_flags &= ~AUTOSCROLL;
		}
	}
	ImGui::EndChild();
}
