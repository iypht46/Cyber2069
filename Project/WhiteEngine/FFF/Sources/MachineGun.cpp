#include "Weapon.hpp"
#include "Input/Input.hpp"
#include "GameController.hpp"

MachineGun::MachineGun() 
{

	this->type = WEAPON_TYPE::WEAPON_MACHINEGUN;

	/*weaponObj = new GameObject();
	weaponObj->AddComponent<MeshRenderer>();
	weaponObj->GetComponent<MeshRenderer>()->CreateMesh(4, 1);
	weaponObj->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/machinegun_shoot.png");
	weaponObj->GetComponent<MeshRenderer>()->SetLayer(3);

	weaponObj->SetActive(false);*/
}

void MachineGun::OnAwake() {

	weapon_damage = 1.0f;
	//weapon_firerate = 0.3f;
	bullet_speed = 400.0f;
	
	m_gameObject->SetActive(false);
}

void MachineGun::Modify() 
{
}

void MachineGun::MultiplyWeaponAmplifier(float value) 
{
	std::queue<GameObject*> PoolObj = GameController::GetInstance()->GetPool(POOL_TYPE::BULLET_MG)->GetAllGameObject();

	if (PoolObj.size() > 0) {
		GameObject* back = PoolObj.back();
		GameObject* obj = nullptr;

		do {
			obj = PoolObj.front();
			PoolObj.pop();
			PoolObj.push(obj);

			obj->m_transform->SetScale(value * obj->m_transform->GetScale());
			BoxCollider* box = dynamic_cast<BoxCollider*>(obj->GetComponent<Rigidbody>()->GetCollider());

			box->ReSize(box->GetHw() * value, box->GetHh() * value);

		} while (obj != back);
	}

	/*for (GameObject* obj : PoolObj) 
	{
		obj->m_transform->SetScale(value * obj->m_transform->GetScale());
		BoxCollider* box = dynamic_cast<BoxCollider*>(obj->GetComponent<Rigidbody>()->GetCollider());

		box->ReSize(box->GetHw() * value, box->GetHh() * value);
	}*/

}


void MachineGun::GameTimeBehaviour(float dt) {

	BulletPool = GameController::GetInstance()->GetPool(POOL_TYPE::BULLET_MG);

	if (Input::GetMouseHold(Input::MouseKeyCode::MOUSE_LEFT) ||
		Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT))
	{
		weapon_delay_count += dt;
		//ENGINE_INFO("MG: {}/{}", weapon_delay_count, (1.0f / weapon_firerate));
		if (weapon_delay_count >= (1.0f / weapon_firerate))
		{
			GameObject* bullet = BulletPool->GetInactiveObject();

			if (bullet != nullptr) {
				bullet->SetActive(true);
				bullet->GetComponent<MachineGunBullet>()->SetDamage(weapon_damage);

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