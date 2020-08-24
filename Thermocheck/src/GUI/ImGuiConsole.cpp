#include "tcpch.h"
#include "GUI/ImGuiConsole.h"

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

void ImGuiConsole::drawImGui() {

	const ImGuiWindowFlags flags =
		ImGuiWindowFlags_HorizontalScrollbar;
	
	ImGui::BeginChild("Scroll Region", { 0, 0 }, true, flags);
	{
		if(_flushConsole) {
			for(const auto& message : _consoleBuffer) {
				ImGui::PushStyleColor(ImGuiCol_Text, getMessageColor(message.getLevel()));
				ImGui::TextUnformatted(message.getMessage().c_str());
				ImGui::PopStyleColor();
			}
		}
	}
	ImGui::EndChild();
}
