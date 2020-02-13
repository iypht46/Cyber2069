#include "FlyerBehaviour.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"

FlyerBehaviour::FlyerBehaviour() {
	hp = 10;
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
	af->FollowPlayer(dt);
	//if (Input::GetKeyHold(Input::KeyCode::KEY_M)) {
		ad->Dash(dt);
	//}

}

void FlyerBehaviour::OnFixedUpdate(float dt)
{
}
