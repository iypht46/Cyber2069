#pragma once

#include "UIWindow.hpp"
#include <map>

namespace Tools
{
	class GameObjectHierarchy : public UIWindow
	{
	private:
		
	public:
		GameObjectHierarchy() : UIWindow("Gameobject List") {}
		void Init() override;
		void OnRender() override;
	};

}
