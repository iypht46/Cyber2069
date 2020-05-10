#pragma once

#include "EditorObject.hpp"

namespace Tools
{
	class EditorObjectRenderer
	{
	protected:
		EditorObject* m_objectToRender;
	public:
		virtual void SetEditorObject(EditorObject* obj);
		virtual EditorObject* GetEditorObject();
		virtual void ResetEditorObject();
	};

	inline void EditorObjectRenderer::SetEditorObject(EditorObject* obj)
	{
		if (obj)
			m_objectToRender = obj;
	}

	inline EditorObject* EditorObjectRenderer::GetEditorObject()
	{
		return m_objectToRender;
	}

	inline void EditorObjectRenderer::ResetEditorObject()
	{
		m_objectToRender = nullptr;
	}
}
