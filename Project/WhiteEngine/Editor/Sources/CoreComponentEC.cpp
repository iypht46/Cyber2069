#include "CoreComponentEC.hpp"
#include "Core/Logger.hpp"

namespace Tools
{
	//*****Prototype Design Variable*****//
	std::map<std::string, EditorComponent*> EditorComponent::m_componentTable;
	AvailableComponent EditorComponent::m_availableComponent;
	MAKE_COMPONENT(TransformEC);
	MAKE_COMPONENT(MeshRendererEC);
	MAKE_COMPONENT(BoxColliderEC);
	MAKE_COMPONENT(RigidbodyEC);
	//*****          END            *****//

	void TransformEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;

		Transform* transform = dynamic_cast<Transform*>(m_component);

		if (transform)
		{
			m_enable = &transform->enabled;
			m_position = &transform->m_position;
			m_scale = &transform->m_scale;
			m_rotation = &transform->m_rotation;
			return;
		}

		ENGINE_ERROR("ERROR::Component is not transfrom");
	}

	void TransformEC::OnRender()
	{
		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Position"); ImGui::SameLine(); ImGui::DragFloat3("##Position", &m_position->x, 0.1, -500.0f, 500.0f, "%.1f", 1.0f);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Scale"); ImGui::SameLine(); ImGui::DragFloat3("##Scale", &m_scale->x, 0.1f, 0.0f, 10.0f, "%.1f", 1.0f);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::DragFloat("##Rotation (Euler)", m_rotation, 0.1f, 0.0f, 360.0f, "%.1f", 1.0f);
		ImGui::PopItemWidth();
	}

	void TransformEC::SetChild(bool isChild)
	{
		Transform* transform = dynamic_cast<Transform*>(m_component);

		if (!transform)
		{
			ENGINE_ERROR("ERROR::Component is not transfrom");
			return;
		}

		if (isChild)
		{
			m_position = &transform->m_localPosition;
			m_scale = &transform->m_localScale;
			m_rotation = &transform->m_localRotation;
		}
		else
		{
			m_position = &transform->m_position;
			m_scale = &transform->m_scale;
			m_rotation = &transform->m_rotation;
		}
	}

	void MeshRendererEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;
		MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(m_component);
		
		if (meshRenderer)
		{
			m_enable = &meshRenderer->enabled;
			m_isLoaded = meshRenderer->m_texLoaded;

			m_textureFileDialog.SetTitle("Choose texture");
			m_textureFileDialog.SetTypeFilters({ ".png", ".jpg" });
			return;
		}
		ENGINE_ERROR("ERROR::Component is not transfrom");
	}

	void MeshRendererEC::RenderTexture(glm::mat4 matrix)
	{
		MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(m_component);
		meshRenderer->Render(matrix);
	}

	bool MeshRendererEC::IsTextureLoaded()
	{
		MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(m_component);

		if (meshRenderer)
		{
			return meshRenderer->m_texLoaded;
		}

		return false;
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
			m_textureName = m_textureFileDialog.GetSelected().filename().string();
			m_isLoaded = true;
			MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(m_component);
			meshRenderer->SetTexture(m_textureFileDialog.GetSelected().string());
			m_textureFileDialog.ClearSelected();
		}
	}

	bool MeshRendererEC::AddTexture(std::string path)
	{
		MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(m_component);
		meshRenderer->SetTexture(path);
		return true;
	}

	void BoxColliderEC::Init(Component* engineComponent)
	{
		m_component = engineComponent;
		BoxCollider* boxCol = dynamic_cast<BoxCollider*>(m_component);

		if (boxCol)
		{
			m_enable = &boxCol->enabled;
			m_width = &boxCol->m_halfWidth;
			m_height = &boxCol->m_halfHeight;
			m_colliderScale = &boxCol->m_colliderScale;
			m_enable = &boxCol->enabled;

			return;
		}
		ENGINE_ERROR("ERROR::Component is not transfrom");
	}

	void BoxColliderEC::OnRender()
	{
		ImGui::PushItemWidth(-1); 
		ImGui::AlignTextToFramePadding(); ImGui::Text("Size"); 
		ImGui::SameLine(); ImGui::DragFloat("X##Width", m_width, 0.1, -500.0f, 500.0f, "%.1f", 1.0f);
		ImGui::SameLine(); ImGui::DragFloat("Y##Height", m_height, 0.1, -500.0f, 500.0f, "%.1f", 1.0f);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Scale"); 
		ImGui::SameLine(); ImGui::DragFloat2("##Scale", &m_colliderScale->x, 0.1f, 0.0f, 10.0f, "%.1f", 1.0f);
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
		Rigidbody* rigid = dynamic_cast<Rigidbody*>(m_component);

		if (rigid)
		{
			m_enable = &rigid->enabled;
			m_velocity = &rigid->m_velocity;
			m_acceleration = &rigid->m_acceleration;
			m_mass = &rigid->m_mass;
			m_gravityScale = &rigid->m_gravityScale;
			m_drag = &rigid->m_drag;
		}
	}
}