#include "Core/Message/MessageObjects.hpp"
#include "Core/Message/IMessageHandler.hpp"

namespace Core
{
	MSG_GENERATE_DF(TestMessage)

	MSG_GENERATE_DF(GameCloseMessage)

	MSG_GENERATE_DF(Collision)

	MSG_GENERATE_DF(Trigger)
}