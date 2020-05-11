#pragma once
#include "Core/Factory.h"
#include <string>
#include <irrKlang.h>

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>

using namespace irrklang;

class SoundPlayer : public Component {
private:
	bool isLooping = false;
	float volumeValue = 1.0f;
	/*undetermined*/std::string sr_soundpath;

	ISoundEngine* soundPlayer = nullptr;
	ISoundSource* soundSource = nullptr;
	ISound* soundVolume = nullptr;
	
	//these should be private -------------
	//void CreateSoundPlayer();
	//void UpdateVolume();
	//-------------------------------------

public:
	SoundPlayer();
	~SoundPlayer();

	virtual void Init();
	
	//---------------------------
	void CreateSoundPlayer();
	void UpdateVolume();
	//---------------------------

	float GetVolume() { return volumeValue; }

	void SetSound(std::string path);
	void PlaySound();
	void StopSound();
	void DeleteSoundPlayer();
	void SetLoop(bool loop);
	void SetVolume(float value);
	void IncreaseVolume();
	void DecreaseVolume();
	void AdjustVolume(float diff);

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			sr_soundpath,
			isLooping,
			volumeValue
		);
	}
};

CEREAL_REGISTER_TYPE(SoundPlayer);