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
		virtual void HandleMessage(const Message&) {}
		virtual IMessageHandler* GetParent() const { return nullptr; }
		virtual void SendToChildren(const Message&) {}

		//Handle Message for each type
		virtual void HandleMessage(const TestMessage&) {}
		virtual void HandleMessage(const GameCloseMessage&) {}
	};
}