#include "Weapon.hpp"
#include "Input/Input.hpp"
#include "GameController.hpp"

BlackholeGun::BlackholeGun() {

	this->type = WEAPON_TYPE::WEAPON_GRENADELAUNCHER;

	/*weaponObj = new GameObject();
	weaponObj->AddComponent<MeshRenderer>();
	weaponObj->GetComponent<MeshRenderer>()->CreateMesh(3, 5);
	weaponObj->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/blackhole_spritesheet.png");

	weaponObj->SetActive(false);*/

}

void BlackholeGun::OnAwake() {

	weapon_damage = 1.0f;
	weapon_firerate = 0.3f;
	bullet_speed = 300.0f;

	bullet_Duration = 2.0f;
	bullet_Radius = 100.0f;
	bullet_ToCenterSpeed = 100.0f;

	m_gameObject->SetActive(false);
}

void BlackholeGun::Modify() {

}

void BlackholeGun::MultiplyWeaponAmplifier(float value) {
	bullet_Radius = bullet_Radius * value;
}

void BlackholeGun::GameTimeBehaviour(float dt) {
	BulletPool = GameController::GetInstance()->GetPool(POOL_TYPE::BULLET_BH);

	if (Input::GetMouseHold(Input::MouseKeyCode::MOUSE_LEFT) ||
		Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT))
	{
		weapon_delay_count += dt;
		if (weapon_delay_count > weapon_firerate)
		{
			GameObject* bullet = BulletPool->GetInactiveObject();

			if (bullet != nullptr) {
				bullet->SetActive(true);
				bullet->GetComponent<BlackholeGunBullet>()->SetDamage(weapon_damage);
				bullet->GetComponent<BlackholeGunBullet>()->SetDuration(bullet_Duration);
				bullet->GetComponent<BlackholeGunBullet>()->SetRadius(bullet_Radius);
				bullet->GetComponent<BlackholeGunBullet>()->SetToCenterSpeed(bullet_ToCenterSpeed);

				angle_deg = *angle;
				angle_rad = glm::radians(*angle);

				float posX = modifyObject->m_transform->GetPosition().x + (50 * cos(angle_rad));
				float posY = modifyObject->m_transform->GetPosition().y + (50 * sin(angle_rad));
				bullet->m_transform->SetPosition(glm::vec3(posX, posY, 0.0f));
				bullet->m_transform->SetRotation(angle_deg);

				float speedX = bullet_speed * cos(angle_rad);
				float speedY = bullet_speed * sin(angle_rad);

				bullet->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(speedX, speedY, 0.0f));

				weapon_delay_count = 0.0f;
			}
		}
	}
}