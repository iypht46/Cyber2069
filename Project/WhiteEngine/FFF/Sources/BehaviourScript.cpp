#include "BehaviourScript.h"
#include "Core/Factory.h"

BehaviourScript::BehaviourScript() {
	Factory<BehaviourScript>::Add(this);
}