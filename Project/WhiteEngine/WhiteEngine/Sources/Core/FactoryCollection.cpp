#include "FactoryCollection.h"

#include "Core/Factory.h"

//essential component
#include "EC/GameObject.hpp"

#include "Ec/Components/Animator.hpp"
#include "Core/Particle/ParticleSystem.h"
#include "EC/Components/BehaviourScript.h"

#include "Logger.hpp"

namespace FactoryCollection {

	void FixedUpdateComponents(float dt) {
		//update animator
		for (Animator* anim : Factory<Animator>::getCollection()) {
			if (anim->GetGameObject() != nullptr && anim->GetGameObject()->Active()) {
				anim->animUpdate(dt);
			}
		}

		//update particle
		for (ParticleSystem* particlesystem : Factory<ParticleSystem>::getCollection()) {
			if (particlesystem->GetGameObject() != nullptr && particlesystem->GetGameObject()->Active()) {
				particlesystem->ConstantEmit(dt);
			}
			particlesystem->LifeTimeModification(dt);
		}

		//update behaviour script
		for (BehaviourScript* behaviour : Factory<BehaviourScript>::getCollection()) {
			if (behaviour->GetGameObject() != nullptr && behaviour->GetGameObject()->Active()) {
				behaviour->OnFixedUpdate(dt);
			}
		}
	}

	void UpdateComponents(float dt) {
		//update behaviour script
		for (BehaviourScript* behaviour : Factory<BehaviourScript>::getCollection()) {
			if (behaviour->GetGameObject() != nullptr && behaviour->GetGameObject()->Active()) {
				behaviour->OnUpdate(dt);
			}
		}
	}
}