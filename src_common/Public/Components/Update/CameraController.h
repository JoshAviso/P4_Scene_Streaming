#pragma once

#include <Components/Update/IUpdateComponent.h>
#include <Input/Input.h>

#include <Logger.hpp>

class CameraController : public IUpdateComponent {
public:
	CameraController(float speed) : _moveSpeed(speed) {} ;

private:
	float _moveSpeed;
	float _rotSpeed = 10.f;
	float _pitch = 0.f;
	float _yaw = 0.f;
	void Update(float deltaTime) {
		if(Input::Mouse.RightDown)
			UpdateRotation(deltaTime);
		UpdatePosition(deltaTime);
	}

	void UpdateRotation(float deltaTime) {
		Logger::Log(Input::Mouse.Delta);
			
		_yaw += Input::Mouse.Delta.x * deltaTime * _rotSpeed;
		_pitch += Input::Mouse.Delta.y * deltaTime * _rotSpeed;
		if (_yaw < -180.f) _yaw = -180.f;
		if (_yaw > 180.f) _yaw = 180.f;

		Quaternion qYaw(_yaw, Vec3(0.f, 1.f, 0.f));
		Quaternion qPitch(_pitch, _owner->transform.rotation * Vec3(0.f, 0.f, 1.f));

		_owner->transform.rotation = qYaw;
	}

	void UpdatePosition(float deltaTime) {
		_owner->transform.position += CalculateMoveVector() * deltaTime;
	}
	Vec3 CalculateMoveVector() {
		Vec2 moveDir = Vec2({ 0.f, 0.f });
		if (Input::KeyDown[Input::W]) moveDir.y -= 1.f;
		if (Input::KeyDown[Input::S]) moveDir.y += 1.f;
		if (Input::KeyDown[Input::D]) moveDir.x += 1.f;
		if (Input::KeyDown[Input::A]) moveDir.x -= 1.f;
		moveDir.normalize();
		Vec3 move = Vec3(moveDir.x, 0.f, moveDir.y);
		move = _owner->transform.rotation * move;
		move *= _moveSpeed;

		float yMove = 0.f;
		if (Input::KeyDown[Input::Q]) yMove -= 1.f;
		if (Input::KeyDown[Input::E]) yMove += 1.f;
		yMove *= _moveSpeed;

		move.y += yMove;
		return move;
	}
	friend class ObjectManager;
};