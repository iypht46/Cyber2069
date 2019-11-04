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
	inverseGun = false;

	GunDistance = 0.5f;
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

	if (m_gameObject->m_transform.GetScale().x > 0) {
		Gun->SetRotation(angle_deg);
		
		if (angle_deg > 90) {
			if (!inverseGun) {
				Gun->SetScale(glm::vec3(Gun->GetScale().x, -1.0f * Gun->GetScale().y, Gun->GetScale().z));
				inverseGun = true;
			}
		}
		else {
			if (inverseGun) {

				Gun->SetScale(glm::vec3(Gun->GetScale().x, -1.0f * Gun->GetScale().y, Gun->GetScale().z));
				inverseGun = false;
			}
		}

		Gun->SetLocalPosition(glm::vec3(GunDistance * cos(angle_rad), GunDistance * sin(angle_rad), 1));
	}
	else {
		Gun->SetRotation(-1.0f * (angle_deg + 180));
		
		if (angle_deg < 90) {
			if (!inverseGun) {
				Gun->SetScale(glm::vec3(Gun->GetScale().x, -1.0f * Gun->GetScale().y, Gun->GetScale().z));
				inverseGun = true;
			}
		}
		else {
			if (inverseGun) {

				Gun->SetScale(glm::vec3(Gun->GetScale().x, -1.0f * Gun->GetScale().y, Gun->GetScale().z));
				inverseGun = false;
			}
		}

		Gun->SetLocalPosition(glm::vec3( -1.0f * GunDistance * cos(angle_rad), GunDistance * sin(angle_rad), 1));
	}


	std::cout << angle_deg << std::endl;
}
