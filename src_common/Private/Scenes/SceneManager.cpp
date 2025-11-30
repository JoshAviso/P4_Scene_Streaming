#include <Scenes/SceneManager.h>

#include <Logger.hpp>
#include <Objects/Object.h>
#include <Objects/ObjectManager.h>

#include <Resources/Mesh.h>
#include <Resources/ResourceManager.h>
#include <Components/Renderers/MeshRenderer.h>
#include <Graphics/ShaderList.h>

void SceneManager::OpenScene(const String sceneName)
{
	if (_instance->_scenes.find(sceneName) == _instance->_scenes.end()) {
		Logger::LogWarning("Trying to open a non-existent scene: " + sceneName);
		return;
	}

    Shared<Object> camera = nullptr;
    if(!IsEmptyOrWhitespace(_instance->_camName))
         camera = ObjectManager::FindObjectByName(_instance->_camName);
	ObjectManager::ClearObjects();
    if (camera != nullptr)
        ObjectManager::RegisterObject(camera);
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
	//Shared<Object> s1o1 = s1->AddObject(new Object("Item 1"));

    Shared<Mesh> bunnyMesh = ResourceManager::LoadFromFile<Mesh>("Bunny", "Assets/Models/bunny.obj");
    Shared<Mesh> teapotMesh = ResourceManager::LoadFromFile<Mesh>("Teapot", "Assets/Models/teapot.obj");
    Shared<Shader> basicShader = ShaderList::GenerateShader("Basic Shader",
        ResourceManager::LoadFromFile<VertexShader>("Basic Vertex Shader", "Assets/Shaders/sample.vert"),
        ResourceManager::LoadFromFile<FragShader>("Basic Frag Shader", "Assets/Shaders/sample.frag")
    );

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> bunny = s1->AddObject(new Object("Bunny" + std::to_string(i * j)));
            MeshRenderer* bunnyRenderer = bunny->AddComponent(new MeshRenderer());
            bunnyRenderer->Mesh = bunnyMesh;
            bunnyRenderer->Shader = basicShader;
            bunnyRenderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            bunny->transform.position = Vec3((i - 2) * 0.5f, 0.f, (j - 2) * 0.5f);
            bunny->transform.rotation = Quaternion((i + j) / 10.f * 360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> teapot = s1->AddObject(new Object("Teapot" + std::to_string(i * j)));
            MeshRenderer* tpRenderer = teapot->AddComponent(new MeshRenderer());
            tpRenderer->Mesh = teapotMesh;
            tpRenderer->Shader = basicShader;
            tpRenderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            teapot->transform.position = Vec3((i - 2) * 0.5f, -0.5f, (j - 2) * 0.5f);
            teapot->transform.scale = Vec3(0.05f, 0.05f, 0.05f);
            teapot->transform.rotation = Quaternion((i + j) / 10.f * -360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
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
