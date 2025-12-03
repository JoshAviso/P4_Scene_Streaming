#pragma once

#include <UserInterface/IUserInterface.h>
#include <Scenes/SceneManager.h>

class SceneSelectUI : public IUserInterface {
public:
	SceneSelectUI() : IUserInterface("Scene Select") {};
private:
	Shared<Scene> _selectedScene = nullptr;
	bool loadAllSelected = false;
	void Render() override {
		if (ImGui::Begin("Select Scene")) {
			List<Shared<Scene>> scenes = SceneManager::GetScenes();

			ImVec2 button_dim = ImVec2(ImGui::GetContentRegionAvail().x, 0.f);

			for (int i = 0; i < scenes.size(); i++) {
				Shared<Scene> scene = scenes[i];
				bool selected = scene == _selectedScene;
				if (selected)
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);

				if (ImGui::Button(scene->GetName().c_str(), button_dim)) {
					_selectedScene = scene;
					loadAllSelected = false;
					SceneManager::OpenScene(scene->GetName());
				}

				if (selected)
					ImGui::PopStyleVar();
			}

			bool wasLoadAllSelected = loadAllSelected;
			if (wasLoadAllSelected) ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
			if (ImGui::Button("Load All", button_dim)) {
				if (!loadAllSelected) {
					loadAllSelected = true;
					_selectedScene = nullptr;
					SceneManager::OpenAllScenes();
				}
			}
			if (wasLoadAllSelected) ImGui::PopStyleVar();

			ImGui::End();
		}
	}
};