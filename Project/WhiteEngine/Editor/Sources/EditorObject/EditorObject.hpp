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
}