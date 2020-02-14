#include "FactoryCollection.h"

#include "Core/Factory.h"

//essential component
#include "Core/EC/Components/Animator.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Transform.hpp"

#include "Core/EC/Components/BehaviourScript.h"
#include "FlyerBehaviour.hpp"
#include "PlayerController.hpp"
#include "MachineGunBullet.hpp"
#include "EnemySpawner.hpp"
#include "HPsystem.hpp"
#include "GameController.hpp"

#include "Core/Logger.hpp"

namespace FactoryCollection {

	void FixedUpdateComponents(float dt) {
		//update animator
		for (Animator* anim : Factory<Animator>::getCollection()) {
			anim->animUpdate(dt);
		}

		//update behaviour script
		for (FlyerBehaviour* behaviour : Factory<FlyerBehaviour>::getCollection()) {
			behaviour->OnFixedUpdate(dt);
		}

		for (HPsystem* hp : Factory<HPsystem>::getCollection()) {
			hp->OnFixedUpdate(dt);
		}

		for (GameController* gc : Factory<GameController>::getCollection()) {
			gc->OnFixedUpdate(dt);
		}
	}

	void UpdateComponents(float dt) {
		//update behaviour script
		for (EnemySpawner* behaviour : Factory<EnemySpawner>::getCollection()) {
			behaviour->OnUpdate(dt);
		}

		for (FlyerBehaviour* behaviour : Factory<FlyerBehaviour>::getCollection()) {
			behaviour->OnUpdate(dt);
		}

		for (MachineGunBullet* behaviour : Factory<MachineGunBullet>::getCollection()) {
			if (behaviour->GetGameObject()->Active()) 
			{
				behaviour->OnUpdate(dt);
			}
		}

		for (HPsystem* hp : Factory<HPsystem>::getCollection()) {
			hp->OnUpdate(dt);
		}

		for (GameController* gc : Factory<GameController>::getCollection()) {
			gc->OnUpdate(dt);
		}

		Factory<PlayerController>::getCollection().at(0)->OnUpdate(dt);

	}
}