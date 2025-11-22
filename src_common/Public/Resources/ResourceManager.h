#pragma once

#include <Common.hpp>
#include <String.hpp>
#include <Resources/IResource.h>

#include <stdexcept>
#include <mutex>

class ResourceManager
{
private:
	Dict<String, Shared<IResource>> _resources;
	std::mutex _resourceGuard;

public:
	template <typename TResource>
	static Shared<TResource> LoadFromFile(String name, String path) {
		static_assert(std::is_base_of<IResource, TResource>::value, "TResource must be a valid resource type.");
		
		_instance->_resourceGuard.lock();
		if (_instance->_resources.find(name) != _instance->_resources.end()) {
			auto casted = std::dynamic_pointer_cast<TResource>(_instance->_resources[name]);

			_instance->_resourceGuard.unlock();
			if (!casted) {
				throw std::runtime_error("Stored resource type mismatch for key: " + name);
			}
			return casted;
		}
		_instance->_resourceGuard.unlock();

		Shared<TResource> resource = Make_Shared<TResource>();
		if (resource->LoadFromFile(path)) {
			_instance->_resourceGuard.lock();
			_instance->_resources[name] = resource;
			_instance->_resourceGuard.unlock();

			return resource;
		}

		return nullptr;
	}

	template <typename TResource>
	static Shared<TResource> GetResource(String name) {
		static_assert(std::is_base_of<IResource, TResource>::value, "TResource must be a valid resource type.");

		_instance->_resourceGuard.lock();
		if (_instance->_resources.find(name) != _instance->_resources.end()) {
			auto casted = std::dynamic_pointer_cast<TResource>(_instance->_resources[name]);
			_instance->_resourceGuard.unlock();

			if (!casted) {
				throw std::runtime_error("Stored resource type mismatch for key: " + name);
			}
			return casted;
		}
		_instance->_resourceGuard.unlock();

		return nullptr;
	}

// SINGLETON
public:
	static ResourceManager* Initialize();
private:
	static ResourceManager* _instance;
	explicit ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

};

