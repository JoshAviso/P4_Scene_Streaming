#pragma once

#include <UserInterface/IUserInterface.h>
#include <Scenes/SceneManager.h>

class SceneSelectUI : public IUserInterface {
public:
	SceneSelectUI() : IUserInterface("Scene Select") {};
private:
	void Render() override {
		if (ImGui::Begin("Select Scene")) {
			List<Shared<Scene>> scenes = SceneManager::GetScenes();
			for (int i = 0; i < scenes.size(); i++) {
				Shared<Scene> scene = scenes[i];
				if (ImGui::Button(scene->GetName().c_str())) {
					SceneManager::OpenScene(scene->GetName());
				}
			}

			ImGui::End();
		}
	}
};