#pragma once

#include <Components/Update/IUpdateComponent.h>
#include <Core/Common.h>

#include <Components/Update/EnemyController.h>
#include <Components/Renderers/ShapeRenderers/PolygonRenderer.h>
#include <Math/Random.h>

#include <Components/Update/ScoreUpdater.h>

class EnemySpawner : public IUpdateComponent {
public:
	struct Desc {
		Vec2 spawnMargin;
	};
	explicit EnemySpawner(const Desc desc) {
		_spawnMargin = desc.spawnMargin;
	}

private:
	float _elapsedTime = 0.f;
	Vec2 _spawnMargin = { 0.f, 0.f };
	List<Object*> _enemies;
	ScoreUpdater* _score;

	void Update(float deltaTime) {
		if (_score == nullptr) _score = ObjectManager::FindObjectByName("ScoreDisplay")->GetComponent<ScoreUpdater>();

		_elapsedTime += deltaTime;

		float spawnFreq = (2.f - ((float)_score->Score / 20000.f) * 1.9f);

		if (_elapsedTime >= spawnFreq) {
			_elapsedTime = 0.f;

			int cnt = Random::RandInt(2, 3);
			for(int i = 0; i < cnt; i++) SpawnEnemy();
		}
	}

	void SpawnEnemy() {
		if (_enemies.size() >= 500) return;

		Object* enemy = ObjectManager::RegisterObject(new Object("Enemy"));
		enemy->Position = {
			Random::RandFloat(_spawnMargin.x, Application::WindowSize().x - _spawnMargin.x),
			Random::RandFloat(_spawnMargin.y, Application::WindowSize().y - _spawnMargin.y)
		};

		enemy->AddComponent(new EnemyController({ 100, 100.f, 180.f, 0.1f, {0.f, 0.f}, {0.f, 0.f} }));
		enemy->AddComponent(new PolygonRenderer(5, 12.f))->FillColor = Color(0.8f, 0.f, 0.2f, 1.f);


		_enemies.push_back(enemy);
	}

public:
	void DisableEnemies() {
		for (int i = 0; i < _enemies.size(); i++)
			ObjectManager::RemoveObject(_enemies[i]);
	}
};