#include "SoundtrackController.h"

#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/SoundPlayer.hpp"

#include "Utility/WhiteMath.h"
#include "Core//Logger.hpp"

void SoundtrackController::OnAwake() {
	ReRollTrackSet();
}

void SoundtrackController::ReRollTrackSet() {
	CurrentGameActionTrack = RandomTrack(GameplayTracks);
	CurrentGameBossTrack = RandomTrack(BossTracks);
}

void SoundtrackController::PlayState(SOUNDTRACK_STATE state, bool shouldfade) {
	fade = shouldfade;
	playing = true;

	if (!channel.expired()) {

		channel.lock()->StopSound();

		//set track
		switch (state)
		{
		case MENU:
			channel.lock()->SetLoop(true);
			if (MenuTracks.size() > 0) {
				channel.lock()->SetSound(RandomTrack(MenuTracks));
			}
			else {
				ENGINE_ERROR("{}; No menu track assigned", *m_gameObject);
			}
			break;

		default:
		case GAMEPLAY_NORMAL:
			channel.lock()->SetLoop(true);
			if (GameplayTracks.size() > 0) {
				channel.lock()->SetSound(CurrentGameActionTrack);
			}
			else {
				ENGINE_ERROR("{}; No gameplay track assigned", *m_gameObject);
			}
			break;

		case GAMEPLAY_BOSS:
			channel.lock()->SetLoop(true);
			if (BossTracks.size() > 0) {
				channel.lock()->SetSound(CurrentGameBossTrack);
			}
			else {
				ENGINE_ERROR("{}; No boss fight track assigned", *m_gameObject);
			}
			break;
		}

		channel.lock()->PlaySound();
	}
	else {
		ENGINE_ERROR("{}; No soundtrack channel assigned", *m_gameObject);
	}
}

void SoundtrackController::Stop(bool shouldfade) {
	fade = shouldfade;
	playing = false;
}

void SoundtrackController::OnFixedUpdate(float dt) {
	float diff = (1.0f / fadeLength) * dt;
	if (!channel.expired()) {
		if (!SoundAdjusted()) {
			if (playing) {
				if (fade) {
					channel.lock()->AdjustVolume(diff);
				}
				else if (!fade) {
					channel.lock()->SetVolume(1.0f);
				}
			}
			else {
				if (fade) {
					channel.lock()->AdjustVolume(-diff);
				}
				else {
					channel.lock()->SetVolume(0.0f);
				}
			}
		}
		else {
			if (!playing) {
				channel.lock()->StopSound();
			}
		}
	}
}

std::string SoundtrackController::RandomTrack(std::vector<std::string>& list) {
	return list[WhiteMath::Rand(0, list.size() - 1)];
}

bool SoundtrackController::SoundAdjusted() {
	if (!channel.expired()) {
		return (playing && channel.lock()->GetVolume() >= 1.0f) || (!playing && channel.lock()->GetVolume() <= 0.0f);
	}
	else {
		return false;
	}
}