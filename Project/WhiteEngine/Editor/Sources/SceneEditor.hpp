#pragma once

#include "Editor.hpp"
#include "Inspector.hpp"
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
	protected:
		virtual void Init(void) override;
		virtual void Terminate(void) override;
		bool LoadScene(const char*);
		bool SaveScene(const char*);
	public:
		SceneEditor() : Editor(EDITOR_TYPE::SCENE_EDITOR) {}
		//Interface
		virtual void Update(void) override;
		virtual bool Save(const char* path, const char* returnMessage) override;
		virtual bool Load(const char* path, const char* returnMessage) override;
		//GameObject* GetSelectedGameObject(void);
	};
}
