#pragma once
#include "Component.hpp"
#include <string>
#include <irrKlang.h>

using namespace irrklang;

class SoundPlayer : public Component {
private:
	ISoundEngine* soundPlayer;
	ISoundSource* soundSource;
	ISound* soundVolume;
	bool isLooping;
	float volumeValue;
public:
	SoundPlayer();
	~SoundPlayer();
	void CreateSoundPlayer();
	void SetSound(const ik_c8* path);
	void PlaySound();
	void DeleteSoundPlayer();
	void SetLoop(bool loop);
	void UpdateVolume();
	void SetVolume(float value);
	void IncreaseVolume();
	void DecreaseVolume();
};