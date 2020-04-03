#pragma once
//White Engine
#include "Core/Handle.hpp"
//From Editor
#include "EditorObject.hpp"
//Third Party Library

//Standard Library
#include <utility>

namespace Tools
{
	//using SceneObjectHandle = UNIQUE_HANDLE(SceneObject);

	class EditorSceneObject : public EditorObject
	{
	private:
		
	public:
		EditorSceneObject();
		//EditorSceneObject(SceneObject* obj);
		~EditorSceneObject();
		virtual void OnRender() override;
	};
}


