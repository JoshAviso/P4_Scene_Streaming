#include <Scenes/SceneManager.h>

#include <Logger.hpp>
#include <Objects/Object.h>
#include <Objects/ObjectManager.h>

#include <Resources/Mesh.h>
#include <Resources/ResourceManager.h>
#include <Components/Renderers/MeshRenderer.h>
#include <Graphics/ShaderList.h>

#include <Math/Random.h>

#include <algorithm>

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

List<Shared<Scene>> SceneManager::GetScenes()
{
    List<Shared<Scene>> list;
    list.reserve(_instance->_scenes.size());

    std::transform(_instance->_scenes.begin(), _instance->_scenes.end(),
        std::back_inserter(list),
        [](const auto& pair) { return pair.second; });
    return list;
}

void SceneManager::PopulateScenes()
{
	// Manual definition of scenes for the server
    
    // Resource Loading
    //Shared<Mesh> bunnyMesh = ResourceManager::LoadFromFile<Mesh>("Bunny", "Assets/Models/bunny.obj");
    //Shared<Mesh> teapotMesh = ResourceManager::LoadFromFile<Mesh>("Teapot", "Assets/Models/teapot.obj");
    Shared<Shader> basicShader = ShaderList::GenerateShader("Basic Shader",
        ResourceManager::LoadFromFile<VertexShader>("Basic Vertex Shader", "Assets/Shaders/sample.vert"),
        ResourceManager::LoadFromFile<FragShader>("Basic Frag Shader", "Assets/Shaders/sample.frag")
    );
	
    PopulateRandomScene("Scene1");
    PopulateRandomScene("Scene2");
    PopulateRandomScene("Scene3");
    PopulateRandomScene("Scene4");
    PopulateRandomScene("Scene5");

    /*
	// SCENE 1
	Shared<Scene> s1 = AddScene(new Scene("Scene 1"));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> bunny = s1->AddObject(new Object("Bunny" + std::to_string(i * j)));
            MeshRenderer* bunnyRenderer = bunny->AddComponent(new MeshRenderer("Bunny", "Assets/Models/bunny.obj"));
            bunnyRenderer->Shader = basicShader;
            bunnyRenderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            bunny->transform.position = Vec3((i - 2) * 0.5f, 0.f, (j - 2) * 0.5f);
            bunny->transform.rotation = Quaternion((i + j) / 10.f * 360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> teapot = s1->AddObject(new Object("Teapot" + std::to_string(i * j)));
            MeshRenderer* tpRenderer = teapot->AddComponent(new MeshRenderer("Teapot", "Assets/Models/teapot.obj"));
            //tpRenderer->ActiveMesh = teapotMesh;
            tpRenderer->Shader = basicShader;
            tpRenderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            teapot->transform.position = Vec3((i - 2) * 0.5f, -0.5f, (j - 2) * 0.5f);
            teapot->transform.scale = Vec3(0.05f, 0.05f, 0.05f);
            teapot->transform.rotation = Quaternion((i + j) / 10.f * -360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> obj = s1->AddObject(new Object("Armadillo" + std::to_string(i * j)));
            MeshRenderer* renderer = obj->AddComponent(new MeshRenderer("Armadillo", "Assets/Models/armadillo.obj"));
            //tpRenderer->ActiveMesh = teapotMesh;
            renderer->Shader = basicShader;
            renderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            obj->transform.position = Vec3((i - 2) * 0.5f, -1.f, (j - 2) * 0.5f);
            obj->transform.scale = Vec3(0.07f, 0.07f, 0.07f);
            obj->transform.rotation = Quaternion((i + j) / 10.f * -360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> obj = s1->AddObject(new Object("Cruiser" + std::to_string(i * j)));
            MeshRenderer* renderer = obj->AddComponent(new MeshRenderer("Cruiser", "Assets/Models/cruiser.obj"));
            //tpRenderer->ActiveMesh = teapotMesh;
            renderer->Shader = basicShader;
            renderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            obj->transform.position = Vec3((i - 2) * 0.5f, -1.5f, (j - 2) * 0.5f);
            obj->transform.scale = Vec3(0.07f, 0.07f, 0.07f);
            obj->transform.rotation = Quaternion((i + j) / 10.f * -360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> obj = s1->AddObject(new Object("F16" + std::to_string(i * j)));
            MeshRenderer* renderer = obj->AddComponent(new MeshRenderer("F16", "Assets/Models/f-16.obj"));
            //tpRenderer->ActiveMesh = teapotMesh;
            renderer->Shader = basicShader;
            renderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            obj->transform.position = Vec3((i - 2) * 0.5f, -2.f, (j - 2) * 0.5f);
            obj->transform.scale = Vec3(0.07f, 0.07f, 0.07f);
            obj->transform.rotation = Quaternion((i + j) / 10.f * -360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> obj = s1->AddObject(new Object("Suzanne" + std::to_string(i * j)));
            MeshRenderer* renderer = obj->AddComponent(new MeshRenderer("Suzanne", "Assets/Models/suzanne.obj"));
            //tpRenderer->ActiveMesh = teapotMesh;
            renderer->Shader = basicShader;
            renderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            obj->transform.position = Vec3((i - 2) * 0.5f, -2.5f, (j - 2) * 0.5f);
            obj->transform.scale = Vec3(0.07f, 0.07f, 0.07f);
            obj->transform.rotation = Quaternion((i + j) / 10.f * -360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> obj = s1->AddObject(new Object("Tyra" + std::to_string(i * j)));
            MeshRenderer* renderer = obj->AddComponent(new MeshRenderer("Tyra", "Assets/Models/tyra.obj"));
            //tpRenderer->ActiveMesh = teapotMesh;
            renderer->Shader = basicShader;
            renderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            obj->transform.position = Vec3((i - 2) * 0.5f, -3.f, (j - 2) * 0.5f);
            obj->transform.scale = Vec3(0.07f, 0.07f, 0.07f);
            obj->transform.rotation = Quaternion((i + j) / 10.f * -360.f, Vec3(0.f, 1.f, 0.f));
        }
    }

    // SCENE 2
    Shared<Scene> s2 = AddScene(new Scene("Scene 2"));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> bunny = s2->AddObject(new Object("Bunny" + std::to_string(i * j)));
            MeshRenderer* bunnyRenderer = bunny->AddComponent(new MeshRenderer("Bunny", "Assets/Models/bunny.obj"));
            //bunnyRenderer->ActiveMesh = bunnyMesh;
            bunnyRenderer->Shader = basicShader;
            bunnyRenderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            bunny->transform.position = Vec3((i - 2) * 0.5f, 0.f, (j - 2) * 0.5f);
            bunny->transform.rotation = Quaternion((i + j) / 10.f * 360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Shared<Object> teapot = s2->AddObject(new Object("Teapot" + std::to_string(i * j)));
            MeshRenderer* tpRenderer = teapot->AddComponent(new MeshRenderer("Teapot", "Assets/Models/teapot.obj"));
            //tpRenderer->ActiveMesh = teapotMesh;
            tpRenderer->Shader = basicShader;
            tpRenderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            teapot->transform.position = Vec3((i - 2) * 0.5f, 0.5f, (j - 2) * 0.5f);
            teapot->transform.scale = Vec3(0.05f, 0.05f, 0.05f);
            teapot->transform.rotation = Quaternion((i + j) / 10.f * -360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    */
}

void SceneManager::PopulateRandomScene(String name)
{
    // Main Shader 
    Shared<Shader> basicShader = ShaderList::GetShader("Basic Shader");

    // List of mesh names 
    String mesh_names[] = { "Armadillo", "Bunny", "Cruiser", "Suzanne", "Teapot", "Tyra" };
    String mesh_files[] = { "armadillo.obj", "bunny.obj", "cruiser.obj", "suzanne.obj", "teapot.obj", "tyra.obj" };

    Shared<Scene> scene = AddScene(new Scene(name));
    int objCnt = Random::RandInt(1, 7);
    for (int i = 0; i < objCnt; i++) {
        Shared<Object> obj = scene->AddObject(new Object("Object" + std::to_string(i)));

        // Randomize mesh to use
        int meshChoice = Random::RandInt(0, 5); // Pick one of 6 meshes
        MeshRenderer* renderer = obj->AddComponent<MeshRenderer>(new MeshRenderer(mesh_names[meshChoice], "Assets/Models/" + mesh_files[meshChoice]));
        renderer->Shader = basicShader;
        
        // Randomize colors
        float r = Random::RandFloat(0.1f, 0.9f);
        float g = Random::RandFloat(0.1f, 0.9f);
        float b = Random::RandFloat(0.1f, 0.9f);
        renderer->base_color = Color(r, g, b);

        // Slight scale randomization, special scale factor for bunny, tyra, and cruiser
        float scale = Random::RandFloat(0.07f, 0.1f);
        if (mesh_names[meshChoice] == "Bunny" || mesh_names[meshChoice] == "Tyra" || mesh_names[meshChoice] == "Cruiser")
            scale = Random::RandFloat(0.8f, 1.1f);
        obj->transform.scale = Vec3(scale);
        
        // Randomize Rotation
        float rot = Random::RandFloat(0.f, 360.f);
        obj->transform.rotation = Quaternion(rot, Vec3(0.f, 1.f, 0.f));

        // Randomize Position
        float interval = 0.2f;
        float x = Random::RandInt(-10, 10) * interval;
        float y = Random::RandInt(-5, 5) * interval;
        float z = Random::RandInt(-10, 10) * interval;
        obj->transform.position = Vec3(x, y ,z);
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
