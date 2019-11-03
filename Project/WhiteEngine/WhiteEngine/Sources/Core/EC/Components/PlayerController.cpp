#include "PlayerController.hpp"
#include <math.h>

PlayerController::PlayerController() {

}

void PlayerController::OnAwake() {

}

void PlayerController::OnDisable() {

}

void PlayerController::OnStart() {
	Gun = m_gameObject->m_transform.GetChild(0);

	GunDistance = 50.0f;
}

void PlayerController::OnEnable() {
	
}

void PlayerController::OnUpdate(float dt) 
{
	mouseAim();
}

void PlayerController::OnFixedUpdate(float dt) {

}

void PlayerController::mouseAim() 
{
	float mouse_x, mouse_y, pos_x, pos_y;

	pos_x = m_gameObject->m_transform.GetPosition().x;
	pos_y = m_gameObject->m_transform.GetPosition().y;

	mouse_x = Input::GetMouseWorldPosition().x;
	mouse_y = Input::GetMouseWorldPosition().y;

	angle_deg = ((atan2(mouse_x - pos_x, mouse_y - pos_y) * 180 / PI) - 90) * -1.0f;

	angle_rad = angle_deg / 180 * PI;

	Gun->SetRotation(angle_deg);

	Gun->SetLocalPosition(glm::vec3(GunDistance * cos(angle_rad), GunDistance * sin(angle_rad), 0));

	/*if (angle_deg > 90) {
		Gun->SetScale(glm::vec3(Gun->GetScale().x, -1.0 * Gun->GetScale().y, Gun->GetScale().z));
	}
	else {
		Gun->SetScale(glm::vec3(Gun->GetScale().x, Gun->GetScale().y, Gun->GetScale().z));
	}*/


	//std::cout << angle_deg << std::endl;
}
