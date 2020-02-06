#pragma once

#include "Core/Message/Message.hpp"
#include "Core/Message/MessageType.hpp"

//Forward Declaration
#include "Physic/Collision.hpp"
//#include "Core/EC/Components/Rigidbody.hpp"
//#include "Core/EC/Components/Collider.hpp"


namespace Core
{

	struct TestMessage : Message
	{
		TestMessage() : Message(MessageType::MSG_TEST) {}

		MSG_GENERATE_DC()

	};

	struct GameCloseMessage : Message
	{
		GameCloseMessage() : Message(MessageType::MSG_GAME_CLOSE) {}

		MSG_GENERATE_DC()

	};

	//struct PhysicObjectAddMessage : Message
	//{
	//	union
	//	{
	//		Rigidbody* rigid;
	//		Collider* col;
	//	} m_objToAdd;
	//};

	//struct PhysicObjectRemoveMessage : Message
	//{
	//	union
	//	{
	//		Rigidbody* rigid;
	//		Collider* col;
	//	} m_objToRemove, m_objToAdd;
	//};

	
	struct Trigger : Message
	{
		Physic::Collision m_collision;
		Trigger();
		Trigger(Physic::Collision);

		MSG_GENERATE_DC()
	};

	inline Trigger::Trigger() : Message(MessageType::MSG_TRIGGER) {}
	inline Trigger::Trigger(Physic::Collision col)
		: Message(MessageType::MSG_TRIGGER), m_collision(col) {}

	struct Collision : Message
	{
		Physic::Collision m_collision;
		Collision();
		Collision(Physic::Collision);

		MSG_GENERATE_DC()
	};

	inline Collision::Collision() : Message(MessageType::MSG_COLLISION) {}
	inline Collision::Collision(Physic::Collision col) : Message(MessageType::MSG_COLLISION), m_collision(col) {}
}