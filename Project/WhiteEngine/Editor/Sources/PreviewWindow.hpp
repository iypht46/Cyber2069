#pragma once

#include "Graphic/Framebuffer.hpp"
#include "Graphic/Shader.hpp"
#include "UIWindow.hpp"

namespace Tools
{
	class PreviewWindow : public UIWindow
	{
	private:
		Graphic::Framebuffer m_framebuffer;
		Graphic::Shader* m_shader;
		unsigned int m_textureID;
		bool m_textureLoaded;
	protected:
		virtual void Init() override;
		virtual void Terminate() override;
		virtual void OnRender() override;
	public:
		PreviewWindow();
		~PreviewWindow();
		bool SetTexture(unsigned int textureID);
	};
}


