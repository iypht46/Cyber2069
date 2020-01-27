#pragma once

#include "Core/Message/Message.hpp"
#include "Core/Message/MessageObjects.hpp"
namespace Core
{
	//Class Declaration
	class IMessageHandler
	{
	public:
		//Generic Interface
		inline virtual void HandleMessage(const Message&) {}
		inline virtual IMessageHandler* GetParent() const { return nullptr; }
		inline virtual void SendToChildren(const Message&) {}

		//Handle Message for each type
		inline virtual void HandleMessage(const TestMessage&) {}
		inline virtual void HandleMessage(const GameCloseMessage&) {}
		inline virtual void HandleMessage(const Trigger&) {}
		inline virtual void HandleMessage(const Collision&) {}
	};
}