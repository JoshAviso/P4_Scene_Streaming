#include <Resources/ResourceManager.h>

// SINGLETON 
ResourceManager* ResourceManager::_instance = nullptr;
ResourceManager* ResourceManager::Initialize()
{
    if (_instance != nullptr) return _instance;

    _instance = new ResourceManager();
    return _instance;
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager() {}

