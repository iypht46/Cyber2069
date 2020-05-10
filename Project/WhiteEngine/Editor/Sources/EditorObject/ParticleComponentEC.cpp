#include "ParticleComponentEC.hpp"
#include "EditorWindow/PopupWindow.hpp"
#include "Utility/Filesystem.hpp"
#include "Serialization/Serialization.h"
#include <imgui.h>


namespace Tools
{
	ParticleComponentEC::ParticleComponentEC() : EditorComponent("ParticleComponentEC")
	{
		

	}

	void ParticleComponentEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;
		m_typeComponent = dynamic_cast<ParticleSystem*>(engineComponent);
		m_particleFileBrowser.SetTitle("Select Particle System");
		m_particleFileBrowser.SetTypeFilters({ ".ptcl" });

		if (!m_typeComponent)
			ENGINE_ERROR("ERROR::Component is not particle");
	}

	bool ParticleComponentEC::IsLoaded()
	{
		return (m_typeComponent);
	}

	void ParticleComponentEC::OnRender()
	{
		ImGui::PushItemWidth(-1);

		ImGui::Text("Load Particle System:");
		
		ImGui::TextDisabled(m_particleFileName.c_str()); ImGui::SameLine();
		if (ImGui::Button(".."))
			m_particleFileBrowser.Open();

		m_particleFileBrowser.Display();

		if (m_particleFileBrowser.HasSelected())
		{
			bool loadResult = true;
			auto path = Utility::File::GetRelativePath(m_particleFileBrowser.GetSelected(), Utility::File::s_projectPath);

			if (path.has_filename() && Utility::fs::is_regular_file(path))
			{
				try
				{
					Serialization::LoadObject(*m_typeComponent, path.generic_string());
				}
				catch (const std::exception&)
				{
					ENGINE_WARN("Failed Loading Particle System!");
					loadResult = false;
				}

				if (loadResult)
				{
					m_particleFileName = path.filename().generic_string();
				}
				else
				{
					PopupData data("Load Particle:", "Failed loading " + path.filename().generic_string());
					PopupWindow::GetGlobalPopup().Push(data);
				}
			}

			m_particleFileBrowser.ClearSelected();
		}

		ImGui::PopItemWidth();
	}
}

