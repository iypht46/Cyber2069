//Editor
#include "EditorObject/CoreComponentEC.hpp"
//White Engine
#include "Core/Logger.hpp"
#include "Utility/Filesystem.hpp"
//Third Party
#include "misc/cpp/imgui_stdlib.h"


namespace Tools
{
	//*****Prototype Design Variable*****//
	std::map<std::string, EditorComponent*> EditorComponent::m_componentTable;
	AvailableComponent EditorComponent::m_availableComponent;
	MAKE_COMPONENT(TransformEC);
	MAKE_COMPONENT(MeshRendererEC);
	MAKE_COMPONENT(BoxColliderEC);
	MAKE_COMPONENT(RigidbodyEC);
	MAKE_COMPONENT(AnimatorEC);
	MAKE_COMPONENT(SoundPlayerEC);
	//*****          END            *****//

	void TransformEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;

		//Transform* transform = dynamic_cast<Transform*>(m_component);
		m_typeComponent = dynamic_cast<Transform*>(engineComponent);
		if (m_typeComponent)
		{
			m_position = m_typeComponent->GetLocalPosition();
			m_rotation = m_typeComponent->GetLocalRotation();
			m_scale = m_typeComponent->GetScale();
			return;
		}

		ENGINE_ERROR("ERROR::Component is not transfrom");
	}

	void TransformEC::OnRender()
	{
		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Position"); ImGui::SameLine(); 
		if (ImGui::DragFloat3("##Position", &m_position.x, 0.1f, -99999.0f, 99999.0f, "%.1f", 1.0f))
		{
			m_typeComponent->SetLocalPosition(m_position);
			dirty = true;
		}
		ImGui::AlignTextToFramePadding(); ImGui::Text("Scale"); ImGui::SameLine(); 
		if (ImGui::DragFloat3("##Scale", &m_scale.x, 1.0f, 0.0f, 1000.0f, "%.1f", 1.0f))
		{
			m_typeComponent->SetLocalScale(m_scale);
			dirty = true;
		}
		ImGui::AlignTextToFramePadding(); ImGui::Text("Rotation"); ImGui::SameLine(); 
		if (ImGui::DragFloat("##Rotation", &m_rotation, 0.1f, -180.0f, 180.0f, "%.1f", 1.0f))
		{
			m_typeComponent->SetLocalRotation(m_rotation);
			dirty = true;
		}
		ImGui::PopItemWidth();
	}

	void TransformEC::SetParent(TransformEC* other)
	{
		m_typeComponent->SetParent(other->m_typeComponent->GetGameObject()->m_transform);		
	}

	void MeshRendererEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;
		m_typeComponent = dynamic_cast<MeshRenderer*>(m_component);
		
		if (m_typeComponent)
		{
			m_isLoaded = m_typeComponent->IsTextureLoaded();

			if (m_isLoaded)
				m_typeComponent->Init();
			
			//m_isUI = m_typeComponent->IsUI();
			m_textureFileDialog.SetTitle("Choose texture");
			m_textureFileDialog.SetTypeFilters({ ".png", ".jpg" });
			return;
		}
		ENGINE_ERROR("ERROR::Component is not transfrom");
	}

	void MeshRendererEC::RenderTexture(glm::mat4 matrix)
	{
		m_typeComponent->Render(matrix);
	}

	bool MeshRendererEC::IsTextureLoaded()
	{
		return m_typeComponent->IsTextureLoaded();
	}

	void MeshRendererEC::OnRender()
	{
		ImGui::PushItemWidth(-1);
		//Checkbox: UI
		ImGui::AlignTextToFramePadding(); ImGui::Text("UI"); ImGui::SameLine();
		if (ImGui::Checkbox("##MeshUI", &m_isUI))
		{
			m_typeComponent->SetUI(m_isUI);
			dirty = true;
		}
			

		//Text: Texture name
		ImGui::AlignTextToFramePadding(); ImGui::Text("Sprite"); ImGui::SameLine();
		if (!m_isLoaded)
			ImGui::TextDisabled("None");
		else
			ImGui::TextDisabled(m_textureName.c_str());
		ImGui::SameLine();

		//Button: Add texture button
		if (ImGui::Button("..##AddTextureButton"))
		{
			m_textureFileDialog.Open();
		}
		ImGui::PopItemWidth();

		m_textureFileDialog.Display();

		if (m_textureFileDialog.HasSelected())
		{
			m_texturePath = Utility::File::GetRelativePath(m_textureFileDialog.GetSelected(), Utility::File::s_projectPath);

			m_isLoaded = this->AddTexture(m_texturePath.generic_string());

			if (m_isLoaded)
			{
				m_typeComponent->CreateMesh(1.0f, 1.0f);
				m_textureName = m_texturePath.filename().generic_string();
				ENGINE_INFO("Load texture: " + m_textureName + " success.");
				dirty = true;
			}
			else
			{
				//Toggle Pop Up
				ENGINE_ERROR("ERROR: Failed loading texture file: " + m_textureName);
			}
			//std::cout << "Texture File Path: " << m_texturePath.generic_string() << std::endl;
			/*std::cout << "Texture Absolute Path: " << m_textureFileDialog.GetSelected().string() << std::endl;
			std::cout << "Texture Relative Path: " << m_textureFileDialog.GetSelected().relative_path().string() << std::endl;*/
			//meshRenderer->SetTexture(m_textureName);
			//meshRenderer->SetTexture("Assets/rabbit.png");
			
			m_textureFileDialog.ClearSelected();
		}
	}

	bool MeshRendererEC::AddTexture(std::string path)
	{
		//MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(m_component);
		m_typeComponent->SetTexture(path);

		return m_typeComponent->IsTextureLoaded();
	}

	void BoxColliderEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;
		m_typeComponent = dynamic_cast<BoxCollider*>(m_component);

		if (m_typeComponent)
		{
			m_width = m_typeComponent->GetHw();
			m_height = m_typeComponent->GetHh();

			return;
		}
		ENGINE_ERROR("ERROR::Component is not transfrom");
	}

	void BoxColliderEC::OnRender()
	{
		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Size");
		ImGui::SameLine();
		if (ImGui::DragFloat("X##Width", &m_width, 0.1, -500.0f, 500.0f, "%.1f", 1.0f))
		{
			m_typeComponent->ReSize(m_width, m_height);
			dirty = true;
		}
		ImGui::SameLine(); 

		if (ImGui::DragFloat("Y##Height", &m_height, 0.1, -500.0f, 500.0f, "%.1f", 1.0f))
		{
			m_typeComponent->ReSize(m_width, m_height);
			dirty = true;
		}
		ImGui::PopItemWidth();
	}
	
	void RigidbodyEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;
		m_typeComponent = dynamic_cast<Rigidbody*>(m_component);

		if (m_typeComponent)
		{
			m_mass = m_typeComponent->GetMass();
			m_gravityScale = m_typeComponent->GetGravityScale();
			m_drag = m_typeComponent->GetDrag();
		}
	}

	void RigidbodyEC::OnRender()
	{
		//ImGui::PushItemWidth(-1);
		
		ImGui::AlignTextToFramePadding(); ImGui::Text("Mass");
		ImGui::SameLine(); 
		if (ImGui::DragFloat("##Mass", &m_mass, 0.1f, 0.0f, 500.0f, ".1f", 1.0f))
		{
			m_typeComponent->SetMass(m_mass);
			dirty = true;
		}
		
		ImGui::AlignTextToFramePadding(); ImGui::Text("Gravity Scale");
		ImGui::SameLine(); 
		if (ImGui::DragFloat("##GravityScale", &m_gravityScale, 0.1, 0.0f, 500.0f, ".1f", 1.0f))
		{
			m_typeComponent->SetGravityScale(m_gravityScale);
			dirty = true;
		}
		
		ImGui::AlignTextToFramePadding(); ImGui::Text("Drag");
		ImGui::SameLine(); 
		if (ImGui::DragFloat("##Drag", &m_drag, 0.1f, 0.0f, 500.0f, ".1f", 1.0f))
		{
			m_typeComponent->SetDrag(m_drag);
			dirty = true;
		}
		//ImGui::PopItemWidth();
	}
	
	void AnimatorEC::Init(Component * animatorComponent)
	{
		m_component = animatorComponent;
		m_typeComponent = dynamic_cast<Animator*>(m_component);
		
		if (m_typeComponent)
		{
			m_framerate = &m_typeComponent->framePerSec;

			if (!m_typeComponent->m_controller)
			{
				//m_typeComponent->m_controller = new AnimationController();
			}

			//m_animationController = m_typeComponent->m_controller;
		}
		
	}

	void AnimatorEC::OnRender()
	{
		ImGui::DragFloat("Animation Framerate", m_framerate, 0.1f, 1.0f, 60.0f, "%.1f");
	}

	void SoundPlayerEC::Init(Component * soundPlayerComponent)
	{
		m_component = soundPlayerComponent;
		m_typeComponent = dynamic_cast<SoundPlayer*>(m_component);

		if (m_typeComponent)
		{

		}
	}

	void SoundPlayerEC::OnRender()
	{

	}
}