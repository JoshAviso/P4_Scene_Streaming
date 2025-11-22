#pragma once

#include <Components/Update/IUpdateComponent.h>

#include <Core/Common.h>
#include <Core/String.h>
#include <Resources/SoundClipResource.h>

#include <SFML/Audio.hpp>

class AudioSourceComponent : public IUpdateComponent
{

public:
	AudioSourceComponent() = default;
	AudioSourceComponent(String musicFilePath);
	AudioSourceComponent(Shared<SoundClipResource> soundclip);
	virtual ~AudioSourceComponent();

public:
	void Play();
	void Pause();
	void Stop();

	bool SetMusic(String musicFilePath);
	void SetSoundClip(Shared<SoundClipResource> soundclip);

	void SetVolume(float volume);
	void SetPitch(float pitch);
	void SetLooping(bool loop);
	void SetStartTimeOffset(float time);

	float GetVolume() const;
	float GetPitch() const;
	bool IsLooping() const;
	float GetPlaybackOffset() const;

	String GetMusicFilePath() const;
	Shared<SoundClipResource> GetSoundClip() const;

private:
	sf::Music _music;
	sf::Sound _sound;

	String _musicFilePath;
	Shared<SoundClipResource> _soundclipResource;

	void Update(float deltaTime) override;
};

