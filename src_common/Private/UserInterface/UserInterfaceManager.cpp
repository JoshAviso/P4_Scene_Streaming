#include <UserInterface/UserInterfaceManager.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Shared<IUserInterface> UserInterfaceManager::AddUI(IUserInterface* ui)
{
	String name = ui->Name;
	int cnt = 0;
	while (GetUI(name) != nullptr) {
		cnt++;
		name = ui->Name + std::to_string(cnt);
	}
	ui->Name = name;
	Shared<IUserInterface> sharedUI = Shared<IUserInterface>(ui);
	_instance->_userInterfaces.push_back(sharedUI);
	return sharedUI;
}

Shared<IUserInterface> UserInterfaceManager::GetUI(const String name)
{
	for (int i = 0; i < _instance->_userInterfaces.size(); i++) {
		if (_instance->_userInterfaces[i]->Name == name) {
			return _instance->_userInterfaces[i];
		}
	}
	return nullptr;
}

void UserInterfaceManager::RemoveUI(const String name)
{
	auto it = std::find_if(_instance->_userInterfaces.begin(), _instance->_userInterfaces.end(),
		[&](const Shared<IUserInterface>& ui) { return ui->Name == name; });

	if (it != _instance->_userInterfaces.end())
	{
		_instance->_userInterfaces.erase(it);
		return;
	}
}

void UserInterfaceManager::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UserInterfaceManager::Render()
{
	for (int i = 0; i < _instance->_userInterfaces.size(); i++) {
		_instance->_userInterfaces[i]->Render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UserInterfaceManager* UserInterfaceManager::_instance = nullptr;
UserInterfaceManager* UserInterfaceManager::Initialize(const Desc desc) {
	if (_instance == nullptr)
		_instance = new UserInterfaceManager(desc);

	return _instance;
}

UserInterfaceManager::UserInterfaceManager(const Desc& desc) {

}

UserInterfaceManager::~UserInterfaceManager()
{
}
