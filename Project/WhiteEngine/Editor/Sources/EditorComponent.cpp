#include "imgui.h"
#include "EditorComponent.hpp"
#include "Core/Logger.hpp"


namespace Tools
{
	void EditorComponent::Render()
	{
		if (!ImGui::CollapsingHeader(m_componentName.c_str(), &m_open))
		{
			//TODO: Remove Component
		}
		else
		{
			OnRender();
		}
		
	}

	TransformEC::TransformEC(Transform* component) : EditorComponent("Transform", component) 
	{ 
		this->Init(); 
	}

	void TransformEC::Init()
	{
		Transform* transform = dynamic_cast<Transform*>(m_component);

		if (transform)
		{
			m_position = &transform->m_position;
			m_scale = &transform->m_scale;
			m_rotation = &transform->m_rotation;
			return;
		}

		ENGINE_ERROR("ERROR::Component is not transfrom");
	}

	void TransformEC::OnRender()
	{
		ImGui::PushItemWidth(-1 * (ImGui::GetWindowWidth()/4));
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

	MeshRendererEC::MeshRendererEC(MeshRenderer* component) : EditorComponent("MeshRenderer", component) 
	{
		this->Init(); 
	}

	void MeshRendererEC::Init()
	{
		MeshRenderer* transform = dynamic_cast<MeshRenderer*>(m_component);

	}

	void MeshRendererEC::OnRender()
	{
		ImGui::PushItemWidth(-1);
		ImGui::AlignTextToFramePadding();
		if (!m_tex)
		{
			ImGui::TextDisabled("None"); 
		}
		else
		{
			ImGui::Text(m_textureName.c_str());
		}
		ImGui::SameLine();
		ImGui::Button("..##AddTextureButton");
		ImGui::PopItemWidth();
	}

	bool MeshRendererEC::AddTexture(const char * path)
	{
		return false;
	}

	
}


