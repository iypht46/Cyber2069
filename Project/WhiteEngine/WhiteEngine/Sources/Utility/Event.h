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
	template<class T, class ParamT>
	class TypedEvent :public Event {
	protected:
		T* m_HostObject;
		void (T::*m_Function) (ParamT param);

	public:
		ParamT m_Param;

		TypedEvent(T* host, void(T::*functionptr)(ParamT), ParamT defaultParam);
		virtual void Call();
	};


	std::vector<Event*> m_events;

public:
	template<class T, class ParamT>
	void addEvent(T* host, void(T::*fuctionptr)(ParamT), ParamT defaultParam);

	void Trigger();

	~EventSystem();
};


//============================================
//Event Definition
//============================================

template<class T, class ParamT>
void EventSystem::TypedEvent<T, ParamT>::Call() {
	(m_HostObject->*m_Function)(m_Param);
}

template<class T, class ParamT>
EventSystem::TypedEvent<T, ParamT>::TypedEvent(T* host, void(T::*functionptr)(ParamT), ParamT defaultParam) {
	m_HostObject = host;
	m_Function = functionptr;
	m_Param = defaultParam;
}



//============================================
//Event Controller Definition
//============================================

template<class T, class ParamT>
void EventSystem::addEvent(T* host, void(T::*fuctionptr)(ParamT), ParamT defaultParam) {
	Event* newEvent = new TypedEvent<T, ParamT>(host, fuctionptr, defaultParam);
	m_events.push_back(newEvent);
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