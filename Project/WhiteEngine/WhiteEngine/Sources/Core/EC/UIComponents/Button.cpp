#include "Button.hpp"
#include "../../FFF/Sources/LoadoutUI.hpp"
#include "../../FFF/Sources/GameController.hpp"
#include "../../FFF/Sources/Scripts/GameControl/UIController.h"

#include <iostream>
#include "Graphic/GLRenderer.h"

Button::Button() {
	
}

void Button::OnAwake() 
{
	mesh = m_gameObject->GetComponent<MeshRenderer>();
	text = m_gameObject->GetComponent<TextRenderer>();


	if (mesh != nullptr) 
	{
		//DefaultTexture = mesh->GetTexture();
		DefaultTexObj = mesh->GetTextureObj();

		if (hoverModifier.ReTexturePath != "")
		{
			//OnHoverTex = GLRenderer::GetInstance()->LoadTexture(hoverModifier.ReTexturePath);
			OnHoverTexObj = GLRenderer::GetInstance()->LoadTextureNew(hoverModifier.ReTexturePath);
		}
	}

	if (text != nullptr) 
	{
		DefaultText = text->GetText();
	}

}

void Button::OnUpdate(float dt) {

	isOnHover = false;

	if (mesh != nullptr) {
		if (mesh->IsUI()) 
		{
			m_pos = Input::GetMouseScreenPosition();
		}
		else {
			m_pos = Input::GetMouseWorldPosition();
		}
	}
	else {
		m_pos = Input::GetMouseWorldPosition();
	}

	glm::vec2 diff = m_pos - (glm::vec2)m_gameObject->m_transform->GetPosition();
	if (glm::abs(diff.x) <= glm::abs((m_gameObject->m_transform->GetScale().x * 0.5f)) &&
		glm::abs(diff.y) <= glm::abs((m_gameObject->m_transform->GetScale().y * 0.5f))) 
	{
		isOnHover = true;
		if (Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT)) {
			OnClick();
		}
	}

	ModifyOnHover();
}

void Button::OnClick() 
{
	ENGINE_INFO("Click");

	LoadoutSelectButton* lsb = m_gameObject->GetComponent<LoadoutSelectButton>();

	switch (buttonType)
	{
	case BUTTON_TYPE::UICONTROL:
		UIController::GetInstance()->ToggleUI(onClickType);
		break;
	case BUTTON_TYPE::STATECONTROL:
		GameController::GetInstance()->SetGameState(onClickType);
		break;
	case BUTTON_TYPE::LOADOUTSELECT:

		if (lsb != nullptr) 
		{
			switch (lsb->OnclickType)
			{
			case LOADOUTONCLICK_TYPE::ADD:
				lsb->AddEquipment();
				break;
			case LOADOUTONCLICK_TYPE::REMOVE:
				if (lsb->type == ITEM_TYPE::WEAPON) 
				{
					lsb->RemoveWeapon();
				}
				else {
					lsb->RemoveArtifact(lsb->currIndex);
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

Button::~Button() {

}

void Button::ModifyOnHover() {
	
	if (isOnHover) {

		if (mesh != nullptr) {
			if (OnHoverTex != -1)
			{
				//mesh->SetTexture(OnHoverTex);
				mesh->SetTexture(OnHoverTexObj);
			}

			mesh->SetReplaceColor(hoverModifier.ReColor);
			//mesh->SetReplaceColor(glm::vec3(173.0f / 255.0f, 173.0f / 255.0f, 173.0f / 255.0f));
		}

		if ((text != nullptr) && hoverModifier.ReText != "")
		{
			text->SetText(hoverModifier.ReText);
		}
	}
	else 
	{
		if (mesh != nullptr) {
			if (DefaultTexture != -1 && OnHoverTex != -1)
			{
				//mesh->SetTexture(DefaultTexture);
				mesh->SetTexture(DefaultTexObj);
			}

			mesh->RemoveReplaceColor();
		}

		if ((text != nullptr))
		{
			text->SetText(DefaultText);
		}
	}
}