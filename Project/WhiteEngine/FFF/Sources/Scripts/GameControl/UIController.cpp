#include "UIController.h"
#include "../../GameController.hpp"
#include "Core//Logger.hpp"

UIController* UIController::_instance = nullptr;

UIController::UIController() {
	_instance = this;
}

UIController* UIController::GetInstance() {
	if (_instance != nullptr) {
		return _instance;
	}
	else {
		ENGINE_ERROR("There is no instance of UIController");
	}
}

void UIController::ToggleUI(int openGroup) {
	for (auto group : UIGroups) {
		bool active =
			(group.first == openGroup) ||
			(group.first == 1 && openGroup <= UI_GROUP::Credit);

		for (std::weak_ptr<GameObject> uiobj : group.second) {

			if (!uiobj.expired()) {
				uiobj.lock()->SetActive(active);
			}
			else {
				ENGINE_WARN("uiobj is expired");
			}
		}
	}
}
