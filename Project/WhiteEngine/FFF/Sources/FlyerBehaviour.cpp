#include "FlyerBehaviour.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"

FlyerBehaviour::FlyerBehaviour() {

}

void FlyerBehaviour::OnAwake()
{
}

void FlyerBehaviour::OnEnable()
{
}

void FlyerBehaviour::OnStart()
{
}

void FlyerBehaviour::OnDisable()
{
}

void FlyerBehaviour::OnUpdate(float dt)
{
	//af->FollowPlayer(dt);
	ad->Dash(dt);
}

void FlyerBehaviour::OnFixedUpdate(float dt)
{
}
