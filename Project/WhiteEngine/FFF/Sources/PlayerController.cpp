#include "PlayerController.hpp"
#include "Input/Input.hpp"
#include "Graphic/Camera.hpp"
#include "Graphic/GLRenderer.h"
#include "Core/Logger.hpp"
#include <math.h>

PlayerController::PlayerController() {

}

void PlayerController::OnAwake() {

}

void PlayerController::OnDisable() {

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

void PlayerController::OnStart() {
	rb = m_gameObject->GetComponent<Rigidbody>();
	hpSystem = m_gameObject->GetComponent<HPsystem>();

	hpSystem->SetMaxHP(100.0f);

	inverseGun = false;

	jumping = false;
	falling = false;
	max_move_speed = 200.0f;
	move_speed = 200.0f;
	dash_speed = 750.0f;
	jump_speed = 300.0f;
	direction.x = 1;
	direction.y = 1;

	max_stamina = 500000000.0f;
	dashStamina = 5.0f;
	jumpStamina = 5.0f;
	staminaRegenRate = 1.0f;

	stamina = max_stamina;

	camZoomOutSpeed = 0.005f;
	camZoomInSpeed = 0.01f;

	camZoomInDelay = 0.0f;
	//camMaxZoom = 0.75f;
	camLarge = 0.65f;
	//camMinZoom = 1.00f;
	camSmall = 1.50f;

	dashTime = 0.35f;

	GunDistance = 0.45f;

	AddEquipment(new MachineGun());
	AddEquipment(new LaserGun());

	assignWeapon(Weapons[0]);

	for (Equipment* e : Equipments) 
	{
		e->Modify(this->m_gameObject);
	}

}

void PlayerController::OnEnable() {

}

void PlayerController::OnUpdate(float dt)
{
	Graphic::getCamera()->SetPos(glm::vec3(m_gameObject->m_transform.GetPosition().x, m_gameObject->m_transform.GetPosition().y, m_gameObject->m_transform.GetPosition().z));

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

	mouseAim();

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
			w->GameTimeBehaviour(dt);
		}
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
		m_gameObject->m_transform.SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
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

	if (Input::GetKeyDown(Input::KeyCode::KEY_1))
	{
		assignWeapon(Weapons[0]);
	}
	
	if (Input::GetKeyDown(Input::KeyCode::KEY_2))
	{
		assignWeapon(Weapons[1]);
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


		m_gameObject->m_transform.SetScale(glm::vec3(glm::abs(m_gameObject->m_transform.GetScale().x) * -1.0f, m_gameObject->m_transform.GetScale().y, m_gameObject->m_transform.GetScale().z));

	}
	else if (Input::GetKeyHold(Input::KeyCode::KEY_D))
	{
		direction.x = 1.0f;
		velocity.x = move_speed * direction.x;
		//rb->SetVelocity(glm::vec3(move_speed * direction.x, rb->GetVelocity().y, rb->GetVelocity().z));
		m_gameObject->m_transform.SetScale(glm::vec3(glm::abs(m_gameObject->m_transform.GetScale().x), m_gameObject->m_transform.GetScale().y, m_gameObject->m_transform.GetScale().z));
	}

	if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE) && (stamina > 0))
	{
		rb->SetVelocity(glm::vec3(0, jump_speed, 0));
		stamina -= jumpStamina;
		jumping = true;
		running = false;
		falling = false;
		onGround = false;

		m_gameObject->GetComponent<Animator>()->setCurrentState(3);
	}

	if ((!Input::GetKeyHold(Input::KeyCode::KEY_A) && !Input::GetKeyHold(Input::KeyCode::KEY_D)) && !jumping && !falling)
	{
		if (!Dash) {
			running = false;
			m_gameObject->GetComponent<Animator>()->setCurrentState(0);
		}
	}
	else if ((Input::GetKeyHold(Input::KeyCode::KEY_A) || Input::GetKeyHold(Input::KeyCode::KEY_D)) && !jumping && !falling) {

		if (!running && !Dash) {
			running = true;
			m_gameObject->GetComponent<Animator>()->setCurrentState(1);
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
		m_gameObject->GetComponent<Animator>()->setCurrentState(4);
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
				m_gameObject->GetComponent<Animator>()->setCurrentState(2);
			}
		}

		if (dashDirection.x != 0)
		{
			m_gameObject->m_transform.SetScale(glm::vec3(glm::abs(m_gameObject->m_transform.GetScale().x) * dashDirection.x, m_gameObject->m_transform.GetScale().y, m_gameObject->m_transform.GetScale().z));
		}

		rb->SetVelocity(glm::vec3(dash_speed * dashDirection.x, 0, 0));
	}
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
	
	pos_x = m_gameObject->m_transform.GetPosition().x;
	pos_y = m_gameObject->m_transform.GetPosition().y;
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

void PlayerController::assignWeapon(Weapon* wp) 
{


	weaponTranform = &(wp->GetWeapon()->m_transform);
	float currDirY = weaponTranform->GetScale().y / glm::abs(weaponTranform->GetScale().y);

	if (this->weapon != nullptr) 
	{
		this->weapon->onDisable();
	}

	weapon = wp;

	wp->GetWeapon()->SetActive(true);
	wp->AssignAngle(&angle_deg);
	wp->SetGameObject(m_gameObject);
	wp->GetWeapon()->m_transform.SetParent(&m_gameObject->m_transform);

	wp->GetWeapon()->m_transform.SetScale(glm::vec3(70, 70 * currDirY, 1));
	wp->GetWeapon()->m_transform.SetLocalPosition(glm::vec3(1, 0, 0));
}

void PlayerController::AddEquipment(Equipment* e) 
{
	e->SetGameObject(m_gameObject);
	Equipments.push_back(e);

	if (Weapon* w = dynamic_cast<Weapon*>(e))
	{
		Weapons.push_back(w);
	}
}
