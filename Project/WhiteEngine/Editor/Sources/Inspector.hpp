#pragma once
#include <vector>

#include "UIWindow.hpp"
#include "EditorEntity.hpp"

class Component;

namespace Tools
{
	using ComponentList = std::vector<Component*>*;
	class Inspector : public UIWindow
	{
	private:
		EditorEntity* m_entityToRender;
		std::vector<EditorComponent*>* m_componentList;
	protected:
		//Inherit Function
		virtual void OnRender(void) override;
		virtual void Init(void) override;
	public:
		//Constructor
		Inspector() : UIWindow("Inspector") { this->Init(); }
		void SetEntity(EditorEntity*);
	};
}