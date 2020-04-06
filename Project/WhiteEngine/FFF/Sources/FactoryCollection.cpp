#include "FactoryCollection.h"

#include "Core/Factory.h"

//essential component
#include "Core/EC/Components/Animator.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Transform.hpp"

#include "Core/EC/Components/BehaviourScript.h"
#include "PlayerController.hpp"
#include "EnemySpawner.hpp"

#include "Core/Logger.hpp"

namespace FactoryCollection {

	void FixedUpdateComponents(float dt) {
		//update animator
		for (Animator* anim : Factory<Animator>::getCollection()) {
			if (anim->GetGameObject()->Active()) {
				anim->animUpdate(dt);
			}
		}

		//update behaviour script
		for (BehaviourScript* behaviour : Factory<BehaviourScript>::getCollection()) {
			if (behaviour->GetGameObject()->Active()) {
				behaviour->OnFixedUpdate(dt);
			}
		}
	}

	void UpdateComponents(float dt) {
		//update behaviour script
		for (BehaviourScript* behaviour : Factory<BehaviourScript>::getCollection()) {
			if (behaviour->GetGameObject()->Active()) {
				behaviour->OnUpdate(dt);
			}
		}
	}
}