#include "PopupWindow.hpp"
#include "imgui.h"

namespace Tools
{
	PopupWindow::PopupWindow()
	{
	}

	void PopupWindow::DisplayPopup(PopupData& popup)
	{
		if (!ImGui::IsPopupOpen(popup.m_title.c_str()))
			ImGui::OpenPopup(popup.m_title.c_str());

		if (ImGui::BeginPopupModal(popup.m_title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 size = ImGui::GetItemRectSize();
			float mid_pos = size.x / 2;
			//float indent_size = size.x / 12.0f;
			float wrap_width = 250.0f;//size.x - indent_size;
			//ImGui::Indent(indent_size);
			ImGui::PushTextWrapPos(wrap_width);
			ImGui::Text(popup.m_message.c_str(), wrap_width);
			ImGui::PopTextWrapPos();
			if (popup.m_isConfirmBox)
			{
				if (ImGui::Button("Yes", ImVec2(40, 0)))
				{
					//Call Function Pointer or Lambda?

					ImGui::CloseCurrentPopup();
					m_popupQueue.pop();
				}
				ImGui::SameLine();
				if (ImGui::Button("No", ImVec2(40, 0)))
				{
					//Call Function Pointer or Lambda?

					ImGui::CloseCurrentPopup();
					m_popupQueue.pop();
				}
			}
			else
			{
				
				//float indent_size = (size.x/2) - 40.0f;
				ImGui::Indent(mid_pos - (c_button_x / 2));
				if (ImGui::Button("Ok", ImVec2(c_button_x, 0)))
				{
					ImGui::CloseCurrentPopup();
					m_popupQueue.pop();
				}
			}

			ImGui::EndPopup();
		}
	}

	void PopupWindow::Push(PopupData& PopupData)
	{
		m_popupQueue.push(PopupData);
	}
	
	void PopupWindow::Update()
	{
		if (m_popupQueue.empty())
			return;

		//Show one pop up per frame
		auto popup = m_popupQueue.front();
		DisplayPopup(popup);
	}

	int PopupWindow::size() const
	{
		return m_popupQueue.size();
	}
}


