//Editor
#include "EditorObject/CoreComponentEC.hpp"
#include "EditorObject/ParticleComponentEC.hpp"
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
	MAKE_COMPONENT(ParticleComponentEC);
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
		if (ImGui::DragFloat3("##Scale", &m_scale.x, 0.1f, 0.0f, 1000.0f, "%.1f", 1.0f))
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
			m_isReplaceColor = m_typeComponent->IsReplaceColor();

			if (m_isLoaded)
			{
				m_typeComponent->Init();
				m_textureName = Utility::fs::path(m_typeComponent->GetTexturePath()).filename().generic_string();
			}
				

			if (m_isReplaceColor)
				m_replaceColor = m_typeComponent->GetReplaceColor();
			
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


		ImGui::AlignTextToFramePadding(); ImGui::Text("Replace Color"); ImGui::SameLine(); 
		
		if (ImGui::Checkbox("##rcBool", &m_isReplaceColor))
		{
			m_typeComponent->SetReplaceColorBool(m_isReplaceColor);
		}

		if (m_isReplaceColor)
		{
			if (ImGui::ColorEdit3("##ReplaceColor", &m_replaceColor[0]))
				m_typeComponent->SetReplaceColor(m_replaceColor);
		}
			

		ImGui::PopItemWidth();

		

		//File Dialog Section
		{
			m_textureFileDialog.Display();

			if (m_textureFileDialog.HasSelected())
			{
				m_texturePath = Utility::File::GetRelativePath(m_textureFileDialog.GetSelected(), Utility::File::s_projectPath);

				m_isLoaded = this->AddTexture(m_texturePath.generic_string());

				if (m_isLoaded)
				{
					m_typeComponent->CreateMesh(1.0f, 1.0f);
					//m_typeComponent->
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

		if (ImGui::DragFloat("X##Width", &m_width, 0.1, -500.0f, 500.0f, "%.1f", 1.0f))
		{
			m_typeComponent->ReSize(m_width, m_height);
			dirty = true;
		}
		//ImGui::SameLine(); 

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
			m_controllerPath = m_typeComponent->GetControllerPath();

			if (Utility::fs::exists(m_controllerPath) && Utility::fs::is_regular_file(m_controllerPath))
			{
				m_typeComponent->Init();
				m_controller = m_typeComponent->GetController();
			}
			/* //Make Animation Editor Failed
			else
			{
				m_controller = std::make_shared<AnimationController>();
				m_typeComponent->AssignController(m_controller);
			}*/

			m_framerate = m_typeComponent->GetFramePerSec();

			m_acFileDialog.SetTitle("Choose Animation Controller File");
			m_acFileDialog.SetTypeFilters({ ".animcon" });
		}
		
	}

	void AnimatorEC::SetController(std::shared_ptr<AnimationController> controller)
	{
		if (!controller.get())
			return;

		m_controller = controller;
		m_typeComponent->AssignController(m_controller);
	}

	void AnimatorEC::SetCurrentState(int index)
	{
		if (m_controller.get())
		{
			m_typeComponent->setCurrentState(index);
		}
	}

	void RenderAnimation(Animation* anim);

	void RenderAnimationState(AnimationState* state);

	void AnimatorEC::OnRender()
	{
		ImGui::PushItemWidth(-1);
		if (ImGui::DragFloat("Animation Framerate", &m_framerate, 0.1f, 1.0f, 60.0f, "%.1f"))
		{
			m_typeComponent->setFramePerSec(m_framerate);
		}

		ImGui::TextDisabled(m_typeComponent->GetControllerPath().c_str());
		ImGui::SameLine();

		//Button: Add texture button
		if (ImGui::Button("..##AddACButton"))
		{
			m_acFileDialog.Open();
		}

		if (m_controller.get())
		{
			int  i = 0;
			for (auto it = m_controller->m_states.begin(); it != m_controller->m_states.end(); ++it)
			{
				std::string name = "State " + std::to_string((i + 1));
				if (ImGui::TreeNodeEx(name.c_str()))
				{
					if (ImGui::IsItemClicked())
						m_typeComponent->setCurrentState(i);
					RenderAnimationState(it->get());

					ImGui::TreePop();
				}
				i++;
			}
		}

		ImGui::PopItemWidth();

		m_acFileDialog.Display();

		if (m_acFileDialog.HasSelected())
		{
			bool result = true;
			auto path = Utility::File::GetRelativePath(m_acFileDialog.GetSelected(), Utility::File::s_projectPath);
			
			if (!path.has_filename() || !Utility::fs::is_regular_file(path))
			{
				m_acFileDialog.ClearSelected();
				return;
			}
			
			/*if (m_controller)
			{
				m_controller.reset();
				m_controller = std::make_shared<AnimationController>();
			}*/
			std::shared_ptr<AnimationController> controllerToLoad = std::make_shared<AnimationController>();
			try
			{
				//Serialization::LoadObject(*m_controller, path.generic_string());
				Serialization::LoadObject(*controllerToLoad, path.generic_string());
			}
			catch (const std::exception&)
			{
				ENGINE_WARN("Failed Loading Animation Controller!");
				result = false;
			}

			if (controllerToLoad.get())
			{
				m_controller.reset();
				m_controller = controllerToLoad;
				m_typeComponent->SetControllerPath(path.generic_string());
				m_typeComponent->Init();
				m_typeComponent->GetGameObject()->GetComponent<MeshRenderer>()->CreateMesh(m_controller->getSheetSize().x, m_controller->getSheetSize().y);
			}
			////Load Success
			//if (result)//(m_controller->getSheetSize() != glm::vec2(0.0f, 0.0f))
			//{
			//	m_typeComponent->SetControllerPath(path.generic_string());
			//	m_typeComponent->Init();
			//	//m_typeComponent->AssignController(m_controller);
			//	m_typeComponent->GetGameObject()->GetComponent<MeshRenderer>()->CreateMesh(m_controller->getSheetSize().x, m_controller->getSheetSize().y);
			//}
			//else
			//{
			//	m_controller = m_typeComponent->GetController();
			//}

			m_acFileDialog.ClearSelected();
		}
	}
}