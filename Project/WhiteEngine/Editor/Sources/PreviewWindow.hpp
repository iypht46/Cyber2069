#pragma once
#include "UIWindow.hpp"

namespace Tools
{
	class PreviewWindow : public UIWindow
	{
	private:

	public:
		PreviewWindow() : UIWindow("Preview") { Init(); }
		void Init() override;
		void OnRender() override;
		void Terminate() override;
	};
}


