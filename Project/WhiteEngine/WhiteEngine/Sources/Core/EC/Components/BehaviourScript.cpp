#include "BehaviourScript.h"
#include "../../Factory.h"

BehaviourScript::BehaviourScript() {
	Factory<BehaviourScript>::Add(this);
}