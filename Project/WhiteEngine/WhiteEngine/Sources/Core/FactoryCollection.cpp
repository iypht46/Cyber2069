#include "FactoryCollection.h"

#include "Factory.h"

//essential component
#include "Core/Animator/Animator.hpp"
#include "EC/Components/MeshRenderer.hpp"
#include "EC/Components/Rigidbody.hpp"
#include "EC/Components/Transform.hpp"

void FactoryCollection::UpdateComponents(float dt) {
	//update animator
	for (Animator* anim : Factory<Animator>::getCollection()) {
		anim->animUpdate();
	}

	//update renderer
	for (MeshRenderer* renderer : Factory<MeshRenderer>::getCollection()) {
		renderer->OnUpdate(dt);
	}
}