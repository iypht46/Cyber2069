#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer() {
	//isLooping = false;
	//volumeValue = 1.0f;
	Factory<SoundPlayer>::Add(this);
}

SoundPlayer::~SoundPlayer() {

}

void SoundPlayer::Init() {
	CreateSoundPlayer();
	SetSound(&(sr_soundpath[0]));
}

void SoundPlayer::CreateSoundPlayer() {
	soundPlayer = createIrrKlangDevice();
}

void SoundPlayer::SetSound(const ik_c8* path) {
	sr_soundpath = path;
	soundSource = soundPlayer->addSoundSourceFromFile(path); 
}

void SoundPlayer::PlaySound() {
	soundVolume = soundPlayer->play2D(soundSource, isLooping, false, true);
	
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
}

void SoundPlayer::IncreaseVolume() {
	if (volumeValue == 1) {
		volumeValue += 0;
	}
	else {
		volumeValue += 0.05f;
	}
}

void SoundPlayer::DecreaseVolume() {
	if (volumeValue == 0) {
		volumeValue -= 0;
	}
	else {
		volumeValue -= 0.05f;
	}
}