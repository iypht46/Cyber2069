#include "FactoryCollection.h"

#include "Factory.h"

//essential component
#include "EC/Components/Animator.hpp"
#include "EC/Components/MeshRenderer.hpp"
#include "EC/Components/Rigidbody.hpp"
#include "EC/Components/Transform.hpp"
#include "EC/Components/BehaviourScript.h"

void FactoryCollection::FixedUpdateComponents(float dt) {
	//update animator
	for (Animator* anim : Factory<Animator>::getCollection()) {
		anim->animUpdate();
	}

	//update behaviour script
	for (BehaviourScript* behaviour : Factory<BehaviourScript>::getCollection()) {
		behaviour->OnFixedUpdate(dt);
	}
}

void FactoryCollection::UpdateComponents(float dt) {
	//update behaviour script
	for (BehaviourScript* behaviour : Factory<BehaviourScript>::getCollection()) {
		behaviour->OnUpdate(dt);
	}
}