#pragma once
#include "EditorObject/EditorComponent.hpp"
#include "Core/Particle/ParticleSystem.h"
#include <unordered_map>
#include <imgui.h>
#include <imfilebrowser.h>

namespace Tools
{
	class ParticleComponentEC : public EditorComponent
	{
	public:
		ParticleComponentEC();
		virtual void Init(Component* engineComponent) override;
		bool IsLoaded();
	protected:
		IMPLEMENT_CLONE(ParticleComponentEC);
		virtual void OnRender() override;
	private:
		INIT_COMPONENT(ParticleSystem);
		ImGui::FileBrowser m_particleFileBrowser;
		std::string m_particleFileName = "none";
	};
}