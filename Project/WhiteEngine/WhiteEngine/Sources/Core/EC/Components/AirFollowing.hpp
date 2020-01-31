#include "Transform.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

class AirFollowing : public Component
{
protected:
	Transform* player;
	Transform* flyer;
	float flySpeed;
	float rotAngle;
	float rotRate;
public:
	AirFollowing();
	void SetPlayer(Transform&);
	void SetFlySpeed(float value);
	void SetRotAngle(float value);
	void SetRotRate(float value);
	void FollowPlayer(float dt);
};

