#pragma once
#include "Container/String.hpp"

namespace Tools
{
	class EditorObject
	{
	public:
		EditorObject();
		void Render();
		virtual void Init();
		virtual EditorObject* GetSelected();
		virtual void AddObject(EditorObject* obj);
		virtual void AddObject();
		virtual bool Load(Container::wString path);
		virtual bool Save(Container::wString path);
	protected:
		virtual void PreRender();
		virtual void OnRender() = 0;
		virtual void PostRender();
		static unsigned int m_totalObject;
		static unsigned int m_selectedObject;
		unsigned int m_objectID;
		EditorObject* m_selected;
		bool m_open = false;
	};

	inline unsigned int EditorObject::m_totalObject = 0;
	inline unsigned int EditorObject::m_selectedObject = 0;

	inline void EditorObject::Init() {}
	inline EditorObject* EditorObject::GetSelected() { return m_selected; }
	inline void EditorObject::AddObject(EditorObject* obj) {}
	inline void EditorObject::AddObject() {}
	inline bool EditorObject::Load(Container::wString path) { return false; }
	inline bool EditorObject::Save(Container::wString path) { return false; }

	inline EditorObject::EditorObject() 
	{
		m_objectID = ++m_totalObject;
		this->Init();
	}

	inline void EditorObject::PreRender() {}

	inline void EditorObject::Render()
	{
		PreRender();
		OnRender();
		PostRender();
	}

	inline void EditorObject::PostRender() {}

}