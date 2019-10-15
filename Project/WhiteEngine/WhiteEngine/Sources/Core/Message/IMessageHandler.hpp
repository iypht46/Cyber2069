#pragma once

#include "Core/Message/MessageObjects.hpp"
namespace Core
{
	//Forward Declaration
	class Message;

	//Class Declaration
	class IMessageHandler
	{
	public:
		//Generic Interface
		virtual void HandleMessage(const Message&) {};
		virtual IMessageHandler* GetParent() const {};
		virtual void SendToChildren(const Message&) {};

		//Handle Message for each type
		virtual void HandleMessage(const TestMessage&) {}
		virtual void HandleMessage(const GameCloseMessage&) {}
	};
}