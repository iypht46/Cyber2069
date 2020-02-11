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
		SceneEditor() : Editor(EDITOR_TYPE::SCENE_EDITOR) {}
		virtual void Init(void) override;
		virtual void Update(void) override;
		virtual void Terminate(void) override;
		virtual bool Save(const char*) override;
		virtual bool Load(const char*) override;

		//Interface
		bool LoadScene();
		bool SaveScene();
		//GameObject* GetSelectedGameObject(void);
	};
}
