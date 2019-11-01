#include "FactoryCollection.h"

#include "Factory.h"

//essential component
#include "EC/Components/Animator.hpp"
#include "EC/Components/MeshRenderer.hpp"
#include "EC/Components/Rigidbody.hpp"
#include "EC/Components/Transform.hpp"
#include "EC/Components/BehaviourScript.h"

void FactoryCollection::UpdateComponents(float dt) {
	//update animator
	for (Animator* anim : Factory<Animator>::getCollection()) {
		anim->animUpdate();
	}

	//update renderer
	for (MeshRenderer* renderer : Factory<MeshRenderer>::getCollection()) {
		renderer->OnUpdate(dt);
	}

	//update behaviour
	for (BehaviourScript* script : Factory<BehaviourScript>::getCollection()) {
		script->OnUpdate(dt);
	}
}