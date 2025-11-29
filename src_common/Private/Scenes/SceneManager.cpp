#include <Scenes/SceneManager.h>

#include <Logger.hpp>
#include <Objects/Object.h>
#include <Objects/ObjectManager.h>

void SceneManager::OpenScene(const String sceneName)
{
	if (_instance->_scenes.find(sceneName) == _instance->_scenes.end()) {
		Logger::LogWarning("Trying to open a non-existent scene: " + sceneName);
		return;
	}

	ObjectManager::ClearObjects();
	_instance->_scenes[sceneName]->Open();
}

Shared<Scene> SceneManager::AddScene(Scene* scene)
{
	if (scene == nullptr) return nullptr;
	String name = scene->SceneName;
	auto& scenes = _instance->_scenes;
	if (scenes.find(name) != scenes.end()) 
		return _instance->_scenes[name];
	Shared<Scene> sc = Shared<Scene>(scene);
	_instance->_scenes[name] = sc;
	return sc;
}

void SceneManager::PopulateScenes()
{
	// Manual definition of scenes for the server
	
	// SCENE 1
	Shared<Scene> s1 = AddScene(new Scene("Scene 1"));
	Shared<Object> s1o1 = s1->AddObject(new Object("Item 1"));
}

// SINGLETON
SceneManager* SceneManager::_instance = nullptr;
SceneManager* SceneManager::Initialize(const Desc& desc)
{
	if (_instance == nullptr)
		_instance = new SceneManager(desc);

	if (_instance->_isServer)
		_instance->PopulateScenes();

	return _instance;
}
SceneManager::SceneManager(const Desc& desc) : _isServer(desc.isServer), _camName(desc.camName) {
}

SceneManager::~SceneManager()
{
}
