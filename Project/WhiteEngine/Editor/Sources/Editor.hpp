#pragma once
#include "Core/Handle.hpp"
#include "EditorEntity.hpp"

namespace Tools
{

	enum class EDITOR_TYPE { PREFAB_EDITOR, SCENE_EDITOR };

	class Editor
	{
	protected:
		bool m_hasSaved;
		EDITOR_TYPE m_editorType;
		EditorEntity* m_selectedEntity;
	public:
		Editor(EDITOR_TYPE);

		EDITOR_TYPE GetType();
		bool HasSaved();

		virtual void Init(void) = 0;
		virtual void Update(void) = 0;
		virtual void Terminate(void) = 0;
		virtual bool Save(const char* path, const char* returnMessage) = 0;
		virtual bool Load(const char* path, const char* returnMessage) = 0;
		virtual EditorEntity* GetSelectedEntity();
	};

	inline Editor::Editor(EDITOR_TYPE type) : m_editorType(type) {}
	inline EDITOR_TYPE Editor::GetType() { return m_editorType; }
	inline bool Editor::HasSaved() { return m_hasSaved; }
	inline EditorEntity* Editor::GetSelectedEntity() { return m_selectedEntity; }

	
}
