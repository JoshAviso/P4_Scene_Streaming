#pragma once
#include <Components/Update/IUpdateComponent.h>
#include <Input/Input.h>
#include <Application.h>
#include <Input/ILeftClickInputCallback.h>

#include <cmath>
#include <Math/Constants.h>

#include <Objects/ObjectManager.h>

#include <Components/Renderers/ShapeRenderers/RectangleRenderer.h>
#include <Components/Update/BulletMover.h>

#include <Components/Update/MouseDetectScript.h>

#include <Components/Interfaces/ICollider.h>

#include <Components/Update/ScoreUpdater.h>
#include <Components/Update/EnemySpawner.h>

class CharacterController : public IUpdateComponent, public ILeftClickInputCallback, public ICollider
{
public:
	struct Desc {
		float moveSpeed;
		Vec2 screenEdgeMargin;
	};
	explicit CharacterController(Desc desc) 
		: _moveSpeed(desc.moveSpeed), 
		_screenEdgeMargin(desc.screenEdgeMargin)
	{};

private:
	List<Object*> _bullets;
	ScoreUpdater* _score;
	EnemySpawner* _spawner;

	float _moveSpeed = 0.f;
	Vec2 _screenEdgeMargin = {0.f, 0.f};
	void Update(float deltaTime) {
		if (_score == nullptr) _score = ObjectManager::FindObjectByName("ScoreDisplay")->GetComponent<ScoreUpdater>();
		if (_spawner == nullptr) _spawner = ObjectManager::FindObjectByName("EnemySpawner")->GetComponent<EnemySpawner>();

		HandleRotation();
		HandleMovement(deltaTime);
	}

	void HandleRotation() {
		Vec2 mouseDir = Vec2({ (float)Input::Mouse.Position.x, (float)Input::Mouse.Position.y }) - _owner->Position;
		mouseDir.normalize();
		mouseDir.y = -mouseDir.y;

		float angleRad = std::atan2(mouseDir.y, mouseDir.x);
		float angleDeg = angleRad * (180.f / fPI);

		float targetRot = angleDeg - 90.f;

		_owner->Rotation = -targetRot;
	}

	void HandleMovement(float deltaTime) {
		Vec2 inputVec = { 0.f, 0.f };
		if (Input::KeyDown[Input::EKeyCode::Key_W]) inputVec.y -= 1.f;
		if (Input::KeyDown[Input::EKeyCode::Key_S]) inputVec.y += 1.f;
		if (Input::KeyDown[Input::EKeyCode::Key_A]) inputVec.x -= 1.f;
		if (Input::KeyDown[Input::EKeyCode::Key_D]) inputVec.x += 1.f;
		inputVec.normalize();

		float speed = _moveSpeed;

		_owner->Position += inputVec * speed * deltaTime;

		// Loop the player on the screen
		if (_owner->Position.x > Application::WindowSize().x + _screenEdgeMargin.x)
			_owner->Position.x = -_screenEdgeMargin.x;
		if (_owner->Position.y > Application::WindowSize().y + _screenEdgeMargin.y)
			_owner->Position.x = -_screenEdgeMargin.y;
		if (_owner->Position.x < -_screenEdgeMargin.x)
			_owner->Position.x = Application::WindowSize().x + _screenEdgeMargin.x;
		if (_owner->Position.y < -_screenEdgeMargin.y)
			_owner->Position.y = Application::WindowSize().y + _screenEdgeMargin.y;
	}

	void OnLeftClickPressed(Vec2i mousePosition) override {
		SpawnBullet();
	};

	void SpawnBullet() {
		Object* bullet = ObjectManager::RegisterObject(new Object("PlayerBullet"));
		bullet->AddComponent(new RectangleRenderer({10.f, 5.f}))->RenderLayer = 8;
		bullet->AddComponent(new BulletMover({ _moveSpeed * 1.5f, {15.f, 10.f} , true}));
		bullet->Position = _owner->Position;
		bullet->Rotation = _owner->Rotation - 90.f;

		bullet->AddComponent(new MouseDetectScript());
		_bullets.push_back(bullet);
	}

	virtual void OnCollisionEnter(ICollider* other) override {
		if (other->GetOwner()->Name == "Enemy") {
			_owner->Position = {
				Application::WindowSize().x / 2.f,
				Application::WindowSize().y / 2.f
			};
			
			if (_score) _score->Score = -1;
			if (_spawner) _spawner->DisableEnemies();
		}
	};

public:
	void DisableBullets() {
		for (int i = 0; i < _bullets.size(); i++) {
			ObjectManager::RemoveObject(_bullets[i]);
		}
	}
};

