#pragma once

#include <Components/Update/IUpdateComponent.h>
#include <Components/Interfaces/ICollider.h>

#include <Objects/Object.h>
#include <Core/Logger.h>

#include <Components/Update/ScoreUpdater.h>

class EnemyController : public IUpdateComponent, public ICollider {
public:
	struct Desc {
		int scoreReward;
		float acceleration;
		float maxVelocity;
		float targetingFreq;
		Vec2 maxTargetOffset;
		Vec2 screenEdgeMargin;
	};
	explicit EnemyController(const Desc desc) {
		_scoreReward = desc.scoreReward;
		_maxVelocity = desc.maxVelocity;
		_acceleration = desc.acceleration;
		_targetingFreq = desc.targetingFreq;
		_maxTargetOffset = desc.maxTargetOffset;
	}

private:
	int _scoreReward = 0;
	float _targetingFreq = 0.f;
	Vec2 _maxTargetOffset = { 0.f, 0.f };
	float _acceleration = 0.f;
	Vec2 _velocity = { 0.f, 0.f };
	float _maxVelocity = 100000.f;
	Vec2 _targetPos = { 0.f, 0.f };
	Vec2 _screenEdgeMargin = { 0.f, 0.f };

	Object* _playerRef = nullptr;
	ScoreUpdater* _score = nullptr;

	float elapsedTime = 0.f;

	void Update(float deltaTime) override {
		if (_playerRef == nullptr) _playerRef = ObjectManager::FindObjectByName("Player");
		if (_score == nullptr) _score = ObjectManager::FindObjectByName("ScoreDisplay")->GetComponent<ScoreUpdater>();

		elapsedTime += deltaTime;
		
		// Calc Acc Dir
		Vec2 accDir = _targetPos - _owner->Position;
		accDir.normalize();

		// Apply acceleration
		_velocity += accDir * _acceleration * deltaTime;
		if (_velocity.sqrMagnitude() > _maxVelocity * _maxVelocity) {
			_velocity.normalize();
			_velocity *= _maxVelocity;
		}

		// Apply velocity
		_owner->Position += _velocity * deltaTime;

		// Loop the enemy on the screen
		if (_owner->Position.x > Application::WindowSize().x + _screenEdgeMargin.x)
			_owner->Position.x = -_screenEdgeMargin.x;
		if (_owner->Position.y > Application::WindowSize().y + _screenEdgeMargin.y)
			_owner->Position.x = -_screenEdgeMargin.y;
		if (_owner->Position.x < -_screenEdgeMargin.x)
			_owner->Position.x = Application::WindowSize().x + _screenEdgeMargin.x;
		if (_owner->Position.y < -_screenEdgeMargin.y)
			_owner->Position.y = Application::WindowSize().y + _screenEdgeMargin.y;

		// Update Targeting
		if (elapsedTime >= _targetingFreq && _playerRef) {
			elapsedTime = 0.f;

			_targetPos = _playerRef->Position;
		}

	}

	virtual void OnCollisionEnter(ICollider* other) override {
		if (other->GetOwner()->Name == "PlayerBullet") {
			_owner->Enabled = false;
			ObjectManager::RemoveObject(this->_owner);

			if (_score) _score->UpdateScore(_scoreReward);
		}
	};

};

