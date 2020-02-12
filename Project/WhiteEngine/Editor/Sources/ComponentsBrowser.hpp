#pragma once
#include <vector>

#include "UIWindow.hpp"

class Component;

namespace Tools
{
	using ComponentList = std::vector<Component*>*;
	class ComponentsBrowser : public UIWindow
	{
	private:
		const ComponentList m_componentLists;
	protected:
		//Inherit Function
		virtual void OnRender(void) override;
		virtual void Init(void) override;
		virtual void Terminate(void) override;
	public:
		//Constructor
		ComponentsBrowser(ComponentList list) : UIWindow("Components List"), m_componentLists(list) { }
	};
}
