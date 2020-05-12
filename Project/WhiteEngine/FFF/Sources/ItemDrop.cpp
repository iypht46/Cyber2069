#include "ItemDrop.hpp"

void ItemDrop::OnAwake() {
	rb = GetGameObject()->GetComponent<Rigidbody>();
	cam = Graphic::getCamera();
}

void ItemDrop::OnStart()
{
	itemtype = pair<int, int>(-1, -1);

	eqManager = GameController::GetInstance()->GetGameObject()->GetComponent<EquipmentManager>();
	player = GameController::GetInstance()->GetPlayer()->GetComponent<PlayerController>();

	defaultTex = m_gameObject->GetComponent<MeshRenderer>()->GetTextureObj();
}

void ItemDrop::OnEnable() 
{
	itemtype = eqManager->GetRandomType();

	if (itemtype.first != -1 && itemtype.second != -1) 
	{
		switch (itemtype.first)
		{
		case ITEM_TYPE::WEAPON:
			if (eqManager->weaponItemTex[itemtype.second].m_textureID != -1) 
			{
				m_gameObject->GetComponent<MeshRenderer>()->SetTexture(eqManager->weaponItemTex[itemtype.second]);
			}
			else {
				m_gameObject->GetComponent<MeshRenderer>()->SetTexture(defaultTex);
			}
			break;
		case ITEM_TYPE::ARTIFACT:
			if (eqManager->artifactItemTex[itemtype.second].m_textureID != -1)
			{
				m_gameObject->GetComponent<MeshRenderer>()->SetTexture(eqManager->artifactItemTex[itemtype.second]);
			}
			else {
				m_gameObject->GetComponent<MeshRenderer>()->SetTexture(defaultTex);
			}
			break;
		default:
			break;
		}
	}
}

void ItemDrop::OnUpdate(float dt) {
	int winWidth;
	int winHeight;

	glm::vec3 camPos = cam->GetCampos();

	winWidth = Graphic::Window::GetWidth() * cam->GetZoom();
	winHeight = Graphic::Window::GetHeight() * cam->GetZoom();

	if (GetGameObject()->m_transform->GetPosition().y < (camPos.y - (winHeight / 2)))
	{
		rb->SetVelocity(glm::vec3(0));
		GetGameObject()->SetActive(false);
	}
}

void ItemDrop::OnTriggerEnter(const Physic::Collision col)
{
	if (itemtype.first != -1 && itemtype.second != -1)
	{
		GAME_INFO("UNLOCK {}, {}", itemtype.first, itemtype.second);
		eqManager->Unlock(itemtype.first, itemtype.second);
	}

	m_gameObject->SetActive(false);
}