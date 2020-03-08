#pragma once

#include "Editor.hpp"
#include "Inspector.hpp"
#include "EditorEntity.hpp"


#include <map>
#include <memory>

namespace Tools
{
	using EntityHandle = UNIQUE_HANDLE(EditorEntity);
	using EntityMap = std::map<int, EntityHandle>;

	class SceneEditor : public Editor
	{
	private:
		//static GameObject* m_selectedGameObject;
		static EntityMap m_entitiesMap;
		//TODO: Add UI Window
		//GameObjectHierarchy
		//Inspector
	protected:
		virtual void Init(void) override;
		virtual void Terminate(void) override;
		bool LoadScene(const char*);
		bool SaveScene(const char*);
	public:
		SceneEditor(bool* isOpen) : Editor(EDITOR_TYPE::SCENE_EDITOR, isOpen) {}
		//Interface
		virtual void Update(void) override;
		virtual bool Save(const char* path, const char* returnMessage) override;
		virtual bool Load(const char* path, const char* returnMessage) override;
		//GameObject* GetSelectedGameObject(void);
	};
}
