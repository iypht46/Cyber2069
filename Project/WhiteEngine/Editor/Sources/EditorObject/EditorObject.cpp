#include "EditorObject/EditorObject.hpp"

namespace Tools
{
	unsigned int EditorObject::m_totalObject = 0;
	unsigned int EditorObject::m_selectedObject = 0;

	void EditorObject::Init() {}
	EditorObject* EditorObject::GetSelected() { return m_selected; }
	void EditorObject::AddObject(EditorObject* obj) {}
	void EditorObject::AddObject() {}
	bool EditorObject::Load(Container::wString path) { return false; }
	bool EditorObject::Save(Container::wString path) { return false; }

	EditorObject::EditorObject()
	{
		m_objectID = ++m_totalObject;
		this->Init();
	}

	void EditorObject::PreRender() {}

	void EditorObject::Render()
	{
		PreRender();
		OnRender();
		PostRender();
	}

	void EditorObject::PostRender() {}
}