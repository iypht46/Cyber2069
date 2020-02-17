#pragma once
#include "UIWindow.hpp"

namespace Tools
{
	class PreviewWindow : public UIWindow
	{
	private:
		bool m_run;
	public:
		PreviewWindow() : UIWindow("Preview", 800, 800) { Init(); }
		void Init() override;
		void OnRender() override;
		void Terminate() override;
	};
}


