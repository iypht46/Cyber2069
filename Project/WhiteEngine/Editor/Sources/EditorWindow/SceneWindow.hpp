#pragma once
//Editor
#include "EditorWindow/UIWindow.hpp"
//White Engine
#include "Graphic/CameraObject.hpp"
#include "Graphic/Framebuffer.hpp"

namespace Tools
{
	class SceneWindow : public UIWindow
	{
	private:
		Graphic::CameraObject* m_sceneCam = nullptr;
		Graphic::Framebuffer* m_framebuffer = nullptr;
		void SceneCameraControl();
	public:
		inline SceneWindow(bool* open) : UIWindow("Scene", 600, 600, open, false) { this->Init(); }
		virtual void Init() override;
		virtual void PreRender() override;
		virtual void OnRender() override;
		virtual void Terminate() override;
	};
}


