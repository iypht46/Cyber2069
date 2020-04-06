#include "CoreComponentEC.hpp"
#include "Core/Logger.hpp"
#include "misc/cpp/imgui_stdlib.h"
#include "Utility/Filesystem.hpp"

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
			//m_position = &m_typeComponent->m_position;
			//m_scale = &m_typeComponent->m_scale;
			//m_rotation = &m_typeComponent->m_rotation;
			return;
		}

		ENGINE_ERROR("ERROR::Component is not transfrom");
	}

	void TransformEC::OnRender()
	{
		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Position"); ImGui::SameLine(); 
		if (ImGui::DragFloat3("##Position", &m_position.x, 1.0f, -500.0f, 500.0f, "%.1f", 1.0f))
		{
			m_typeComponent->SetPosition(m_position);
		}
		ImGui::AlignTextToFramePadding(); ImGui::Text("Scale"); ImGui::SameLine(); 
		if (ImGui::DragFloat3("##Scale", &m_scale.x, 1.0f, 0.0f, 1000.0f, "%.1f", 1.0f))
		{
			m_typeComponent->SetScale(m_scale);
		}
		ImGui::AlignTextToFramePadding(); ImGui::Text("Rotation"); ImGui::SameLine(); 
		if (ImGui::DragFloat("##Rotation", &m_rotation, 0.1f, 0.0f, 360.0f, "%.1f", 1.0f))
		{
			m_typeComponent->SetRotation(m_rotation);
		}
		ImGui::PopItemWidth();
	}

	void TransformEC::SetChild(bool isChild)
	{
		if (!m_typeComponent)
		{
			ENGINE_ERROR("ERROR::Component is not transfrom");
			return;
		}

		/*if (ischild)
		{
			m_position = &transform->m_localposition;
			m_scale = &transform->m_localscale;
			m_rotation = &transform->m_localrotation;
		}
		else
		{
			m_position = &transform->m_position;
			m_scale = &transform->m_scale;
			m_rotation = &transform->m_rotation;
		}*/
	}

	void MeshRendererEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;
		m_typeComponent = dynamic_cast<MeshRenderer*>(m_component);
		
		if (m_typeComponent)
		{
			m_isLoaded = m_typeComponent->m_texLoaded;

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
		return m_isLoaded;
	}

	void MeshRendererEC::OnRender()
	{
		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Sprite"); ImGui::SameLine();
		if (!m_isLoaded)
			ImGui::TextDisabled("None");
		else
			ImGui::TextDisabled(m_textureName.c_str());
		ImGui::SameLine();
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
				
			}
			else
			{
				//Toggle Pop Up
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

		return m_typeComponent->m_texLoaded;
	}

	void BoxColliderEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;
		m_typeComponent = dynamic_cast<BoxCollider*>(m_component);

		if (m_typeComponent)
		{
			/*m_width = &boxCol->m_halfWidth;
			m_height = &boxCol->m_halfHeight;
			m_colliderScale = &boxCol->m_colliderScale;*/

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
			m_typeComponent->m_halfWidth = m_width;
		}
		ImGui::SameLine(); 

		if (ImGui::DragFloat("Y##Height", &m_height, 0.1, -500.0f, 500.0f, "%.1f", 1.0f))
		{
			m_typeComponent->m_halfHeight = m_height;
		}
		ImGui::AlignTextToFramePadding(); ImGui::Text("Scale"); 
		ImGui::SameLine(); 
		if (ImGui::DragFloat2("##Scale", &m_colliderScale.x, 0.1f, 0.0f, 10.0f, "%.1f", 1.0f))
		{
			m_typeComponent->m_colliderScale = m_colliderScale;
		}
		ImGui::PopItemWidth();
	}
	
	void RigidbodyEC::OnRender()
	{
		//ImGui::PushItemWidth(-1);

		ImGui::AlignTextToFramePadding(); ImGui::Text("Velocity");
		ImGui::SameLine(); ImGui::DragFloat3("##Velocity", &m_velocity->x, 0.0f, -500.0f, 500.0f, "%.1f", 1.0f);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Acceleration");
		ImGui::SameLine(); ImGui::DragFloat3("##Acceleration", &m_acceleration->x, 0.0f, -500.0f, 500.0f, "%.1f", 1.0f);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Mass");
		ImGui::SameLine(); ImGui::DragFloat("##Mass", m_mass, 0.1f, 0.0f, 500.0f, ".1f", 1.0f);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Gravity Scale");
		ImGui::SameLine(); ImGui::DragFloat("##GravityScale", m_gravityScale, 0.1, 0.0f, 500.0f, ".1f", 1.0f);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Drag");
		ImGui::SameLine(); ImGui::DragFloat("##Drag", m_drag, 0.1f, 0.0f, 500.0f, ".1f", 1.0f);
		//ImGui::PopItemWidth();
	}
	
	void RigidbodyEC::Init(Component * engineComponent)
	{
		m_component = engineComponent;
		m_typeComponent = dynamic_cast<Rigidbody*>(m_component);

		if (m_typeComponent)
		{
			m_velocity = &m_typeComponent->m_velocity;
			m_acceleration = &m_typeComponent->m_acceleration;
			m_mass = &m_typeComponent->m_mass;
			m_gravityScale = &m_typeComponent->m_gravityScale;
			m_drag = &m_typeComponent->m_drag;
		}
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

	void AnimatorEC::RenderAnimation(Animation * anim)
	{
		/*ImGui::PushItemWidth(-1);
		ImGui::InputText("State Name", &anim->stateName);
		ImGui::Checkbox("Looping", &anim->m_looping);
		ImGui::InputFloat2("Start Position", &anim->m_StartPosition.x, 0, ImGuiInputTextFlags_CharsNoBlank);
		ImGui::InputFloat2("End Position", &anim->m_EndPosition.x, 0, ImGuiInputTextFlags_CharsNoBlank);
		ImGui::InputInt("Speed", &anim->speedMultiplier);
		ImGui::PopItemWidth();*/
	}

	void AnimatorEC::RenderAnimationController()
	{
		/*ImGui::InputFloat2("Sheet Size", &m_animationController->m_spriteSheetFrameSize.x, 0, ImGuiInputTextFlags_CharsNoBlank);
		int i = 1;
		for (auto it = m_animationController->m_states.begin(); it != m_animationController->m_states.end(); ++it, ++i)
		{
			Animation* anim = (*it);

			if (ImGui::TreeNode(anim->stateName.c_str()))
			{
				RenderAnimation(anim);
				ImGui::TreePop();
			}
		}

		if (ImGui::Button("Add State"))
		{
			m_animationController->AddState(new Animation());
		}*/
	}

	void AnimatorEC::OnRender()
	{
		ImGui::DragFloat("Animation Framerate", m_framerate, 0.1f, 1.0f, 60.0f, "%.1f");
		RenderAnimationController();
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