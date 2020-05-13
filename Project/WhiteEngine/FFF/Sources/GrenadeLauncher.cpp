#include "Weapon.hpp"
#include "Input/Input.hpp"
#include "GameController.hpp"

GrenadeLauncher::GrenadeLauncher() {

	this->type = WEAPON_TYPE::WEAPON_GRENADELAUNCHER;

	/*weaponObj = new GameObject();
	weaponObj->AddComponent<MeshRenderer>();
	weaponObj->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
	weaponObj->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/grenadeL_spritesheet.png");
	weaponObj->AddComponent<SoundPlayer>();
	

	weaponObj->SetActive(false);*/

	
}

void GrenadeLauncher::OnAwake() {

	m_gameObject->GetComponent<SoundPlayer>()->CreateSoundPlayer();
	m_gameObject->GetComponent<SoundPlayer>()->SetSound(SoundPath("SFX_GrenadeLauncher_Shoot"));
	m_gameObject->GetComponent<SoundPlayer>()->SetLoop(false);

	weapon_damage = 1.0f;
	//weapon_firerate = 0.3f;
	bullet_speed = 300.0f;

	grenade_radius = 100.0f;

	m_gameObject->SetActive(false);

}

void GrenadeLauncher::Modify() {

}

void GrenadeLauncher::MultiplyWeaponAmplifier(float value) {
	grenade_radius = grenade_radius * value;
}

void GrenadeLauncher::GameTimeBehaviour(float dt) {
	BulletPool = GameController::GetInstance()->GetPool(POOL_TYPE::BULLET_GL);

	weapon_delay_count += dt;
	if (Input::GetMouseHold(Input::MouseKeyCode::MOUSE_LEFT) ||
		Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT))
	{
		if (weapon_delay_count >= (1.0f / weapon_firerate))
		{
			m_gameObject->GetComponent<SoundPlayer>()->PlaySound();

			GameObject* bullet = BulletPool->GetInactiveObject();

			if (bullet != nullptr) {
				bullet->SetActive(true);
				bullet->GetComponent<GrenadeLauncherBullet>()->SetDamage(weapon_damage);
				bullet->GetComponent<GrenadeLauncherBullet>()->SerRadius(grenade_radius);

				angle_deg = *angle;
				angle_rad = glm::radians(*angle);

				float posX = m_gameObject->m_transform->GetPosition().x + (50 * cos(angle_rad));
				float posY = m_gameObject->m_transform->GetPosition().y + (50 * sin(angle_rad));
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
