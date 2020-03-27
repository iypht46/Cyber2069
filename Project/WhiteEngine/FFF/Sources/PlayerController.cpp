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
	hpSystem->TakeDamage(1.0f);
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
	Gun = GetGameObject()->m_transform->GetChild(0);
	rb = GetGameObject()->GetComponent<Rigidbody>();
	hpSystem = GetGameObject()->GetComponent<HPsystem>();

	hpSystem->SetMaxHP(200.0f);
	hpSystem->ResetHP();

	inverseGun = false;

	jumping = false;
	falling = false;
	max_move_speed = 200.0f;
	move_speed = 200.0f;
	dash_speed = 750.0f;
	jump_speed = 300.0f;
	direction.x = 1;
	direction.y = 1;

	max_stamina = 50.0f;
	dashStamina = 5.0f;
	jumpStamina = 5.0f;

	stamina = max_stamina;

	camZoomSpeed = 0.005f;
	camDelay = 0.5f;
	//camMaxZoom = 0.75f;
	camMaxZoom = 0.65f;
	//camMinZoom = 1.00f;
	camMinZoom = 1.50f;

	bullet_speed = 300.0f;
	bullet_delay = 0.1f;
	bullet_delay_count = 0.0f;

	dashTime = 0.35f;

	GunDistance = 0.45f;

}

void PlayerController::OnEnable() {

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

			stamina = max_stamina;
		}
	}

	move();

	if (Dash) {
		dash(dt);
	}

	mouseAim();

	shoot(dt);
}

void PlayerController::OnFixedUpdate(float dt)
{
}

void PlayerController::updateDirection() {

}

void PlayerController::move()
{
	glm::vec3 velocity(0, 0, 0);
	//direction = glm::vec2(0);

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


		GetGameObject()->m_transform->SetScale(glm::vec3(glm::abs(GetGameObject()->m_transform->GetScale().x) * -1.0f, GetGameObject()->m_transform->GetScale().y, GetGameObject()->m_transform->GetScale().z));

	}
	else if (Input::GetKeyHold(Input::KeyCode::KEY_D))
	{
		direction.x = 1.0f;
		velocity.x = move_speed * direction.x;
		//rb->SetVelocity(glm::vec3(move_speed * direction.x, rb->GetVelocity().y, rb->GetVelocity().z));
		GetGameObject()->m_transform->SetScale(glm::vec3(glm::abs(GetGameObject()->m_transform->GetScale().x), GetGameObject()->m_transform->GetScale().y, GetGameObject()->m_transform->GetScale().z));
	}

	if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE) && (stamina > 0))
	{
		rb->SetVelocity(glm::vec3(0, jump_speed, 0));
		stamina -= jumpStamina;
		jumping = true;
		running = false;
		falling = false;

		GetGameObject()->GetComponent<Animator>()->setCurrentState(3);
	}

	if (Input::GetKeyDown(Input::KeyCode::KEY_R)) 
	{
		GetGameObject()->m_transform->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
		hpSystem->ResetHP();
		GetGameObject()->SetActive(true);
	}

	if (Input::GetKeyUp(Input::KeyCode::KEY_N))
	{
		if (GLRenderer::GetInstance()->drawDebug) {

			GLRenderer::GetInstance()->drawDebug = false;
		}
		else {
			GLRenderer::GetInstance()->drawDebug = true;
		}
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

	if (GetGameObject()->m_transform->GetScale().x > 0) {
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

		Gun->SetLocalPosition(glm::vec3(GunDistance * cos(angle_rad), GunDistance * sin(angle_rad) + 0.05f, 1));
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

		Gun->SetLocalPosition(glm::vec3( -1.0f * GunDistance * cos(angle_rad), GunDistance * sin(angle_rad) + 0.05f, 1));
	}
}

void PlayerController::shoot(float dt)
{
	bullet_delay_count += dt;
	if (Input::GetMouseHold(Input::MouseKeyCode::MOUSE_LEFT) ||
		Input::GetMouseDown(Input::MouseKeyCode::MOUSE_LEFT))
	{
		if (bullet_delay_count > bullet_delay)
		{
			GameObject* bullet = MGbulletPool->GetInactiveObject();

			if (bullet != nullptr) {
				bullet->SetActive(true);

				float posX = GetGameObject()->m_transform->GetPosition().x + (50 * cos(angle_rad));
				float posY = GetGameObject()->m_transform->GetPosition().y + (50 * sin(angle_rad));
				bullet->m_transform->SetPosition(glm::vec3(posX, posY, 0.0f));
				bullet->m_transform->SetRotation(angle_deg);

				float speedX = bullet_speed * cos(angle_rad);
				float speedY = bullet_speed * sin(angle_rad);

				//if (((rb->GetVelocity().x > 0) && (speedX < 0)) ||
				//	((rb->GetVelocity().x < 0) && (speedX > 0)))
				//{
				//	speedX += -1.0 * rb->GetVelocity().x;
				//}
				//else {
				//	speedX += rb->GetVelocity().x;
				//}

				//if (((rb->GetVelocity().y > 0) && (speedY < 0)) ||
				//	((rb->GetVelocity().y < 0) && (speedY > 0)))
				//{
				//	speedY += -1.0 * rb->GetVelocity().y;
				//}
				//else {
				//	speedY += rb->GetVelocity().y;
				//}

				bullet->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(speedX, speedY, 0.0f));

				bullet_delay_count = 0.0f;
			}
			else {
				//ENGINE_INFO("Where's ma bullet");
			}
		}
		else {
			//ENGINE_INFO("Firen't");
		}
	}

	float mouse_x, mouse_y, pos_x, pos_y;

	pos_x = GetGameObject()->m_transform->GetPosition().x;
	pos_y = GetGameObject()->m_transform->GetPosition().y;

	mouse_x = Input::GetMouseWorldPosition().x;
	mouse_y = Input::GetMouseWorldPosition().y;

	int hits = ps->RaycastAll(Physic::Ray(pos_x, pos_y, mouse_x, mouse_y), ps->GetLayerFromString("Enemy")).size();
	//ENGINE_INFO("{} enemies hit with ray", hits);
	GLRenderer::GetInstance()->DrawDebug_Line(pos_x, pos_y, mouse_x, mouse_y, 1.0f, 0.0f, 0.0f);
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
	ENGINE_INFO("{} Platform hit with ray", hits);
	GLRenderer::GetInstance()->DrawDebug_Line(pos_x, pos_y, pos_x, pos_y - raycastRange, 1.0f, 0.0f, 0.0f);

	if (hits > 0) 
	{
		return true;
	}
	else {
		return false;
	}
}

float PlayerController::GetStamina() {
	return this->stamina;
}

void PlayerController::cameraZoom(float dt) {

	if (!falling && jumping)
	{
		camDelay_count = 0.0f;
		if (Graphic::getCamera()->GetZoom() < camMinZoom)
		{
			Graphic::getCamera()->Zoom(camZoomSpeed);
		}
		else
		{
			Graphic::getCamera()->SetZoom(camMinZoom);
		}
	}
	else {
		camDelay_count += dt;

		if (camDelay_count > camDelay) {

			if (Graphic::getCamera()->GetZoom() > camMaxZoom)
			{
				Graphic::getCamera()->Zoom(-camZoomSpeed);
			}
			else
			{
				Graphic::getCamera()->SetZoom(camMaxZoom);
			}
		}
	}
}

void PlayerController::assignPool(ObjectPool* pool)
{
	this->MGbulletPool = pool;
}
