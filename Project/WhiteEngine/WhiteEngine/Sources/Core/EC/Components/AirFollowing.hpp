#include "Transform.hpp"
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

class AirFollowing : public Component
{
protected:
	Transform* m_target;
	Transform* flyer;
	float m_speed;
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

