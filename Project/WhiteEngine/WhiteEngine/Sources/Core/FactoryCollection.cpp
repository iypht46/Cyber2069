#include "FactoryCollection.h"

#include "Factory.h"

//essential component
#include "EC/Components/Animator.hpp"
#include "EC/Components/MeshRenderer.hpp"
#include "EC/Components/Rigidbody.hpp"
#include "EC/Components/Transform.hpp"
#include "EC/Components/BehaviourScript.h"
#include "EC/Components/FlyerBehaviour.hpp"

#include "Core/Logger.hpp"

namespace FactoryCollection {

	void FixedUpdateComponents(float dt) {
		//update animator
		for (Animator* anim : Factory<Animator>::getCollection()) {
			anim->animUpdate();
		}

		//update behaviour script
		for (FlyerBehaviour* behaviour : Factory<FlyerBehaviour>::getCollection()) {
			behaviour->OnFixedUpdate(dt);
		}
	}

	void UpdateComponents(float dt) {
		//update behaviour script
		for (FlyerBehaviour* behaviour : Factory<FlyerBehaviour>::getCollection()) {
			behaviour->OnUpdate(dt);
		}

	}
}