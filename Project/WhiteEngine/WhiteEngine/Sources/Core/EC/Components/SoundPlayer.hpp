#pragma once
#include "Component.hpp"
#include <string>
#include <irrKlang.h>

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>

using namespace irrklang;

class SoundPlayer : public Component {
private:
	bool isLooping;
	float volumeValue;
	/*undetermined*/std::string sr_soundpath;

	ISoundEngine* soundPlayer = nullptr;
	ISoundSource* soundSource = nullptr;
	ISound* soundVolume = nullptr;
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

//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			isLooping,
			volumeValue
		);
	}
};

CEREAL_REGISTER_TYPE(SoundPlayer);