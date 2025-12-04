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

#include <SceneStreaming.grpc.pb.h>

#include <Threading/ThreadPoolManager.h>
#include <Threading/WorkerTasks/LoadObjectsFromServerTask.h>

void SceneManager::LoadScene(const String sceneName)
{
    _instance->scenesMtx.lock();
    if (_instance->_scenes.find(sceneName) == _instance->_scenes.end()) {
        _instance->scenesMtx.unlock();

        Logger::LogWarning("Trying to load a non-existent scene: " + sceneName);
        return;
    }

    Shared<Scene> scene = _instance->_scenes[sceneName];
    _instance->scenesMtx.unlock();
    
    scene->IsLoading = true;
    
    if (_instance->_isServer) {
        // On the server side, have meshes begin to load from files
        for (auto& obj : scene->_objects) {
            List<MeshRenderer*> meshes = obj->GetComponents<MeshRenderer>();
            for (auto& mesh : meshes) {
                mesh->BeginLoad();
            }
        }
    }
    else {
        _instance->ProcessStreamedScene(sceneName);
    }

}

void SceneManager::UnloadScene(const String sceneName)
{
    _instance->scenesMtx.lock();
    if (_instance->_scenes.find(sceneName) == _instance->_scenes.end()) {
        _instance->scenesMtx.unlock();
        Logger::LogWarning("Trying to unload a non-existent scene: " + sceneName);
        return;
    }

    Shared<Scene> scene = _instance->_scenes[sceneName];
    _instance->scenesMtx.unlock();

    for (auto& obj : scene->_objects) {
        List<MeshRenderer*> meshes = obj->GetComponents<MeshRenderer>();
        for (auto& mesh : meshes) {
            mesh->Unload();
        }
    }
    scene->SceneCompletion = 0;
}

void SceneManager::OpenScene(const String sceneName)
{
    _instance->scenesMtx.lock();
	if (_instance->_scenes.find(sceneName) == _instance->_scenes.end()) {
        _instance->scenesMtx.unlock();
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
    _instance->scenesMtx.unlock();
}

void SceneManager::CloseAllScenes()
{
    _instance->scenesMtx.lock();
    Shared<Object> camera = nullptr;
    if (!IsEmptyOrWhitespace(_instance->_camName))
        camera = ObjectManager::FindObjectByName(_instance->_camName);
    ObjectManager::ClearObjects();
    if (camera != nullptr)
        ObjectManager::RegisterObject(camera); 
    _instance->scenesMtx.unlock();
}


Shared<Scene> SceneManager::AddScene(Scene* scene)
{
	if (scene == nullptr) return nullptr;
	String name = scene->SceneName;

    _instance->scenesMtx.lock();
	auto& scenes = _instance->_scenes;
    if (scenes.find(name) != scenes.end()) {
        _instance->scenesMtx.unlock();
		return _instance->_scenes[name];
    }
	Shared<Scene> sc = Shared<Scene>(scene);
	_instance->_scenes[name] = sc;
    _instance->scenesMtx.unlock();

	return sc;
}

Shared<Scene> SceneManager::GetScene(const String sceneName)
{
    _instance->scenesMtx.lock();
    if (_instance->_scenes.find(sceneName) == _instance->_scenes.end()) {
        _instance->scenesMtx.unlock();

        Logger::LogWarning("Trying to get a non-existent scene: " + sceneName);
        return nullptr;
    }
    
    Shared<Scene> scene = _instance->_scenes[sceneName];
    _instance->scenesMtx.unlock();
    return scene;
}

List<Shared<Scene>> SceneManager::GetScenes()
{
    _instance->scenesMtx.lock();

    List<Shared<Scene>> list;
    list.reserve(_instance->_scenes.size());

    std::transform(_instance->_scenes.begin(), _instance->_scenes.end(),
        std::back_inserter(list),
        [](const auto& pair) { return pair.second; });
    _instance->scenesMtx.unlock();

    return list;
}

void SceneManager::OpenAllScenes()
{
    _instance->scenesMtx.lock();
    Shared<Object> camera = nullptr;
    if (!IsEmptyOrWhitespace(_instance->_camName))
        camera = ObjectManager::FindObjectByName(_instance->_camName);
    ObjectManager::ClearObjects();
    if (camera != nullptr)
        ObjectManager::RegisterObject(camera);

    for (const auto& [key, scene] : _instance->_scenes) {
        scene->Open();
    }
    _instance->scenesMtx.unlock();
}

void SceneManager::PopulateScenes()
{
    Shared<Shader> basicShader = ShaderList::GenerateShader("Basic Shader",
        ResourceManager::LoadFromFile<VertexShader>("Basic Vertex Shader", "Assets/Shaders/sample.vert"),
        ResourceManager::LoadFromFile<FragShader>("Basic Frag Shader", "Assets/Shaders/sample.frag")
    );
	
    PopulateRandomScene("Scene1");
    PopulateRandomScene("Scene2");
    PopulateRandomScene("Scene3");
    PopulateRandomScene("Scene4");
    PopulateRandomScene("Scene5");
}

void SceneManager::PopulateRandomScene(String name)
{
    // Main Shader 
    Shared<Shader> basicShader = ShaderList::GetShader("Basic Shader");

    // List of mesh names, files, and scaling ranges
    struct mesh_info_composite { String file; Vec2 scale; };
    static const List<mesh_info_composite> meshes = {
        {"armadillo.obj", {0.07f, 0.1f}},
        {"beast.obj", {0.003f, 0.007f}},
        {"beetle.obj", {0.8f, 1.1f}},
        {"bimba.obj", {0.8f, 1.1f}},
        {"bunny.obj", {0.8f, 1.1f}},
        {"cheburashka.obj", {0.07f, 0.1f}},
        {"cow.obj", {0.07f, 0.1f}},
        {"cruiser.obj", {0.4f, 0.6f}},
        {"fandisk.obj", {0.07f, 0.1f}},
        {"happy.obj", {0.8f, 1.1f}},
        {"homer.obj", {0.8f, 1.1f}},
        {"horse.obj", {1.2f, 1.5f}},
        {"igea.obj", {0.8f, 1.1f}},
        {"lucy.obj", {0.0005f, 0.0008f}},
        {"max-planck.obj", {0.0005f, 0.0008f}},
        {"nefertiti.obj", {0.0005f, 0.0008f}},
        {"ogre.obj", {0.01f, 0.03f}},
        {"rocker-arm.obj", {0.8f, 1.1f}},
        {"spot.obj", {0.07f, 0.1f}},
        {"suzanne.obj", {0.07f, 0.1f}},
        {"teapot.obj", {0.07f, 0.1f}},
        {"tyra.obj", {0.4f, 0.6f}},
        {"xyzrgb_dragon.obj", {0.005f, 0.01f}},
    };

    Shared<Scene> scene = AddScene(new Scene(name));
    int objCnt = Random::RandInt(1, 9);
    scene->ScenePending = objCnt; // Tell the scene it is to wait for X objects
    scene->IsLoading = true;
    for (int i = 0; i < objCnt; i++) {
        Shared<Object> obj = scene->AddObject(new Object("Object" + std::to_string(i)));

        // Randomize mesh to use
        int meshChoice = Random::RandInt(0, meshes.size() - 1);
        mesh_info_composite chosenMesh = meshes[meshChoice];
        MeshRenderer* renderer = obj->AddComponent<MeshRenderer>(new MeshRenderer(chosenMesh.file, "Assets/Models/" + chosenMesh.file, 2000, name));
        renderer->Shader = basicShader;
        
        // Randomize colors
        float r = Random::RandFloat(0.1f, 0.9f);
        float g = Random::RandFloat(0.1f, 0.9f);
        float b = Random::RandFloat(0.1f, 0.9f);
        renderer->base_color = Color(r, g, b);

        // Slight scale randomization, special scale factor for bunny, tyra, and cruiser
        float scale = Random::RandFloat(chosenMesh.scale.x, chosenMesh.scale.y);
        obj->transform.scale = Vec3(scale);
        
        // Randomize Rotation
        float rot = Random::RandFloat(0.f, 360.f);
        obj->transform.rotation = Quaternion(rot, Vec3(0.f, 1.f, 0.f));

        // Randomize Position
        float interval = 0.15f;
        float x = (float)Random::RandInt(-6, 6) * interval;
        float y = (float)Random::RandFloat(-1, 1) * interval * 0.2f;
        float z = (float)Random::RandInt(-6, 6) * interval;
        obj->transform.position = Vec3(x, y ,z);
    }
}

/// <summary>
/// On the client side, request scenes from the server then construct dummy scenes in the client's scene manager
/// Runs once on startup
/// </summary>
void SceneManager::RequestScenes()
{
    // Any processing of the client request into a list of string names
    List<String> scenelist; // = Client->RequestSceneList();
    
    _scenes.clear();
    for (int i = 0; i < scenelist.size(); i++) {
        String name = scenelist[i];
        Shared<Scene> scene = Make_Shared<Scene>(name);
        scene->ScenePending = 1;
        scene->IsLoading = true;
        _scenes[name] = scene;
        ProcessStreamedScene(name);
    }

}

/// <summary>
/// Construct and interpret scene data from client to server
/// </summary>
void SceneManager::ProcessStreamedScene(const String name)
{
    // Request the server to get scene meta info
    // Process result to variables
    bool exists;
    int objCount;

    if (!exists) {
        Logger::LogWarning("Tried to load a scene that does not exist on the server");
        return;
    }

    // Create a worker task to get streamed data
    ThreadPoolManager::GetThreadPool("Main")->ScheduleTask(new LoadObjectsFromServerTask(name, objCount));
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
