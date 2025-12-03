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

			ImVec2 fullButtonDim = ImVec2(ImGui::GetContentRegionAvail().x, 0.f);
			ImVec2 mainButtonDim = ImVec2(ImGui::GetContentRegionAvail().x - 85.f, 0.f);
			ImVec2 loadButtonDim = ImVec2(80.f, 0.f);

			float runningLoad = 0.f;
			for (int i = 0; i < scenes.size(); i++) {
				Shared<Scene> scene = scenes[i];
				float load = scene->SceneProgress();
				runningLoad += load;

				bool selected = scene == _selectedScene;
				if (selected)
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);

				if (ImGui::Button((scene->GetName() + " (" + std::to_string((int)(load * 100.f)) + "% Loaded)").c_str(), mainButtonDim)) {
					_selectedScene = scene;
					loadAllSelected = false;
					SceneManager::OpenScene(scene->GetName());
				}

				if (selected)
					ImGui::PopStyleVar();

				bool sceneLoaded = scene->SceneProgress() >= 1.f;
				if (sceneLoaded) scene->IsLoading = false;
				bool sceneLoading = scene->IsLoading;
				String loadButtonText = sceneLoaded ? "Unload##" + scene->GetName() : sceneLoading ? "(Loading)##" + scene->GetName() : "Load##" + scene->GetName();
				ImGui::SameLine();
				if(sceneLoading)
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
				if (ImGui::Button(loadButtonText.c_str(), loadButtonDim)) {
					if (!sceneLoading) {
						if (sceneLoaded) SceneManager::UnloadScene(scene->GetName());
						else SceneManager::LoadScene(scene->GetName());
					}
				}
				if (sceneLoading)
					ImGui::PopStyleVar();
			}

			float totalLoad = runningLoad / (float)scenes.size();

			bool wasLoadAllSelected = loadAllSelected;
			if (wasLoadAllSelected) ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
			if (ImGui::Button(("Show All (" + std::to_string((int)(totalLoad * 100.f)) + "% Loaded)").c_str(), fullButtonDim)) {
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