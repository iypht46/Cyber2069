#include "ParticleComponentEC.hpp"
#include "Utility/Filesystem.hpp"
#include "Serialization/Serialization.h"
#include <imgui.h>


namespace Tools
{
	ParticleComponentEC::ParticleComponentEC() : EditorComponent("ParticleComponentEC")
	{
		m_particleFileBrowser.SetTitle("Select Particle System");
		m_particleFileBrowser.SetTypeFilters({ ".ptcl" });

	}

	void ParticleComponentEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;
		m_typeComponent = dynamic_cast<ParticleSystem*>(engineComponent);

		ENGINE_ERROR("ERROR::Component is not particle");
	}

	bool ParticleComponentEC::IsLoaded()
	{
		return (m_typeComponent);
	}

	void ParticleComponentEC::OnRender()
	{
		ImGui::PushItemWidth(-1);

		ImGui::Text("Load Particle System");
		if (ImGui::Button("Load"))
		{
			m_particleFileBrowser.Open();
		}

		m_particleFileBrowser.Display();

		if (m_particleFileBrowser.HasSelected())
		{
			auto path = Utility::File::GetRelativePath(m_particleFileBrowser.GetSelected(), Utility::File::s_projectPath);

			if (path.has_filename() && Utility::fs::is_regular_file(path))
			{
				try
				{
					Serialization::LoadObject(*m_typeComponent, path.generic_string());
				}
				catch (const std::exception&)
				{
					ENGINE_WARN("Failed Loading Animation Controller!");
				}
			}
		}

		ImGui::PopItemWidth();
	}
}

