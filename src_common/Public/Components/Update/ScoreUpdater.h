#pragma once

#include <Components/Update/IUpdateComponent.h>
#include <Components/Renderers/TextRenderer.h>

class ScoreUpdater : public IUpdateComponent {
public:
	int Score = 0;
	void UpdateScore(int amount) {
		Score += amount;
		if (Score < 0) Score = 0;
	}

private:
	bool _playerDied = false;
	float _elapsedTime = 0.f;
	float _playerDiedDisplayTime = 0.5f;

	TextRenderer* _text = nullptr;
	void Update(float deltaTime) {
		if (_text == nullptr) { _text = _owner->GetComponent<TextRenderer>(); return; }
		if (Score < 0) {
			_playerDied = true;
			_elapsedTime = 0.f;
			_text->Text = "You Died!";
			Score = 0;
		}

		if (_playerDied) {
			_elapsedTime += deltaTime;
			if (_elapsedTime >= _playerDiedDisplayTime)
				_playerDied = false;
		}
		else {
			_text->Text = std::to_string(Score);

		}

	}
};