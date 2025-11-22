#pragma once
#include <Components/Update/IUpdateComponent.h>
#include <Components/Interfaces/IMouseClickCallback.h>
#include <Components/Interfaces/IMouseHoverCallback.h>

#include <Core/Logger.h>

class MouseDetectScript : public IUpdateComponent, public IMouseClickCallback, public IMouseHoverCallback
{

private:
	void Update(float deltaTime) {

	}

private:
	virtual void OnClickedOnCallback(Vec2i mousePosition) override {
		Logger::Log("Clicked on " + _owner->Name);
	};
	virtual void OnReleasedOnCallback(Vec2i mousePosition) override {
		Logger::Log("Released on " + _owner->Name);
	};
	virtual void OnHoveredOnCallback(Vec2i mousePosition) override {
		Logger::Log("Hovered on " + _owner->Name);
	};
	virtual void OnHoveredOffCallback(Vec2i mousePosition) override {
		Logger::Log("Hovered off " + _owner->Name);
	};
};

