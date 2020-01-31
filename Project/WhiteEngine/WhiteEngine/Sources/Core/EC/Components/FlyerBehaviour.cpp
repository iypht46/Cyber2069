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
	AirFollowing flyer;
	flyer.FollowPlayer(dt);

}

void FlyerBehaviour::OnFixedUpdate(float dt)
{
}
