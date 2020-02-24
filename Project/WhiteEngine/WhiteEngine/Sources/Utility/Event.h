#pragma once
#include <vector>

//============================================
//Event Controller
//============================================
class EventSystem {
private:

	//============================================
	//Event Abstract
	//============================================
	class Event {
	public:
		virtual void Call() = 0;
	};

	//============================================
	//Event Template
	//============================================
	template<class ParamT>
	class TypedEvent :public Event {
	protected:
		void (*m_Function) (ParamT param);

	public:
		ParamT m_param;

		TypedEvent(void(*functionptr)(ParamT), ParamT defaultParam);
		virtual void Call();
	};

	std::vector<Event*> m_events;

public:
	template<class ParamT>
	void addEvent(void(*fuctionptr)(ParamT defaultParam));

	void Trigger();

	~EventSystem();
};


//============================================
//Event Definition
//============================================

template<class ParamT>
void EventSystem::TypedEvent<ParamT>::Call() {
	return (*m_Function)(m_param);
}

template<class ParamT>
EventSystem::TypedEvent<ParamT>::TypedEvent(void(*functionptr)(ParamT), ParamT defaultParam) {
	m_Function = functionptr;
	m_param = defaultParam;
}



//============================================
//Event Controller Definition
//============================================

template<class ParamT>
void EventSystem::addEvent(void(*fuctionptr)(ParamT defaultParam)) {
	Event = new TypedEvent<ParamT>(fuctionptr, defaultParam);
}

void EventSystem::Trigger() {
	for (Event* event : m_events) {
		event->Call();
	}
}

EventSystem::~EventSystem() {
	delete[] m_events.data();
	m_events.clear();
}