#pragma once

#include <UserInterface/IUserInterface.h>

class DemoUI : public IUserInterface {
public:
	DemoUI() : IUserInterface("Demo UI") {};

private:
	bool show_demo_window = false;
	void Render() override {
		if(show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window); 

		ImGui::Begin("My Window");
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		if (ImGui::Button("Toggle Demo")) {
			show_demo_window = !show_demo_window;
		}
		ImGui::End();
	}
};