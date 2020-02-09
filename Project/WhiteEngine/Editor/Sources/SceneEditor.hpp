#pragma once

#include "UIWindow.hpp"
#include "Inspector.hpp"
#include "ComponentsBrowser.hpp"

namespace Core
{
	class GameObject;
}

namespace Tools
{
	class SceneEditor : public UIWindow
	{
	private:
		static Core::GameObject* m_selectedGameObject;

		//TODO: Add UI Window
		//GameObjectHierarchy
		//Inspector
	public:
		virtual void Init(void) override;
		virtual void OnRender(void) override;
		virtual void Terminate(void) override;

		//Interface
		bool LoadScene();
		bool SaveScene();
		Core::GameObject* GetSelectedGameObject(void);
	};
}
