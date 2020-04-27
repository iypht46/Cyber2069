#include "EditorAC.hpp"
#include "Serialization/Serialization.h"
#include <imgui.h>
#include "misc/cpp/imgui_stdlib.h"

namespace Tools
{
	void RenderAnimationState(AnimationState& state)
	{
		ImGui::Text("State Name:"); ImGui::SameLine();
		ImGui::InputText("##ASName", &state.stateName);
		ImGui::Text("Loop"); ImGui::SameLine();
		ImGui::Checkbox("##ASLoop", &state.loop);

		if (!state.animation)
		{
			//Add animation button

		}
		/*ImGui::TreeNode()
		RenderAnimation(*state.animation);*/
	}

	void RenderAnimation(Animation& anim)
	{

	}

	EditorAC::EditorAC()
	{
		m_animationController = std::make_unique<AnimationController>();
	}

	void EditorAC::OnRender()
	{
		if (ImGui::DragFloat2("##SheetSize", &m_frameSize.x, 1.0f, 1.0f, 50.0f, "%.1f"))
		{
			m_animationController->setSheetSize(m_frameSize);
		}

		if (ImGui::CollapsingHeader("Animation States"))
		{
			if (m_animationController->m_states.size() > 0)
			{
				for (auto it = m_animationController->m_states.begin(); it != m_animationController->m_states.end(); ++it)
				{

				}
			}
		}
		
	}

	bool EditorAC::Load(Path path)
	{
		if (m_animationController)
		{
			m_animationController.reset();
			m_animationController = std::make_unique<AnimationController>();
		}
			
		Serialization::LoadObject(*m_animationController, path.generic_string());
		
		
		return true;
	}

	bool EditorAC::Save(Path path)
	{
		if (!m_animationController)
			return false;

		Serialization::SaveObject(*m_animationController, path.generic_string());
		return true;
	}
}

