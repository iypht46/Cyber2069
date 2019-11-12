#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer() {
	isLooping = false;
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
	soundPlayer->play2D(soundSource, isLooping);
}

void SoundPlayer::DeleteSoundPlayer() {
	soundPlayer->drop();
}

void SoundPlayer::SetLoop(bool loop) {
	isLooping = loop;
}