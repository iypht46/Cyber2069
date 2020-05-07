#pragma once
#include "Core/EC/Components/BehaviourScript.h"

#include <map>
#include <vector>
#include <memory>

#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

enum UI_GROUP {
	MainMenu = 0,
	Option,
	Credit,
	Loadout,		
	Gameplay,				//not include boss hp bar
	Pause,
	GameOver
};

//forward declaration
class GameObject;

class UIController : public BehaviourScript{
public:
	static UIController* GetInstance();

	std::map<int, std::vector<std::weak_ptr<GameObject>>> UIGroups;

	//SetActive the UI of openGroup true and turn other groups inactive
	void ToggleUI(int openGroup);

	UIController();

private:
	static UIController* _instance;

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			cereal::defer(UIGroups)
			);
	}
};

CEREAL_REGISTER_TYPE(UIController);