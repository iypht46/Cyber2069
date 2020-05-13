#include "EnemyBehaviours.h"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "HPsystem.hpp"

void Cocoon::OnUpdate(float dt) 
{
	int spriteIndex = 20 - ((hpSystem->GetHP() * 20) / hpSystem->GetMaxHP());

	float offSetY = (spriteIndex / 5.0f);

	offSetY = floor(offSetY);

	float offSetX = spriteIndex % 5;

	if (spriteIndex == 0) {
		offSetX = 0;
		offSetY = 0;
	}

	m_gameObject->GetComponent<MeshRenderer>()->SetMeshUV(glm::vec2((int)offSetX, (int)offSetY));

	GAME_INFO("Sprite index is {}", spriteIndex);
	GAME_INFO("Sprite offset is {} , {}", offSetX, offSetY);

	auto animation = GetGameObject()->GetComponent<Animator>();
}
