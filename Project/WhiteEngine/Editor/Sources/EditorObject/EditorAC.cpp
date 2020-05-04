#include "EditorAC.hpp"
#include "Serialization/Serialization.h"
#include <imgui.h>
#include "misc/cpp/imgui_stdlib.h"

namespace Tools
{
	void RenderAnimation(Animation* anim)
	{
		if (!anim)
			return;

		//if (ImGui::TreeNodeEx(anim.))
		ImGui::Text("Animation Name: "); ImGui::SameLine();
		ImGui::InputText("##AnimName", &anim->stateName, ImGuiInputTextFlags_CharsNoBlank);

		ImGui::Text("Speed Multiplier: "); ImGui::SameLine(); ImGui::InputInt("##SpeedMul", &anim->speedMultiplier);
		ImGui::Text("Start Pos:"); ImGui::SameLine(); ImGui::InputFloat2("##StartPos", &anim->m_StartPosition[0], 2);
		ImGui::Text("End Pos:"); ImGui::SameLine(); ImGui::InputFloat2("##EndPos", &anim->m_EndPosition[0], 2);

	}

	void RenderAnimationState(AnimationState* state)
	{
		if (!state)
			return;

		ImGui::Text("State Name:"); ImGui::SameLine(); ImGui::InputText("##AsName", &state->stateName);
		ImGui::Text("Loop"); ImGui::SameLine();
		ImGui::Checkbox("##ASLoop", &state->loop);

		RenderAnimation(state->animation.get());
		/*ImGui::TreeNode()
		RenderAnimation(*state.animation);*/
	}

	

	EditorAC::EditorAC()
	{
		m_animationController = std::make_shared<AnimationController>();
	}

	void EditorAC::OnRender()
	{
		ImGui::PushItemWidth(-1);

		ImGui::Text("Sheet Size: "); ImGui::SameLine();
		if (ImGui::DragFloat2("##SheetSize", &m_frameSize.x, 1.0f, 1.0f, 50.0f, "%.1f"))
		{
			m_animationController->setSheetSize(m_frameSize);
			shouldUpdateMesh = true;
		}

		ImGui::PopItemWidth();

		if (m_animationController->m_states.size() > 0)
		{
			int i = 0;
			currentState = -1;
			for (auto it = m_animationController->m_states.begin(); it != m_animationController->m_states.end(); ++it)
			{
				std::string name = "State " + std::to_string((i + 1));
				bool open;
				ImGui::PushID(name.c_str());
				open = ImGui::TreeNodeEx(name.c_str());
				ImGui::PopID();

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Animation"))
					{
						if (payload->DataSize != sizeof(int))
							break;
						int payload_n = *(const int*)payload->Data;
						m_animationController->SwapState(payload_n, i);
					}
					ImGui::EndDragDropTarget();
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("Animation", &i, sizeof(int));
					ImGui::Text(name.c_str());
					ImGui::EndDragDropSource();
				}
				
				if (open)
				{
					if (ImGui::IsItemClicked())
						currentState = i;

					RenderAnimationState(it->get());
					ImGui::TreePop();
				}
				
				i++;
			}
		}

		if (ImGui::Button("Add State"))
		{
			std::shared_ptr<Animation> anim = std::make_shared<Animation>();
			m_animationController->AddState(anim);
		}
		
	}

	bool EditorAC::Load(Container::wString path)
	{
		if (m_animationController)
		{
			m_animationController.reset();
			m_animationController = std::make_unique<AnimationController>();
		}

		Serialization::LoadObject(*m_animationController, path);


		return true;
	}

	bool EditorAC::Save(Container::wString path)
	{
		if (!m_animationController)
			return false;

		Serialization::SaveObject(*m_animationController, path);
		return true;
	}

	AC_HANDLE EditorAC::GetAnimController()
	{
		return m_animationController;
	}
}

