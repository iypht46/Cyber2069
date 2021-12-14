#include "Artifact.hpp"

#include "PlayerController.hpp"
#include "GameController.hpp"

void BulletAmplifier::Modify() 
{
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr) 
	{
		vector<Weapon*> wps = p->GetWeapons();

		for (Weapon* w : wps) 
		{
			if (isAmplify) {
				w->MultiplyWeaponAmplifier(multiplier_weapon * multiplier_amplifier);
			}
			else {
				w->MultiplyWeaponAmplifier(multiplier_weapon);
			}
		}

		if (isAmplify) {

			p->MultiplyMoveSpeed(multiplier_speedDecrease * multiplier_amplifier);
		}
		else {

			p->MultiplyMoveSpeed(multiplier_speedDecrease);
		}
	}
}

void BulletAmplifier::Revert()
{
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr)
	{
		vector<Weapon*> wps = p->GetWeapons();

		for (Weapon* w : wps)
		{
			if (isAmplify) {
				w->MultiplyWeaponAmplifier(1.0f / (multiplier_weapon * multiplier_amplifier));
			}
			else {
				w->MultiplyWeaponAmplifier(1.0f / multiplier_weapon);
			}
		}

		if (isAmplify) {

			p->MultiplyMoveSpeed(1.0f / (multiplier_speedDecrease * multiplier_amplifier));
		}
		else 
		{
			p->MultiplyMoveSpeed(1.0f / multiplier_speedDecrease);
		}
	}
}

void FireRateUP::Modify() 
{
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr)
	{
		vector<Weapon*> wps = p->GetWeapons();

		for (Weapon* w : wps)
		{
			w->MultiplyWeaponFireRate(multiplier_firerate);
		}
	}
}

void FireRateUP::Revert() 
{
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr)
	{
		vector<Weapon*> wps = p->GetWeapons();

		for (Weapon* w : wps)
		{
			w->MultiplyWeaponFireRate(1.0f / multiplier_firerate);
		}
	}
}

void  AttackUP::Modify()
{
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr)
	{
		vector<Weapon*> wps = p->GetWeapons();

		for (Weapon* w : wps)
		{
			if (isAmplify) {

				w->MultiplyWeaponDamage(multiplier_attack * multiplier_amplifier);
			}
			else 
			{
				w->MultiplyWeaponDamage(multiplier_attack);
			}
		}
	}
}

void AttackUP::Revert()
{
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr)
	{
		vector<Weapon*> wps = p->GetWeapons();

		for (Weapon* w : wps)
		{
			if (isAmplify) 
			{
				w->MultiplyWeaponDamage(1.0f / (multiplier_attack * multiplier_amplifier));
			}
			else {

				w->MultiplyWeaponDamage(1.0f / multiplier_attack);
			}
		}
	}
}

void LowGravity::Modify() 
{
	Rigidbody* rb = modifyObject->GetComponent<Rigidbody>();

	if (isAmplify) {

		rb->SetGravityScale(rb->GetGravityScale() / (multiplier_GravityScale * multiplier_amplifier));
	}
	else {
		rb->SetGravityScale(rb->GetGravityScale() / multiplier_GravityScale);
	}
}

void LowGravity::Revert()
{
	Rigidbody* rb = modifyObject->GetComponent<Rigidbody>();

	if (isAmplify) {

		rb->SetGravityScale(rb->GetGravityScale() * multiplier_GravityScale * multiplier_amplifier);
	}
	else {
		rb->SetGravityScale(rb->GetGravityScale() * multiplier_GravityScale);
	}
}

void SpeedRunner::Modify() {
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (isAmplify) 
	{
		p->MultiplyMoveSpeed(multiplier_moveSpeed * multiplier_amplifier);
	}
	else {
		p->MultiplyMoveSpeed(multiplier_moveSpeed);
	}
}

void SpeedRunner::Revert() {
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (isAmplify) {

		p->MultiplyMoveSpeed(1.0f / (multiplier_moveSpeed * multiplier_amplifier));
	}
	else {
		p->MultiplyMoveSpeed(1.0f / multiplier_moveSpeed);
	}
}

void ArtifactAmplifier::Modify() 
{
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr) 
	{
		vector<Equipment*> eq = p->GetEquipments();

		for (Equipment* e : eq) 
		{
			if (Artifact* a = dynamic_cast<Artifact*>(e)) 
			{
				a->isAmplify = true;
			}
		}
	}
}

void ArtifactAmplifier::Revert() 
{
	/*PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr)
	{
		vector<Equipment*> eq = p->GetEquipments();

		for (Equipment* e : eq)
		{
			if (Artifact* a = dynamic_cast<Artifact*>(e))
			{
				a->isAmplify = false;
			}
		}
	}*/
}

void CursedPendant::Modify() 
{
	GameController::GetInstance()->SetCursedMode(true);
}

void CursedPendant::Revert() 
{
	GameController::GetInstance()->SetCursedMode(false);
}