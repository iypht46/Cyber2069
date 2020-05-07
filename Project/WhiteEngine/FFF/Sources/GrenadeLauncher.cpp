#include "Weapon.hpp"
#include "Input/Input.hpp"
#include "GameController.hpp"

GrenadeLauncher::GrenadeLauncher() {
	weaponObj = new GameObject();
	weaponObj->AddComponent<MeshRenderer>();
	weaponObj->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
	weaponObj->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/grenadeL_spritesheet.png");
	weaponObj->AddComponent<SoundPlayer>();
	weaponObj->GetComponent<SoundPlayer>()->CreateSoundPlayer();
	weaponObj->GetComponent<SoundPlayer>()->SetSound(SoundPath("SFX_GrenadeLauncher_Shoot"));
	weaponObj->GetComponent<SoundPlayer>()->SetLoop(false);

	weaponObj->SetActive(false);

	weapon_damage = 1.0f;
	weapon_firerate = 0.3f;
	bullet_speed = 500.0f;

	grenade_radius = 200.0f;

	weapon_scale.x = 50.0f;
	weapon_scale.y = 50.0f;

	SoundTimer = 0.30f;
	SoundCounter = SoundTimer;
}

void GrenadeLauncher::Modify(GameObject* obj) {

}

void GrenadeLauncher::GameTimeBehaviour(float dt) {
	BulletPool = GameController::GetInstance()->GetPool(POOL_TYPE::BULLET_GL);

	if (Input::GetMouseHold(Input::MouseKeyCode::MOUSE_LEFT) ||
		Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT))
	{
		SoundCounter -= dt;
		if (SoundCounter <= 0) {
			weaponObj->GetComponent<SoundPlayer>()->PlaySound();
			SoundCounter = SoundTimer;
		}

		weapon_delay_count += dt;
		if (weapon_delay_count > weapon_firerate)
		{
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

void GrenadeLauncher::onDisable() 
{
	weaponObj->SetActive(false);
}
