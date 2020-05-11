#include "SoundPlayer.hpp"
#include <glm/glm.hpp>
#include "Core/Logger.hpp"

SoundPlayer::SoundPlayer() {
	//isLooping = false;
	//volumeValue = 1.0f;
	Factory<Component, SoundPlayer>::Add(this);
}

SoundPlayer::~SoundPlayer() {
	Factory<Component, SoundPlayer>::Remove(this);
}

void SoundPlayer::Init() {
	CreateSoundPlayer();
}

void SoundPlayer::CreateSoundPlayer() {
	soundPlayer = createIrrKlangDevice();
}

void SoundPlayer::SetSound(std::string path) {
	sr_soundpath = path;
}

void SoundPlayer::PlaySound() {
	if(!soundPlayer->getSoundSource(&(sr_soundpath[0]))){
		soundSource = soundPlayer->addSoundSourceFromFile(&(sr_soundpath[0]));
	}
	else {
		soundSource = soundPlayer->getSoundSource(&(sr_soundpath[0]));
	}

	soundVolume = soundPlayer->play2D(soundSource, isLooping, false, true);
}

void SoundPlayer::StopSound() {
	soundPlayer->stopAllSounds();
	ENGINE_INFO("Stop sound");
}

void SoundPlayer::DeleteSoundPlayer() {
	soundPlayer->drop();
}

void SoundPlayer::SetLoop(bool loop) {
	isLooping = loop;
}

void SoundPlayer::UpdateVolume() {
	if (soundVolume != nullptr) {
		soundVolume->setVolume(volumeValue);
	}
}

void SoundPlayer::SetVolume(float value) {
	volumeValue = value;
	UpdateVolume();
}

void SoundPlayer::IncreaseVolume() {
	volumeValue += 0.05f;
	volumeValue = glm::clamp(volumeValue, 0.0f, 1.0f);
	UpdateVolume();
}

void SoundPlayer::DecreaseVolume() {
	volumeValue -= 0.05f;
	volumeValue = glm::clamp(volumeValue, 0.0f, 1.0f);
	UpdateVolume();
}

void SoundPlayer::AdjustVolume(float diff) {
	volumeValue += diff;
	volumeValue = glm::clamp(volumeValue, 0.0f, 1.0f);
	UpdateVolume();
}