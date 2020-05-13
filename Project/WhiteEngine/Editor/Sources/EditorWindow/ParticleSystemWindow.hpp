#pragma once

#include "EditorWindow/UIWindow.hpp"
#include "EditorObject/ParticleComponentEC.hpp"
#include <imgui.h>
#include <imfilebrowser.h>
#include <string>

namespace Tools
{
	enum class PS_STATE : int
	{ PS_DEFAULT = 0, PS_PLAY, PS_PAUSE};

	class ParticleSystemWindow : public UIWindow
	{
	public:
		ParticleSystemWindow() : UIWindow("Particle System", 300, 600, NULL, false) { this->Init(); }
		~ParticleSystemWindow() { this->Terminate(); }
		void SetObjectToRender(ParticleSystem* particle);
		PS_STATE RunState();
		inline void SetRunState(PS_STATE state) { m_runState = state; } 
	protected:
		virtual void Init() override;
		virtual void OnRender() override;
		virtual void Terminate() override;
		//Internal Function to render particle componenet editor
		void RenderObject(ParticleSystem* particle);
		void RenderObject(ParticleSystem::ParticleEmitter* emitter);
		void RenderObject(ParticleSystem::ParticleLifeTime* lifetime);
		void RenderObject(ParticleSystem::ParticleShape* shape);
		void RenderColorEditor(ParticleSystem* particle);
		void RenderVelocityEditor(ParticleSystem* particle);
		void RenderObject(ParticleSystem::ParticleAnimation* animation);
	private:
		ParticleSystem* m_particleToRender = nullptr;
		ImGui::FileBrowser m_textureFileBrowser;
		ImGui::FileBrowser m_animationFileBrowser;
		PS_STATE m_runState = PS_STATE::PS_DEFAULT;
	};
}