#pragma once
//Editor
#include "EditorWindow/UIWindow.hpp"
#include "EditorWindow/PopupWindow.hpp"
#include "EditorObject/EditorObjectRenderer.hpp"
#include "EditorObject/EditorEntity.hpp"
//White Engine
#include "Utility/Filesystem.hpp"
#include "Core/Animation/AnimationController.hpp"
#include "Core/Handle.hpp"
//Third Party
#include <imfilebrowser.h>

namespace Tools
{

	class AnimationWindow : public UIWindow, public EditorObjectRenderer
	{
	public:
		AnimationWindow() : UIWindow("Animation Controller Creator", 600, 400, NULL, false) { this->Init(); }
		~AnimationWindow() { this->Terminate(); }
		virtual void SetEditorEntity(EditorEntity* obj);
	protected:
		virtual void OnRender() override;
		EditorEntity* m_entity = nullptr;
	};

	
}