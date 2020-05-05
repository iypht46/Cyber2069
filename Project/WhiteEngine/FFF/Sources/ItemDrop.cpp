#include "ItemDrop.hpp"

void ItemDrop::OnStart()
{
	itemtype = pair<int, int>(-1, -1);

	eqManager = GameController::GetInstance()->GetGameObject()->GetComponent<EquipmentManager>();
	player = GameController::GetInstance()->GetPlayer()->GetComponent<PlayerController>();

	m_gameObject->SetActive(true);
}

void ItemDrop::OnEnable() 
{
	itemtype = eqManager->GetRandomType();

	if (itemtype.first != -1 && itemtype.second != -1) 
	{
		switch (itemtype.first)
		{
		case ITEM_TYPE::WEAPON:

			break;
		case ITEM_TYPE::ARTIFACT:

			break;
		default:
			break;
		}
	}
}

void ItemDrop::OnTriggerEnter(const Physic::Collision col)
{
	if (itemtype.first != -1 && itemtype.second != -1)
	{
		GAME_INFO("UNLOCK {}, {}", itemtype.first, itemtype.second);
		eqManager->Unlock(itemtype.first, itemtype.second);

		m_gameObject->SetActive(false);
	}
}