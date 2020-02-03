#pragma once

#include <vector>
#include "Editor.hpp"
#include "UIWindow.hpp"

namespace Tools
{
	class PrefabEditor : public Editor
	{
	private:
		std::vector<UIWindow*> m_windows;
	public:
		virtual void Init(void);
		virtual void Update(void);
		virtual void Terminate(void);
	};
}