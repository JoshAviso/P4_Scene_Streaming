#pragma once

#include <Components/Update/IUpdateComponent.h>

#include <Components/Renderers/TextRenderer.h>

class FPSCounterUpdater : public IUpdateComponent
{
public:
	float UpdateFrequency = 0.f;

private:
	float _accumulatedTime = 0.f;
	int _frameCount = 0;
	TextRenderer* _text = nullptr;

public:
	FPSCounterUpdater(float updateFrequency) : UpdateFrequency(updateFrequency) {}
	void Update(float deltaTime) override;
};

