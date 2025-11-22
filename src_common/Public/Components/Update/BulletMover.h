#pragma once
#include <Components/Update/IUpdateComponent.h>
#include <Input/Input.h>
#include <Application.h>
#include <Input/ILeftClickInputCallback.h>

#include <cmath>
#include <Math/Constants.h>

#include <Objects/ObjectManager.h>

#include <Components/Renderers/ShapeRenderers/RectangleRenderer.h>

#include <Components/Interfaces/ICollider.h>

class BulletMover : public IUpdateComponent, public ICollider
{
public:
	struct Desc {
		float moveSpeed;
		Vec2 screenEdgeMargin;
		bool isFriendly;
	};
	explicit BulletMover(Desc desc)
		: _moveSpeed(desc.moveSpeed), 
		_screenEdgeMargin(desc.screenEdgeMargin),
		_isFriendly(desc.isFriendly)
	{};

private:
	float _moveSpeed = 0.f;
	Vec2 _screenEdgeMargin = {0.f, 0.f};
	bool _isFriendly = true;
	void Update(float deltaTime) {
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
		float rotRad = _owner->Rotation * fPI / 180.f;

		Vec2 inputVec = { std::cos(rotRad), std::sin(rotRad)};

		float speed = _moveSpeed;

		_owner->Position += inputVec * speed * deltaTime;

		// Disable the bullet off screen
		if (
			_owner->Position.x > Application::WindowSize().x + _screenEdgeMargin.x ||
			_owner->Position.y > Application::WindowSize().y + _screenEdgeMargin.y ||
			_owner->Position.x < -_screenEdgeMargin.x ||
			_owner->Position.y < -_screenEdgeMargin.y
			) {
			_owner->Enabled = false;
			ObjectManager::RemoveObject(this->_owner);
		}
	}

	void OnCollisionEnter(ICollider* other) override {
		if (other->GetOwner()->Name == (_isFriendly ? "Enemy" : "Player")) {
			_owner->Enabled = false;
			ObjectManager::RemoveObject(this->_owner);
		}
	};
};

