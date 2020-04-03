#pragma once

namespace Tools
{
	class EditorObject
	{
	public:
		EditorObject();
		virtual void PreRender() {}
		virtual void OnRender() = 0;
		virtual void PostRender() {}
		virtual void Init() {}
		void Render();
	protected:

	};

	inline EditorObject::EditorObject() 
	{
		this->Init();
	}

	inline void EditorObject::Render()
	{
		this->PreRender();
		this->OnRender();
		this->PostRender();
	}

}