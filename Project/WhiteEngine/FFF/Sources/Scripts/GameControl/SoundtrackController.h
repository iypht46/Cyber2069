#pragma once
#include <string>
#include <queue>
#include <vector>
#include <memory>

#include <Core/EC/Components/BehaviourScript.h>

#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

enum SOUNDTRACK_STATE {
	MENU = 0,
	GAMEPLAY_NORMAL,
	GAMEPLAY_BOSS
};

class SoundPlayer;

class SoundtrackController : public BehaviourScript {
private:
	bool playing;	//use to determine whether to fade in or out
	bool fade;		//whether the music should fade when start/stop

	std::weak_ptr<SoundPlayer> channel;

	//std::queue<std::string> trackqueue;

	std::string RandomTrack(std::vector<std::string>& list);

public:
	float fadeLength = 1.5f;

	//pick at random from the list --------------
	std::vector<std::string> MenuTracks;
	std::vector<std::string> GameplayTracks;
	std::vector<std::string> BossTracks;
	//-------------------------------------------

	void SetSoundChannel(std::weak_ptr<SoundPlayer> player) { channel = player; }

	void PlayState(SOUNDTRACK_STATE state, bool shouldfade);
	void Stop(bool shouldfade);

	//whether the sound volume is appropriate to the playing state
	bool SoundAdjusted();

	virtual void OnFixedUpdate(float) override;

	//serialization
public:
	template <class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			GameplayTracks,
			BossTracks,
			MenuTracks,
			fadeLength,
			cereal::defer(channel)
			);
	}
};

CEREAL_REGISTER_TYPE(SoundtrackController);