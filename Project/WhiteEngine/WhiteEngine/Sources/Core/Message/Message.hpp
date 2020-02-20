#pragma once

//

#define MSG_GENERATE_DC()	\
virtual void SendMessageTo(IMessageHandler&) const override;

#define MSG_GENERATE_DF(MSG_TYPE)	\
void MSG_TYPE::SendMessageTo(IMessageHandler& handler) const	\
{	\
	handler.HandleMessage(*this);	\
}

namespace Core
{
	//Forward Declaration
	enum class MessageType;
	class IMessageHandler;

	struct Message
	{
		MessageType m_type;

		//Constructors
		Message (MessageType type) : m_type(type) {}

		//For Sending Message
		virtual void SendMessageTo(IMessageHandler&) const = 0;
	
	};

}