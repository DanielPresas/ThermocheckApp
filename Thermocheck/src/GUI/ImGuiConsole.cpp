#include "tcpch.h"
#include "GUI/ImGuiConsole.h"

#include <imgui/imgui.h>

std::vector<ConsoleMessage> ImGuiConsole::_consoleBuffer;

ImGuiConsole::SpdlogLevel ImGuiConsole::_consoleLevel = ImGuiConsole::SpdlogLevel::trace;
uint32_t                  ImGuiConsole::_messageLimit = 100;
uint32_t                  ImGuiConsole::_flags        = 0;
bool                      ImGuiConsole::_autoscroll   = true;

static ImVec4 getMessageColor(const ImGuiConsole::SpdlogLevel level) {
	
	switch(level) {
		case ImGuiConsole::SpdlogLevel::trace:       return { 0.75f, 0.75f, 0.75f, 1.0f }; // Light gray
		case ImGuiConsole::SpdlogLevel::debug:       return { 0.0f,  1.0f,  1.0f,  1.0f }; // Cyan
		case ImGuiConsole::SpdlogLevel::info:        return { 0.0f,  1.0f,  0.0f,  1.0f }; // Green
		case ImGuiConsole::SpdlogLevel::warn:        return { 1.0f,  1.0f,  0.0f,  1.0f }; // Yellow
		case ImGuiConsole::SpdlogLevel::err:         return { 0.7f,  0.2f,  0.3f,  1.0f }; // Dark red
		case ImGuiConsole::SpdlogLevel::critical:    return { 1.0f,  0.0f,  0.0f,  1.0f }; // Bright red
		default:                                     return { 1.0f,  1.0f,  1.0f,  1.0f }; // White
	}
}

static const char* getLevelName(const spdlog::level::level_enum level) {
	using SpdlogLevel = spdlog::level::level_enum;
	
	switch(level) {
		case SpdlogLevel::trace:       return "Trace";
		case SpdlogLevel::debug:       return "Debug";
		case SpdlogLevel::info:        return "Info";
		case SpdlogLevel::warn:        return "Warning";
		case SpdlogLevel::err:         return "Error";
		case SpdlogLevel::critical:    return "Critical";
		default:                       return "Off";
	}
}

void ImGuiConsole::init() {
	_consoleBuffer.reserve(_messageLimit + 5);
}

void ImGuiConsole::shutdown() {
	_consoleBuffer.clear();
}

void ImGuiConsole::drawImGui(bool* show) {

	while(_consoleBuffer.size() > _messageLimit) {
		_consoleBuffer.erase(_consoleBuffer.begin());
	}

	ImGui::Begin("Log Console", show);
	{
		// ------------------------------
		// ----- Header -----------------
		// ------------------------------
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Display Level"); ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::CalcTextSize("Critical").x * 1.5f);
			if(ImGui::BeginCombo("##Message Level Filter", getLevelName(_consoleLevel)/*, ImGuiComboFlags_NoArrowButton*/))
			{
				for(uint8_t i = 0; i < 7; ++i) {
					const bool isSelected = (_consoleLevel == static_cast<SpdlogLevel>(i));//(i == index);
					if(ImGui::Selectable(getLevelName(static_cast<SpdlogLevel>(i)), isSelected)) {
						_consoleLevel = static_cast<SpdlogLevel>(i);
					}

					if(isSelected) ImGui::SetItemDefaultFocus();
				}
			
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::Checkbox("Enable Autoscroll", &_autoscroll);
		}
		
		// ------------------------------
		// ----- Scroll region ----------
		// ------------------------------
		{
			const ImGuiWindowFlags flags =
				ImGuiWindowFlags_HorizontalScrollbar;

			ImGui::BeginChild("Scroll Region", { 0, 0 }, true, flags);
			{

				for(const auto& message : _consoleBuffer) {
					if(message.getLevel() >= _consoleLevel) {
						ImGui::PushStyleColor(ImGuiCol_Text, getMessageColor(message.getLevel()));
						ImGui::TextUnformatted(message.getMessage().c_str());
						ImGui::PopStyleColor();
					}
				}

				if(_flags & AUTOSCROLL && ImGui::GetScrollMaxY() > 0) {
					ImGui::SetScrollY(ImGui::GetScrollMaxY());
					_flags &= ~AUTOSCROLL;
				}
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();
}
