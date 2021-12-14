#pragma once
#include "EditorWindow/UIWindow.hpp"

namespace Tools
{
	enum GP_STATE {STOP = 0, RUN, PAUSE};
	class GameplayWindow : public UIWindow
	{
	private:
		GP_STATE m_state = GP_STATE::STOP;
	public:
		GameplayWindow() : UIWindow("Gameplay", 600, 600) { }
		virtual void Init() override;
		virtual void PreRender() override;
		virtual void OnRender() override;
		virtual void Terminate() override;
		//Interface to use with buttons
		void RunGame();
		void PauseGame();
		void StopGame();
	};
}


