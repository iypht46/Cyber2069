#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer() {
	isLooping = false;
	volumeValue = 1.0f;
}

SoundPlayer::~SoundPlayer() {

}

void SoundPlayer::CreateSoundPlayer() {
	soundPlayer = createIrrKlangDevice();
}

void SoundPlayer::SetSound(const ik_c8* path) {
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
	soundVolume->setVolume(volumeValue);
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