#pragma once

#include <Components/Update/IUpdateComponent.h>
#include <Math/Vec2.h>

#include <Core/Common.h>

class ImagePanning : public IUpdateComponent
{
public:
	struct ObjectPanningInfo
	{
		Object* object;
		Vec2 startPos;
		Vec2 panSpeed;
		float maxAlpha;
		uint setNumber;
	};

public:
	ImagePanning(float panTime, float fadeTime, List<ObjectPanningInfo>& objects);
	void Update(float deltaTime) override;

private:
	void ToggleObject(ObjectPanningInfo obj, bool enabled);

private:
	float elapsedTime = 0.0f;

	uint currentSet = 0;
	uint setCount = 0;
	float _panTime = 3.0f;
	float _fadeTime = 1.0f;

	Dict<uint, List<ObjectPanningInfo>> _objects;
	List<ObjectPanningInfo>* _currentObjects = nullptr;
	List<ObjectPanningInfo>* _prevObjects = nullptr;
};