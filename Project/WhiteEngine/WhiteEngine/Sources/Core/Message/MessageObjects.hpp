#pragma once

#include "Core/Message/Message.hpp"
#include "Core/Message/MessageType.hpp"

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
}