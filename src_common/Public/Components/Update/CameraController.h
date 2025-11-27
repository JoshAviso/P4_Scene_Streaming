#pragma once

#include <Components/Update/IUpdateComponent.h>
#include <Input/Input.h>

#include <Logger.hpp>
#include <GLFW/glfw3.h>

class CameraController : public IUpdateComponent {
public:
	CameraController(float moveSpeed, float rotSpeed) : _moveSpeed(moveSpeed), _rotSpeed(rotSpeed) {} ;

private:
	float _moveSpeed;
	float _rotSpeed;
	float _pitch = 0.f;
	float _yaw = 0.f;
	float _speedScroll = 0.05f;
	void Update(float deltaTime) {
		if (Input::Mouse.RightDown) {
			UpdateSpeed();
			UpdateRotation(deltaTime);
			UpdatePosition(deltaTime);
			Input::Mouse.Mode = Input::Locked;
		}
		else {
			Input::Mouse.Mode = Input::Normal;
		}
	}
	void UpdateSpeed() {
		_moveSpeed += Input::Mouse.ScrollDelta * _speedScroll;
		if (_moveSpeed < 0.f) _moveSpeed = 0.f;
	}
	void UpdateRotation(float deltaTime) {
		_yaw -= Input::Mouse.Delta.x * deltaTime * _rotSpeed;
		_pitch -= Input::Mouse.Delta.y * deltaTime * _rotSpeed;
		if (_pitch < -90.f) _pitch = -90.f;
		if (_pitch > 90.f) _pitch = 90.f;
		if (_yaw > 180.f) _yaw = _yaw - 360.f;
		if (_yaw < -180.f) _yaw = _yaw + 360.f;

		Quaternion qYaw(_yaw, Vec3(0.f, 1.f, 0.f));
		Quaternion qPitch(_pitch, Vec3(1.f, 0.f, 0.f));

		_owner->transform.rotation = qYaw * qPitch;
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
		if (Input::KeyDown[Input::LShift]) move *= 2.f;
		if (Input::KeyDown[Input::RCtrl]) move *= 0.5f;

		float yMove = 0.f;
		if (Input::KeyDown[Input::Q]) yMove -= 1.f;
		if (Input::KeyDown[Input::E]) yMove += 1.f;
		yMove *= _moveSpeed;

		move.y += yMove;
		return move;
	}
	friend class ObjectManager;
};