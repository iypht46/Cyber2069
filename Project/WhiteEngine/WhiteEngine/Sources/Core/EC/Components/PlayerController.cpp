#include "PlayerController.hpp"
#include "Input/Input.hpp"
#include "Graphic/Camera.hpp"
#include <math.h>

PlayerController::PlayerController() {

}

void PlayerController::OnAwake() {

}

void PlayerController::OnDisable() {

}

void PlayerController::OnStart() {
	Gun = m_gameObject->m_transform.GetChild(0);
	rb = m_gameObject->GetComponent<Rigidbody>();

	inverseGun = false;

	jumping = false;
	falling = false;
	move_speed = 200.0f;
	dash_speed = 400.0f;
	jump_speed = 200.0f;
	direction.x = 1;
	direction.y = 1;

	bullet_speed = 800.0f;
	bullet_delay = 0.05f;

	dashTime = 0.35f;

	GunDistance = 0.45f;
}

void PlayerController::OnEnable() {
	
}

void PlayerController::OnUpdate(float dt) 
{
	/*if (m_gameObject->m_transform.GetPosition().y < -(720 / 2))
	{
		jumping = false;
		m_gameObject->m_transform.SetPosition(glm::vec3(m_gameObject->m_transform.GetPosition().x, -(720 / 2) + 1, m_gameObject->m_transform.GetPosition().z));
	}*/

	Graphic::getCamera()->SetPos(glm::vec3(m_gameObject->m_transform.GetPosition().x, m_gameObject->m_transform.GetPosition().y, m_gameObject->m_transform.GetPosition().z));

	if ((rb->GetVelocity().y < -5.0f) && !falling)
	{
		falling = true;
	}

	if (falling) 
	{
		if (rb->GetVelocity().y >= 0) 
		{
			jumping = false;
			falling = false;
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
	glm::vec3 velocity = glm::vec3(0);
	//direction = glm::vec2(0);

	if (Input::GetKeyHold(Input::KeyCode::KEY_W))
	{
		direction.y = 1.0f;
		velocity.y = move_speed * direction.y;
	}
	else if (Input::GetKeyHold(Input::KeyCode::KEY_S)) 
	{
		direction.y = -1.0f;
		velocity.y = move_speed * direction.y;
	}

	if (Input::GetKeyHold(Input::KeyCode::KEY_A))
	{
		direction.x = -1.0f;
		velocity.x = move_speed * direction.x;

		m_gameObject->m_transform.SetScale(glm::vec3(glm::abs(m_gameObject->m_transform.GetScale().x) * -1.0f, m_gameObject->m_transform.GetScale().y, m_gameObject->m_transform.GetScale().z));

	}
	else if (Input::GetKeyHold(Input::KeyCode::KEY_D))
	{
		direction.x = 1.0f;
		velocity.x = move_speed * direction.x;
		m_gameObject->m_transform.SetScale(glm::vec3(glm::abs(m_gameObject->m_transform.GetScale().x), m_gameObject->m_transform.GetScale().y, m_gameObject->m_transform.GetScale().z));
	}

	if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
	{
		rb->SetVelocity(glm::vec3(0, jump_speed, 0));
		jumping = true;
		running = false;
		falling = false;

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

	if (Input::GetKeyDown(Input::KeyCode::KEY_LEFT_SHIFT) && !Dash) 
	{
		dashRemainingTime = dashTime;
		Dash = true;
		setDashAnim = false;

		delay = 0.1f;
	}
	
	if (!Dash) 
	{
		rb->SetVelocity(glm::vec3(velocity.x, rb->GetVelocity().y, rb->GetVelocity().z));
		//rb->SetVelocity(glm::vec3(velocity.x, velocity.y, rb->GetVelocity().z));
	}
	
}

void PlayerController::dash(float dt) 
{

	if (dashRemainingTime <= 0) 
	{
		running = false;
		m_gameObject->GetComponent<Animator>()->setCurrentState(4);
		Dash = false;
	}
	else 
	{
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


	//std::cout << angle_deg << std::endl;
}

void PlayerController::shoot(float dt) 
{
	if (Input::GetMouseHold(Input::MouseKeyCode::MOUSE_LEFT)) 
	{
		bullet_delay_count += dt;
		if (bullet_delay_count > bullet_delay) 
		{

			GameObject* bullet = MGbulletPool->GetInactiveObject();

			bullet->SetActive(true);

			float posX = m_gameObject->m_transform.GetPosition().x + (50 * cos(angle_rad));
			float posY = m_gameObject->m_transform.GetPosition().y + (50 * sin(angle_rad));
			bullet->m_transform.SetPosition(glm::vec3(posX, posY, 0.0f));
			bullet->m_transform.SetRotation(angle_deg);

			bullet->GetComponent<Rigidbody>()->SetVelocity(glm::vec3(rb->GetVelocity().x + (bullet_speed * cos(angle_rad)), rb->GetVelocity().y + (bullet_speed * sin(angle_rad)), 0.0f));

			bullet_delay_count = 0.0f;
		}
	}
}

void PlayerController::assignPool(ObjectPool* pool) 
{
	this->MGbulletPool = pool;
}