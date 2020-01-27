#pragma once

#include "Core/Message/Message.hpp"
#include "Core/Message/MessageType.hpp"

//Forward Declaration
#include "Physic/Collision.hpp"

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

	struct Trigger : Message
	{
		bool m_isStart = true;
		Physic::Collision* m_collision;
		Trigger() : Message(MessageType::MSG_TRIGGER) {}
		Trigger(bool isStart, Physic::Collision* col) 
			: Message(MessageType::MSG_TRIGGER), m_isStart(isStart), m_collision(col) {}

		MSG_GENERATE_DC()
	};

	struct Collision : Message
	{
		bool m_isStart = true;
		Physic::Collision* m_collision;
		Collision() : Message(MessageType::MSG_COLLISION) {}
		Collision(bool isStart, Physic::Collision* col) 
			: Message(MessageType::MSG_COLLISION), m_isStart(isStart), m_collision(col) {}

		MSG_GENERATE_DC()
	};
}