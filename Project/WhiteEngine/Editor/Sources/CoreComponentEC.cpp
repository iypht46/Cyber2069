#include "CoreComponentEC.hpp"
#include "imgui.h"
#include "Core/Logger.hpp"

namespace Tools
{
	//*****Prototype Design Variable*****//
	std::map<std::string, EditorComponent*> EditorComponent::m_componentTable;
	AvailableComponent EditorComponent::m_availableComponent;
	MAKE_COMPONENT(TransformEC);
	MAKE_COMPONENT(MeshRendererEC);
	MAKE_COMPONENT(BoxColliderEC);
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
			m_tex = &meshRenderer->texture;
			return;
		}
		ENGINE_ERROR("ERROR::Component is not transfrom");
	}

	void MeshRendererEC::OnRender()
	{
		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding(); ImGui::Text("Sprite"); ImGui::SameLine();
		if (!m_tex)
			ImGui::TextDisabled("None");
		else
			ImGui::Text(m_textureName.c_str());
		ImGui::SameLine();
		if (ImGui::Button("..##AddTextureButton"))
		{

		}
		ImGui::PopItemWidth();
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
}