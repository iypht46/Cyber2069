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
		for (std::pair<int, Animator*> anim : Factory<Component, Animator>::getCollection()) {
			if (anim.second->GetGameObject() != nullptr && anim.second->GetGameObject()->Active()) {
				anim.second->animUpdate(dt);
			}
		}

		//update particle
		for (std::pair<int, ParticleSystem*> particlesystem : Factory<Component, ParticleSystem>::getCollection()) {
			if (particlesystem.second->GetGameObject() != nullptr && particlesystem.second->GetGameObject()->Active()) {
				particlesystem.second->ConstantEmit(dt);
			}
			particlesystem.second->LifeTimeModification(dt);
		}

		//update behaviour script
		for (std::pair<int, BehaviourScript*> behaviour : Factory<Component, BehaviourScript>::getCollection()) {
			if (behaviour.second->GetGameObject() != nullptr && behaviour.second->GetGameObject()->Active()) {
				behaviour.second->OnFixedUpdate(dt);
			}
		}
	}

	void UpdateComponents(float dt) {
		//update behaviour script
		for (std::pair<int, BehaviourScript*> behaviour : Factory<Component, BehaviourScript>::getCollection()) {
			if (behaviour.second->GetGameObject() != nullptr && behaviour.second->GetGameObject()->Active()) {
				behaviour.second->OnUpdate(dt);
			}
		}
	}
}