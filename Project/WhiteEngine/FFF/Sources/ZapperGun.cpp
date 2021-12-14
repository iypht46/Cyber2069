#include "Weapon.hpp"
#include "Input/Input.hpp"
#include "GameController.hpp"

ZapperGun::ZapperGun() {

	this->type = WEAPON_TYPE::WEAPON_ZAPPER;
}

void ZapperGun::OnAwake(){

	m_gameObject->GetComponent<SoundPlayer>()->SetSound(SoundPath("SFX_Zapper_Shoot"));
	m_gameObject->GetComponent<SoundPlayer>()->SetLoop(false);
	m_gameObject->SetActive(false);

	/*weapon_damage = 1.0f;
	weapon_firerate = 0.1f;
	bullet_speed = 300.0f;

	chainNumber = 3;
	zapDistance = 300.0f;
	zapDuration = 0.5f;*/

	m_gameObject->SetActive(false);

}

void ZapperGun::Modify() {

}

void ZapperGun::MultiplyWeaponAmplifier(float value) {
	chainNumber = chainNumber * value;
}

void ZapperGun::GameTimeBehaviour(float dt) {
	BulletPool = GameController::GetInstance()->GetPool(POOL_TYPE::BULLET_ZP);

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
				bullet->GetComponent<ZapperGunBullet>()->SetDamage(weapon_damage);
				bullet->GetComponent<ZapperGunBullet>()->SetZapDistance(zapDistance);
				bullet->GetComponent<ZapperGunBullet>()->SetZapDuration(zapDuration);
				bullet->GetComponent<ZapperGunBullet>()->SetZapRate(zapRate);
				bullet->GetComponent<ZapperGunBullet>()->SetChainNumber(chainNumber);

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
