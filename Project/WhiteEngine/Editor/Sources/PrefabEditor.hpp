#pragma once

#include "Editor.hpp"
#include "UIWindow.hpp"
#include "ComponentsBrowser.hpp"
#include "Core/EC/GameObject.hpp"

namespace Core
{
	class GameObject;
}

namespace Tools
{
	class PrefabEditor : public UIWindow
	{
	private:
		//
		//TODO: Add UI Window Elements
		
		ComponentsBrowser m_componentBrowser;
		//TODO: Preview Window
	public:
		virtual void Init(void) override;
		virtual void OnRender(void) override;
		virtual void Terminate(void) override;

		//Interface

	};
}