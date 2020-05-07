#include "ParticleEditor.hpp"
#include "Serialization/Serialization.h"
#include "Core/GameInfo.h"
#include "Physic/PhysicScene.hpp"

namespace Tools
{

	ParticleEditor::ParticleEditor(bool* isOpen) : Editor("Particle System", isOpen)
	{
		this->Init();
	}

	ParticleEditor::ParticleEditor() : Editor("Particle System")
	{
		this->Init();
	}

	void ParticleEditor::Init(void)
	{
		//Initialize Object
		
		//Initialize Window
		m_previewWindow = std::make_unique<PreviewWindow>(&m_previewBool, "Particle System Preview", false, 400, 400);
		RenderFunction customRenderFunc = [=](glm::mat4 mat) { this->RenderParticles(mat); };
		m_previewWindow->SetCustomRenderFunction(customRenderFunc);
		m_particleWindow = std::make_unique<ParticleSystemWindow>();
		m_gameObj = std::make_unique<GameObject>();
		//Initialize File dialog window
		m_fileDialog = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename
			| ImGuiFileBrowserFlags_CloseOnEsc
			| ImGuiFileBrowserFlags_CreateNewDir);
		m_fileDialog->SetTitle("Particle Save/Load Directory");
		m_fileDialog->SetTypeFilters({ ".ptcl" });
	}

	void ParticleEditor::OnUpdate(void)
	{
		if (!m_open)
			return;

		//If user attempt to close window
		if (!m_previewBool)
		{
			if (m_dirtyFlag)
			{
				//Add to pop up list and attach callback to popup
			}
			else
			{
				m_previewBool = true;
				//Toggle Modals
				//Add to pop up list and attach callback to popup
			}
		}
		else
		{
			auto renderState = m_particleWindow->RunState();

			m_previewWindow->Render();
		}

		m_particleWindow->Render();
	}

	void ParticleEditor::Terminate(void)
	{

	}

	bool ParticleEditor::Save(Utility::fs::path path, Container::wString& returnMessage)
	{
		if (!m_particleSystem)
			return false;

		if (path.has_filename())
		{
			path.replace_extension(".ptcl");

			bool result = true;

			try
			{
				Serialization::SaveObject(*m_particleSystem, path.generic_string());
			}
			catch (const std::exception&)
			{
				ENGINE_WARN("Failed Saving Particle System!");
				result = false;
			}
			
			returnMessage = (result) ? (returnMessage + "Success") : (returnMessage + "Failed");

			if (result)
				this->SetDirty(false);
			else
				this->SetDirty(true);

			return result;
		}

		return false;
	}

	bool ParticleEditor::Load(Utility::fs::path path, Container::wString& returnMessage)
	{
		bool result = false;
		if (path.has_filename() && Utility::fs::is_regular_file(path))
		{
			
			//If object is dirty
			if (this->IsDirty())
			{
				//Send to close queue

			}
			else
			{
				result = true;
				//m_sceneObject.reset(); //Clear entity
				//m_sceneObject = make_unique<EditorSceneObject>(); //Make new entity
				if (!m_particleSystem)
					this->New();

				try
				{
					Serialization::LoadObject(*m_particleSystem, path.generic_string());
				}
				catch (const std::exception&)
				{
					ENGINE_WARN("Failed Loading Particle System!");
					result = false;
				}

				if (result)
					m_particleWindow->SetObjectToRender(m_particleSystem);

				returnMessage = (result) ? (returnMessage + "Success") : (returnMessage + "Failed");
			}
		}

		return result;
	}

	bool ParticleEditor::New()
	{
		if (m_particleWindow->RunState() != PS_STATE::PS_DEFAULT)
		{
			m_particleWindow->SetRunState(PS_STATE::PS_DEFAULT);
			m_particleSystem->Reset();
		}

		m_particleSystem = m_gameObj->AddComponent<ParticleSystem>();
		m_particleWindow->SetObjectToRender(m_particleSystem);

		return true;

		/*if (m_particleSystem)
		{
			return false;
		}
		else
		{
			m_particleSystem = new ParticleSystem();
			return true;
		}*/
	}

	void ParticleEditor::RenderMenu()
	{
		if (!m_fileDialog)
			return;

		if (ImGui::BeginMenu((this->GetName() + " Editor").c_str()))
		{
			if (ImGui::MenuItem("Save"))
			{
				m_action = EDITOR_ACTION::EDITOR_SAVE;
				m_fileDialog->Open();
			}

			if (ImGui::MenuItem("Load"))
			{
				m_action = EDITOR_ACTION::EDITOR_LOAD;
				m_fileDialog->Open();
			}

			if (ImGui::MenuItem("New"))
			{
				this->New();
			}

			ImGui::EndMenu();
		}
	}

	bool ParticleEditor::IsFocused()
	{
		bool focus = (m_previewWindow->isFocused() | m_particleWindow->isFocused());
		m_gameObj->SetActive(focus);
		return focus;
	}

	void ParticleEditor::RenderParticles(glm::mat4 mat)
	{
		if (!m_particleSystem)
			return;

		auto renderState = m_particleWindow->RunState();

		if (renderState == PS_STATE::PS_DEFAULT)
			return;

		//If is playing then update the particles
		if (renderState == PS_STATE::PS_PLAY)
		{
			auto dt = World::GameInfo::GetInstance().m_deltaTime;
			Physic::PhysicScene::GetInstance()->Update(dt);
			m_particleSystem->ConstantEmit(dt);
			m_particleSystem->LifeTimeModification(dt);
		}

		//If playing or pausing render the particles
		for (auto it : m_particleSystem->GetSpawnedParticles())
		{
			if (!it)
				continue;

			MeshRenderer* mesh = it->GetComponent<MeshRenderer>();

			if (!mesh)
				continue;

			mesh->Render(mat);
		}
	}

}

