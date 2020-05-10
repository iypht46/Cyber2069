#pragma once
#include "Editor/Editor.hpp"
#include "EditorWindow/PreviewWindow.hpp"
#include "EditorWindow/ParticleSystemWindow.hpp"

namespace Tools
{
	using PreviewHandle = UNIQUE_HANDLE(PreviewWindow);
	using ParticleWindowHandle = UNIQUE_HANDLE(ParticleSystemWindow);
	using ParticleSystemHandle = UNIQUE_HANDLE(ParticleSystem);
	using GameObjectPtrHandle = UNIQUE_HANDLE(GameObject);

	class ParticleEditor : public Editor
	{
	public:
		//Constructor
		ParticleEditor(bool* isOpen);
		ParticleEditor();
		//Inherit Method
		virtual bool Save(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool Load(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool New() override;
		virtual void RenderMenu() override;
		virtual bool IsFocused() override;
		virtual void RenderParticles(glm::mat4 mat);
	protected:
		virtual void Init(void) override;
		virtual void OnUpdate(void) override;
		virtual void Terminate(void) override;
	private:
		//Particle Render Window
		PreviewHandle m_previewWindow;
		bool m_previewBool = true;
		ParticleWindowHandle m_particleWindow;
		//Object
		ParticleSystem* m_particleSystem;
		GameObjectPtrHandle m_gameObj;
	};
}