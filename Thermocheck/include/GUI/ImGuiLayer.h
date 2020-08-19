#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

class ImGuiLayer {
public:

	static void init();
	static void shutdown();

	static void begin();
	static void end();
	static void drawImGui();
	
};

#endif