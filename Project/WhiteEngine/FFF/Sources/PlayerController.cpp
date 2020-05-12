#include "PlayerController.hpp"
#include "Input/Input.hpp"
#include "Graphic/Camera.hpp"
#include "Graphic/GLRenderer.h"
#include "Core/Logger.hpp"
#include <math.h>

PlayerController::PlayerController() {

}

void PlayerController::OnAwake() {
	rb = m_gameObject->GetComponent<Rigidbody>();
	hpSystem = m_gameObject->GetComponent<HPsystem>();

	stamina = max_stamina;
	hpSystem->ResetHP();

	direction.x = 1;
	direction.y = 1;


	/*AddEquipment(new MachineGun());
	AddEquipment(new LaserGun());
	AddEquipment(new GrenadeLauncher());
	AddEquipment(new ZapperGun());
	AddEquipment(new BlackholeGun());*/

	//assignWeapon(Weapons[0]);
}

void PlayerController::OnCollisionEnter(const Physic::Collision col)
{
	//GAME_INFO("Collider Enter");
}

void PlayerController::OnCollisionStay(const Physic::Collision col)
{
	//GAME_INFO("Collider Stay");
}

void PlayerController::OnCollisionExit(const Physic::Collision col)
{
	//GAME_INFO("Collider Exit");
}

void PlayerController::OnTriggerEnter(const Physic::Collision col)
{
	GameObject* obj = col.m_otherCollider->GetGameObject();
	Enemy* enem = obj->GetComponent<Enemy>();
	
	if (enem != nullptr) {
		hpSystem->TakeDamage(enem->GetCollideDamage());
	}
}

void PlayerController::OnTriggerStay(const Physic::Collision col)
{
	//GAME_INFO("Collider Stay");
}

void PlayerController::OnTriggerExit(const Physic::Collision col)
{
	//GAME_INFO("Collider Exit");
}

void PlayerController::OnUpdate(float dt)
{
	Graphic::getCamera()->SetPos(glm::vec3(GetGameObject()->m_transform->GetPosition().x, GetGameObject()->m_transform->GetPosition().y, GetGameObject()->m_transform->GetPosition().z));

	cameraZoom(dt);

	if ((rb->GetVelocity().y < -5.0f) && !falling)
	{
		falling = true;
	}

	if (falling)
	{
		if (checkGround())
		{
			jumping = false;
			falling = false;
			onGround = true;
		}
	}

	if (onGround && (stamina < max_stamina)) 
	{
		stamina += staminaRegenRate;
	}

	DebugInput();
	move();

	if (Dash) {
		dash(dt);
	}

	if (weapon != nullptr) {

		mouseAim();
	}

	for (Equipment* e : Equipments)
	{
		if (Weapon* w = dynamic_cast<Weapon*>(e)) 
		{
			if (w->GetWeapon()->Active()) 
			{
				w->GameTimeBehaviour(dt);
			}
		}
		else {
			e->GameTimeBehaviour(dt);
		}
	}

	if (m_gameObject->m_transform->GetPosition().y < yLimit) 
	{
		hpSystem->Dead();
	}
}

void PlayerController::OnFixedUpdate(float dt)
{

}

void PlayerController::updateDirection() {

}

void PlayerController::DebugInput() {
	
	if (Input::GetKeyDown(Input::KeyCode::KEY_R))
	{
		m_gameObject->m_transform->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
		hpSystem->ResetHP();
		m_gameObject->SetActive(true);
	}

	if (Input::GetKeyDown(Input::KeyCode::KEY_N))
	{
		if (GLRenderer::GetInstance()->drawDebug) {

			GLRenderer::GetInstance()->drawDebug = false;
		}
		else {
			GLRenderer::GetInstance()->drawDebug = true;
		}
	}

	if (Input::GetKeyDown(Input::KeyCode::KEY_M))
	{
		if (!hpSystem->isInvicible()) {

			hpSystem->SetInvincible(true);
		}
		else {
			hpSystem->SetInvincible(false);
		}
	}

}

void PlayerController::move()
{
	glm::vec3 velocity(0, 0, 0);
	//direction = glm::vec2(0);

	if (stamina < 0)
	{
		stamina = 0;
	}

	if (Input::GetKeyHold(Input::KeyCode::KEY_W))
	{
		direction.y = 1.0f;
		velocity.y = move_speed * direction.y;
		//rb->SetVelocity(glm::vec3(rb->GetVelocity().x, move_speed * direction.y, rb->GetVelocity().z));
	}
	else if (Input::GetKeyHold(Input::KeyCode::KEY_S))
	{
		direction.y = -1.0f;
		velocity.y = move_speed * direction.y;
		//rb->SetVelocity(glm::vec3(rb->GetVelocity().x, move_speed * direction.y, rb->GetVelocity().z));
	}

	if (Input::GetKeyHold(Input::KeyCode::KEY_A))
	{
		direction.x = -1.0f;
		velocity.x = move_speed * direction.x;
		//rb->SetVelocity(glm::vec3(move_speed * direction.x, rb->GetVelocity().y, rb->GetVelocity().z));


		if (facingRight) {
			flip();
		}

	}
	else if (Input::GetKeyHold(Input::KeyCode::KEY_D))
	{
		direction.x = 1.0f;
		velocity.x = move_speed * direction.x;
		//rb->SetVelocity(glm::vec3(move_speed * direction.x, rb->GetVelocity().y, rb->GetVelocity().z));

		if (!facingRight) {
			flip();
		}
	}

	if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE) && (stamina > 0))
	{
		rb->SetVelocity(glm::vec3(0, jump_speed, 0));
		stamina -= jumpStamina;
		jumping = true;
		running = false;
		falling = false;
		onGround = false;

		GetGameObject()->GetComponent<Animator>()->setCurrentState(3);
	}

	if ((!Input::GetKeyHold(Input::KeyCode::KEY_A) && !Input::GetKeyHold(Input::KeyCode::KEY_D)) && !jumping && !falling)
	{
		if (!Dash) {
			running = false;
			GetGameObject()->GetComponent<Animator>()->setCurrentState(0);
		}
	}
	else if ((Input::GetKeyHold(Input::KeyCode::KEY_A) || Input::GetKeyHold(Input::KeyCode::KEY_D)) && !jumping && !falling) {

		if (!running && !Dash) {
			running = true;
			GetGameObject()->GetComponent<Animator>()->setCurrentState(1);
		}
	}

	if (Input::GetKeyDown(Input::KeyCode::KEY_LEFT_SHIFT) && !Dash && !checkGround() && (stamina > 0))
	{
		stamina -= dashStamina;
		dashRemainingTime = dashTime;
		Dash = true;
		setDashAnim = false;

		delay = 0.1f;
	}

	if (!Dash)
	{
		rb->SetVelocity(glm::vec3(rb->GetVelocity().x + velocity.x, rb->GetVelocity().y, rb->GetVelocity().z));
		if (abs(rb->GetVelocity().x) > max_move_speed) {
			rb->SetVelocity(glm::vec3(rb->GetVelocity().x / abs(rb->GetVelocity().x) * max_move_speed, rb->GetVelocity().y, rb->GetVelocity().z));
		}
		//rb->SetVelocity(glm::vec3(velocity.x, velocity.y, rb->GetVelocity().z));
	}

}

void PlayerController::dash(float dt)
{
	if (dashRemainingTime <= 0)
	{
		hpSystem->SetInvincible(false);
		running = false;
		GetGameObject()->GetComponent<Animator>()->setCurrentState(4);
		Dash = false;
	}
	else
	{
		hpSystem->SetInvincible(true);
		dashRemainingTime -= dt;
		delay -= dt;

		if (delay >= 0)
		{
			dashDirection = direction;
		}
		else {
			if (!setDashAnim)
			{
				setDashAnim = true;
				GetGameObject()->GetComponent<Animator>()->setCurrentState(2);
			}
		}

		if (dashDirection.x != 0)
		{
			GetGameObject()->m_transform->SetScale(glm::vec3(glm::abs(GetGameObject()->m_transform->GetScale().x) * dashDirection.x, GetGameObject()->m_transform->GetScale().y, GetGameObject()->m_transform->GetScale().z));
		}

		rb->SetVelocity(glm::vec3(dash_speed * dashDirection.x, 0, 0));
	}
}

void PlayerController::mouseAim()
{
	float mouse_x, mouse_y, pos_x, pos_y;

	pos_x = GetGameObject()->m_transform->GetPosition().x;
	pos_y = GetGameObject()->m_transform->GetPosition().y;

	mouse_x = Input::GetMouseWorldPosition().x;
	mouse_y = Input::GetMouseWorldPosition().y;

	angle_deg = ((atan2(mouse_x - pos_x, mouse_y - pos_y) * 180 / PI) - 90) * -1.0f;

	angle_rad = angle_deg / 180 * PI;

	if (m_gameObject->m_transform->GetScale().x > 0) {
		weaponTranform->SetRotation(angle_deg);

		if (angle_deg > 90) {
			if (!inverseGun) {
				weaponTranform->SetScale(glm::vec3(weaponTranform->GetScale().x, -1.0f * weaponTranform->GetScale().y, weaponTranform->GetScale().z));
				inverseGun = true;
			}
		}
		else {
			if (inverseGun) {

				weaponTranform->SetScale(glm::vec3(weaponTranform->GetScale().x, -1.0f * weaponTranform->GetScale().y, weaponTranform->GetScale().z));
				inverseGun = false;
			}
		}

		weaponTranform->SetLocalPosition(glm::vec3(GunDistance * cos(angle_rad), GunDistance * sin(angle_rad), 1));
	}
	else {
		weaponTranform->SetRotation(-1.0f * (angle_deg + 180));

		if (angle_deg < 90) {
			if (!inverseGun) {
				weaponTranform->SetScale(glm::vec3(weaponTranform->GetScale().x, -1.0f * weaponTranform->GetScale().y, weaponTranform->GetScale().z));
				inverseGun = true;
			}
		}
		else {
			if (inverseGun) {

				weaponTranform->SetScale(glm::vec3(weaponTranform->GetScale().x, -1.0f * weaponTranform->GetScale().y, weaponTranform->GetScale().z));
				inverseGun = false;
			}
		}

		weaponTranform->SetLocalPosition(glm::vec3( -1.0f * GunDistance * cos(angle_rad), GunDistance * sin(angle_rad), 1));
	}
}

bool PlayerController::checkGround() 
{
	int hits = 0;
	float pos_x, pos_y, raycastRange;
	Physic::PhysicScene* PhySc = Physic::PhysicScene::GetInstance();
	
	pos_x = GetGameObject()->m_transform->GetPosition().x;
	pos_y = GetGameObject()->m_transform->GetPosition().y;
	raycastRange = 25.0f;

	hits = PhySc->RaycastAll(Physic::Ray(pos_x, pos_y, pos_x, pos_y - raycastRange), PhySc->GetLayerFromString("Platform")).size();
	GLRenderer::GetInstance()->DrawDebug_Line(pos_x, pos_y, pos_x, pos_y - raycastRange, 1.0f, 0.0f, 0.0f);

	if (hits > 0) 
	{
		return true;
	}
	else {
		return false;
	}
}

void PlayerController::cameraZoom(float dt) {

	if (!onGround && jumping && !falling)
	{
		camDelay_count = 0.0f;
		if (Graphic::getCamera()->GetZoom() < camSmall)
		{
			Graphic::getCamera()->Zoom(camZoomOutSpeed);
		}
		else
		{
			Graphic::getCamera()->SetZoom(camSmall);
		}
	}
	else if(onGround) {
		camDelay_count += dt;

		if (camDelay_count > camZoomInDelay) {

			if (Graphic::getCamera()->GetZoom() > camLarge)
			{
				Graphic::getCamera()->Zoom(-camZoomInSpeed);
			}
			else
			{
				Graphic::getCamera()->SetZoom(camLarge);
			}
		}
	}
}

void PlayerController::assignPool(ObjectPool* pool)
{
	this->MGbulletPool = pool;
}

void PlayerController::assignWeapon() 
{
	assignWeapon(Weapons.at(0));
}

void PlayerController::assignWeapon(Weapon* wp) 
{
	if (this->weapon != nullptr)
	{
		this->weapon->GetWeapon()->SetActive(false);
	}
	else 
	{
		this->weapon = wp;
	}

	weaponTranform = wp->GetWeapon()->m_transform.get();
	float currDirY = weapon->GetWeapon()->m_transform->GetScale().y / glm::abs(weapon->GetWeapon()->m_transform->GetScale().y);


	this->weapon = wp;
	wp->GetWeapon()->SetActive(true);
	wp->AssignAngle(&angle_deg);
	wp->SetmodifyObject(m_gameObject);

	//already set in editor, no need to set here
	//wp->GetWeapon()->m_transform->SetParent(m_gameObject->m_transform);


	if (direction.x == -1.0f) 
	{
		wp->GetWeapon()->m_transform->SetScale(glm::vec3(glm::abs(wp->GetWeapon()->m_transform->GetScale().x) * -1.0f, glm::abs(wp->GetWeapon()->m_transform->GetScale().y) * currDirY, 1));
	}
	else {
		wp->GetWeapon()->m_transform->SetScale(glm::vec3(glm::abs(wp->GetWeapon()->m_transform->GetScale().x), glm::abs(wp->GetWeapon()->m_transform->GetScale().y) * currDirY, 1));
	}

	wp->GetWeapon()->m_transform->SetLocalPosition(glm::vec3(1.0f, 0.0f, 0.0f));
}

void PlayerController::AddEquipment(Equipment* e) 
{
	e->SetmodifyObject(m_gameObject);
	Equipments.push_back(e);

	if (Weapon* w = dynamic_cast<Weapon*>(e))
	{
		Weapons.push_back(w);
	}
}

void PlayerController::AddEquipment(GameObject* obj)
{
	Equipment* e = obj->GetComponent<Equipment>();

	if (e != nullptr)
	{
		AddEquipment(e);
	}
}

void PlayerController::RemoveWeapon(int index) 
{
	Weapons.erase(Weapons.begin() + index);
}

void PlayerController::RemoveEquipment(int index)
{
	Equipments.erase(Equipments.begin() + index);
}

void PlayerController::ModifyFromEquipment() 
{
	for (Equipment* e : Equipments)
	{
		if (ArtifactAmplifier* af = dynamic_cast<ArtifactAmplifier*>(e))
		{
			af->Modify();
		}
	}

	for (Equipment* e : Equipments)
	{
		e->Modify();
	}
}

void PlayerController::RevertEquipment() 
{
	for (Equipment* e : Equipments)
	{
		if (Artifact* a = dynamic_cast<Artifact*>(e)) 
		{
			a->Revert();
		}
	}

	for (Equipment* e : Equipments)
	{
		if (Artifact* a = dynamic_cast<Artifact*>(e))
		{
			a->isAmplify = false;
		}
	}

	Equipments.clear();
	Weapons.clear();

	if (weapon != nullptr) {
		weapon->GetGameObject()->SetActive(false);
		weapon = nullptr;
	}
}

void PlayerController::MultiplyMoveSpeed(float value) 
{
	this->move_speed = this->move_speed * value; 
	this->max_move_speed = this->max_move_speed * value; 
}