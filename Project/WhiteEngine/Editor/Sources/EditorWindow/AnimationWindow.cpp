#include "AnimationWindow.hpp"

namespace Tools
{
	void AnimationWindow::SetEditorEntity(EditorEntity* obj)
	{
		m_entity = obj;
	}

	void AnimationWindow::OnRender()
	{
		if (m_entity)
		{
			auto meshRenderer = m_entity->GetComponent("MeshRendererEC");

			if (meshRenderer)
			{
				meshRenderer->Render();
			}
		}

		ImGui::Separator();
		
		if (m_objectToRender)
			m_objectToRender->Render();
	}
}

