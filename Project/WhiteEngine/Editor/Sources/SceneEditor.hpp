#pragma once

#include "Editor.hpp"
#include "Inspector.hpp"
#include "ComponentsBrowser.hpp"
#include "Core/EC/GameObject.hpp"

namespace Tools
{
	class SceneEditor : public Editor
	{
	private:
		//static GameObject* m_selectedGameObject;

		//TODO: Add UI Window
		//GameObjectHierarchy
		//Inspector
	public:
		virtual void Init(void) override;
		virtual void Update(void) override;
		virtual void Terminate(void) override;

		//Interface
		bool LoadScene();
		bool SaveScene();
		//GameObject* GetSelectedGameObject(void);
	};
}
