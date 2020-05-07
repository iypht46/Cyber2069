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
			w->MultiplyWeaponAmplifier(multiplier_weapon);
		}

		p->MultiplyMoveSpeed(multiplier_speedDecrease);
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
			w->MultiplyWeaponAmplifier(1.0f / multiplier_weapon);
		}

		p->MultiplyMoveSpeed(1.0f / multiplier_speedDecrease);
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
			w->MultiplyWeaponDamage(multiplier_attack);
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
			w->MultiplyWeaponDamage(1.0f / multiplier_attack);
		}
	}
}

void LowGravity::Modify() 
{
	Rigidbody* rb = modifyObject->GetComponent<Rigidbody>();

	rb->SetGravityScale(rb->GetGravityScale() / multiplier_GravityScale);
}

void LowGravity::Revert()
{
	Rigidbody* rb = modifyObject->GetComponent<Rigidbody>();

	rb->SetGravityScale(rb->GetGravityScale() * multiplier_GravityScale);
}

void SpeedRunner::Modify() {
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	p->MultiplyMoveSpeed(multiplier_moveSpeed);
}

void SpeedRunner::Revert() {
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	p->MultiplyMoveSpeed(1.0f / multiplier_moveSpeed);
}

void ArtifactAmplifier::Modify() 
{
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr) 
	{
		vector<Equipment*> eq = p->GetEquipments();

		for (Equipment* e : eq) 
		{
			if (ArtifactAmplifier* af = dynamic_cast<ArtifactAmplifier*>(e))
			{
				continue;
			}
			else if (Artifact* a = dynamic_cast<Artifact*>(e)) 
			{
				a->Modify();
			}
		}
	}
}

void ArtifactAmplifier::Revert() 
{
	PlayerController* p = modifyObject->GetComponent<PlayerController>();

	if (p != nullptr)
	{
		vector<Equipment*> eq = p->GetEquipments();

		for (Equipment* e : eq)
		{
			if (ArtifactAmplifier* af = dynamic_cast<ArtifactAmplifier*>(e))
			{
				continue;
			}
			else if (Artifact* a = dynamic_cast<Artifact*>(e))
			{
				a->Revert();
			}
		}
	}
}

void CursedPendant::Modify() 
{
	GameController::GetInstance()->SetCursedMode(true);
}

void CursedPendant::Revert() 
{
	GameController::GetInstance()->SetCursedMode(false);
}