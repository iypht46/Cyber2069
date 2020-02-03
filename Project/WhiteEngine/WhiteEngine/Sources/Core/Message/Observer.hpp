#pragma once
#include <unordered_map>
#include "MessageType.hpp"
#include "Core/Message/IMessageHandler.hpp"

namespace Core
{
	using SubscriberLists = std::unordered_map<MessageType, IMessageHandler*>;
	class Observer
	{

	};
}


