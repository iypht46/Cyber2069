#pragma once
#include "Editor/Editor.hpp"
#include "EditorObject/EditorEntity.hpp"
#include "Core/Handle.hpp"
#include "EditorWindow/PreviewWindow.hpp"
#include "EditorWindow/AnimationWindow.hpp"
#include "EditorObject/EditorAC.hpp"

namespace Tools
{
	using EntityHandle = UNIQUE_HANDLE(EditorEntity);
	using PreviewHandle = UNIQUE_HANDLE(PreviewWindow);
	using AnimationHandle = UNIQUE_HANDLE(AnimationWindow);
	using ACHandle = UNIQUE_HANDLE(EditorAC);

	class ACEditor : public Editor
	{
	public:
		ACEditor(bool* isOpen);
		ACEditor();
		
		virtual bool Save(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool Load(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool New() override;
		virtual void RenderMenu() override;
		virtual bool IsFocused() override;
	protected:
		virtual void Init(void) override;
		virtual void OnUpdate(void) override;
		virtual void Terminate(void) override;
		
	private:
		//TODO: Controller editor window

		//TODO: Entity animation tester
		PreviewHandle m_previewWindow;
		bool m_previewBool = true;
		//Animation
		AnimationHandle m_animationWindow;
		EntityHandle m_entityACTester;
		AnimatorEC* m_animatorEC;
		MeshRendererEC* m_meshEC;
		//TODO: Animation Controller Object
		ACHandle m_acObject;
	};
}



