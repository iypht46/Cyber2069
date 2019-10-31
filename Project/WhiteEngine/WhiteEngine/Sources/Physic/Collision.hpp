#pragma once

#include "Core/EC/Components/Collider.hpp"

namespace Physic
{

	//For Box to Box
	bool AABBtoAABB(BoxCollider, BoxCollider);

	typedef void(*CollisionFunc)(Collider*, Collider*);

	extern CollisionFunc Check[static_cast<uint16_t>(ColliderType::COLNUM)][static_cast<uint16_t>(ColliderType::COLNUM)];
}
