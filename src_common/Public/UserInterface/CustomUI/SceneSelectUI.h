#pragma once

#include <UserInterface/IUserInterface.h>
#include <Scenes/SceneManager.h>

class SceneSelectUI : public IUserInterface {
public:
	SceneSelectUI() : IUserInterface("Scene Select") {};
private:
	Shared<Scene> _selectedScene = nullptr;
	bool loadAllSelected = false;
	bool showPopup = false;
	int hoveredButton = -1;
	bool openedSelected = false;
	bool hoverShowAll = false;
	float RenderSceneButton(Shared<Scene> scene, int id, ImVec2 buttonDim) {
		ImVec2 smallDim = ImVec2(buttonDim.x - 20, 20);

		// Extract scene info
		String name = scene->GetName();
		float progress = scene->SceneProgress();
		bool doneLoading = progress >= 1.f;
		if (doneLoading) scene->IsLoading = false;
		bool isLoading = scene->IsLoading;

		bool isSelected = _selectedScene == scene;
		bool isHovered = hoveredButton == id;

		// Draw button
		ImGui::PushID(id);
		ImGui::BeginGroup();
		ImVec2 min = ImGui::GetCursorPos();
		ImVec2 max = ImVec2(min.x + buttonDim.x, min.y + buttonDim.y);

		if (isSelected) ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);

		// Background Backing color
		ImDrawList* drawlist = ImGui::GetWindowDrawList();
		ImU32 bgColor = isSelected ?
			IM_COL32(100, 100, 100, 100) :
			isHovered ?
			IM_COL32(80, 100, 120, 240) :
			IM_COL32(60, 60, 80, 200);
		drawlist->AddRectFilled(min, max, bgColor);

		// X Button To Unload
		bool unloadClicked = false;
		bool unloadHover = false;
		if (doneLoading) {
			ImGui::SetCursorPos(ImVec2(max.x - 25, min.y + 5));
			unloadClicked = ImGui::Button("X", ImVec2(20, 20));
			unloadHover = ImGui::IsItemHovered();
		}

		//if (isSelected) ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);

		// Center text to be replaced by display image ???
		ImVec2 textSize = ImGui::CalcTextSize(name.c_str());
		ImVec2 textPos = ImVec2{
			min.x + (buttonDim.x - textSize.x) * 0.5f,
			min.y + (buttonDim.y - textSize.y) * 0.5f
		};
		ImGui::SetCursorPos(textPos);
		ImGui::Text(name.c_str());

		ImVec2 lowerBarPos = ImVec2{
			min.x + (buttonDim.x - smallDim.x) * 0.5f,
			max.y - (buttonDim.x - smallDim.x) * 0.5f - smallDim.y
		};

		bool reloadClicked = false;
		bool reloadHover = false;
		ImGui::SetCursorPos(lowerBarPos);
		if (isLoading || doneLoading)
			ImGui::ProgressBar(progress, smallDim, (std::to_string((int)(progress * 100.f)) + "%").c_str());
		else {
			reloadClicked = ImGui::Button("Reload##ActionBtn", smallDim);
			reloadHover = ImGui::IsItemHovered();
		}

		// Background Button
		ImGui::SetCursorPos(min);
		bool clicked = ImGui::InvisibleButton("##BigButton", buttonDim);
		bool bigButtonHover = ImGui::IsItemHovered();
		if (bigButtonHover) hoveredButton = id;
		else if (hoveredButton == id) hoveredButton = -1;

		if (isSelected) ImGui::PopStyleVar();

		// Hover Tooltip
		if (unloadHover)
			ImGui::SetTooltip("Unload Scene");
		else if (reloadHover);
		else if (bigButtonHover)
			isSelected ?
			(progress >= 1.f ? ImGui::SetTooltip("Scene is Open") :
				(isLoading ? ImGui::SetTooltip("Scene Loading Ongoing...") :
				ImGui::SetTooltip("Scene Unloaded"))) : 
			ImGui::SetTooltip("Open Scene");

		// Action Handling
		if (clicked) {
			_selectedScene = scene;
			loadAllSelected = false;
			if (progress >= 1.f) {
				openedSelected = true;
				SceneManager::OpenScene(name);
			}
			else SceneManager::CloseAllScenes();
		}
		if (unloadClicked) {
			SceneManager::CloseAllScenes();
			SceneManager::UnloadScene(name);
		}
		if (reloadClicked) SceneManager::LoadScene(name);

		ImGui::EndGroup();
		ImGui::PopID();
		return progress;
	}
	void RenderShowAllScenes(float loadProgress, ImVec2 startPos, ImVec2 buttonDim, ImVec2 barDim) {

		bool wasLoadAllSelected = loadAllSelected;
		float mult = 1.f;
		if (hoverShowAll) mult *= 0.8f;
		if (wasLoadAllSelected) mult *= 0.5f;
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, mult);
		ImGui::ProgressBar(loadProgress, barDim, "");
		String text = ("Show All (" + std::to_string((int)(loadProgress * 100.f)) + "%% Loaded)");
		ImVec2 textDim = ImGui::CalcTextSize(text.c_str());
		ImVec2 textPos = { (buttonDim.x - textDim.x) * 0.5f, (buttonDim.y - textDim.y) * 0.5f};
		ImGui::SetCursorPos(textPos);
		ImGui::Text(text.c_str());

		ImGui::SetCursorPos(ImVec2(0.f, 0.f));
		bool buttonPress = ImGui::InvisibleButton("InvisButton", buttonDim);
		hoverShowAll = ImGui::IsItemHovered();

		if (buttonPress) {
			if (!loadAllSelected) {
				loadAllSelected = true;
				_selectedScene = nullptr;
				if (loadProgress >= 1.f) {
					openedSelected = true;
					SceneManager::OpenAllScenes();
				}
				else {
					SceneManager::CloseAllScenes();
				}
			}
		}
		ImGui::PopStyleVar();
	}
	void UtilOverlay() {

	}

	void Render() override {
		float runningLoad = 0.f;
		List<Shared<Scene>> scenes = SceneManager::GetScenes();
		
		// Scene Select Window
		static const ImVec2 sceneSelectButtonDim = ImVec2(150, 120);
		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(vp->Pos.x, vp->Pos.y));
		ImGui::SetNextWindowSize(ImVec2(vp->Size.x, sceneSelectButtonDim.y + 40));
		if (
			ImGui::Begin("Select Scene", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings
			)
		) {
			float fps = ImGui::GetIO().Framerate;
			ImGui::Text("FPS: %.f", fps);

			// Draw buttons
			for (int i = 0; i < scenes.size(); i++) {
				Shared<Scene> scene = scenes[i];
				runningLoad += RenderSceneButton(scene, i, sceneSelectButtonDim);
				if(i < scenes.size() - 1) ImGui::SameLine();
			}
			ImGui::End();
		}

		float totalLoad = runningLoad / (float)scenes.size();

		// Load All Button
		static const ImVec2 loadAllButtonDim = ImVec2(160, 40);
		static const ImVec2 loadAllWindowDim = ImVec2(loadAllButtonDim.x + 16, loadAllButtonDim.y + 16);
		ImVec2 vpMax = ImVec2(vp->Pos.x + vp->Size.x, vp->Pos.y + vp->Size.y);
		static const ImVec2 loadAllWindowStartPos = ImVec2(vpMax.x - loadAllWindowDim.x, vpMax.y - loadAllWindowDim.y);
		static const ImVec2 loadAllStartPos = ImVec2(vpMax.x - loadAllButtonDim.x - 8, vpMax.y - loadAllButtonDim.y - 8);
		ImGui::SetNextWindowPos(loadAllWindowStartPos);
		ImGui::SetNextWindowSize(loadAllWindowDim);
		if (
			ImGui::Begin("Load All Window", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings
		)
		) {
			

			RenderShowAllScenes(totalLoad, loadAllStartPos, loadAllWindowDim, loadAllButtonDim);

			ImGui::End();
		}

		// Popup 
		static const ImVec2 popupDim = ImVec2(160, 40);
		static const ImVec2 popupWindowDim = ImVec2(popupDim.x + 16, popupDim.y + 16);
		ImVec2 vpMid = ImVec2(vp->Pos.x + vp->Size.x * 0.5f, vp->Pos.y + vp->Size.y * 0.5f);
		
		// Decide if popup is appropriate
		float progress = 2.f;
		if (_selectedScene != nullptr) progress = _selectedScene->SceneProgress();
		else if (loadAllSelected) progress = totalLoad;
		bool showPopup = progress < 1.f;

		if (showPopup) {
			ImGui::SetNextWindowPos(ImVec2(vpMid.x - popupWindowDim.x * 0.5f, vpMid.y - popupWindowDim.y * 0.5f));
			ImGui::SetNextWindowSize(popupWindowDim);
			if (
				ImGui::Begin("Popup Window", nullptr,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings
				)
				) {
				openedSelected = false;

				// Draw Popup
				ImGui::ProgressBar(progress, popupDim, "");
				
				String popupText = _selectedScene != nullptr ? _selectedScene->GetName() : "All Scenes";
				popupText = popupText + " " + std::to_string((int)(progress * 100.f)) + "%% Loaded";

				// Calculate Text Dims
				ImVec2 textSize = ImGui::CalcTextSize(popupText.c_str());
				ImVec2 textPos = ImVec2{
					(popupWindowDim.x - textSize.x) * 0.5f,
					(popupWindowDim.y - textSize.y) * 0.5f
				};
				ImGui::SetCursorPos(textPos);
				ImGui::Text(popupText.c_str());
				
				ImGui::End();
			}
		}

		if (!openedSelected && progress >= 1.f) {
			openedSelected = true;

			if (loadAllSelected) SceneManager::OpenAllScenes();
			if (_selectedScene != nullptr) SceneManager::OpenScene(_selectedScene->GetName());
		}
	}
};