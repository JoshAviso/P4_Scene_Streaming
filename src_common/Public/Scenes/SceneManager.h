#pragma once

#include <String.hpp>
#include <Common.hpp>
#include <Scenes/Scene.h>

class SceneManager {
public:
	struct Desc {
		bool isServer;
		String camName;
	};


private:
	bool _isServer;
	Dict<String, Shared<Scene>> _scenes;
	String _camName;

public:
	static void OpenScene(const String sceneName);
	static Shared<Scene> AddScene(Scene* scene);

private:
	void PopulateScenes();

	// SINGLETON
public:
	static SceneManager* Initialize(const Desc& desc);
private:
	static SceneManager* _instance;
	explicit SceneManager(const Desc& desc);
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator= (const SceneManager&) = delete;
};