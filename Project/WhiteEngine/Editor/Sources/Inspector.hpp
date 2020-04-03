#pragma once
#include <vector>
#include <map>

#include "UIWindow.hpp"
#include "EditorEntity.hpp"
#include "EditorComponent.hpp"
#include "EditorObjectRenderer.hpp"

class Component;

namespace Tools
{
	using ComponentList = std::vector<Component*>*;
	
	class Inspector : public UIWindow, public EditorObjectRenderer
	{
	private:
		EditorEntity* m_entityToRender;
		AvailableComponent* m_editorComponentList;
		ImGuiTextFilter filter;
	protected:
		//Inherit Function
		virtual void OnRender(void) override;
		virtual void Init(void) override;
	public:
		//Constructor
		Inspector() : UIWindow("Inspector", 500, 500, new bool(true), true) { this->Init(); }
		virtual void SetEditorObject(EditorObject* obj) override;
		virtual void ResetEditorObject() override;
	};
}