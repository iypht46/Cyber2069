#pragma once
#include <vector>
#include <memory>
#include <functional>

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

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
		virtual ~Event() {}
	};

	//============================================
	//Event Template
	//============================================
	template<class T, class ParamT>
	class TypedEvent :public Event {
	protected:
		std::weak_ptr<T> m_HostObject;
		std::function<void(ParamT)> m_Function;

	public:
		ParamT m_Param;

		TypedEvent(std::weak_ptr<T> host, void(T::*functionptr)(ParamT), ParamT defaultParam);
		virtual void Call();

		virtual ~TypedEvent() {}
	};


	std::vector<std::unique_ptr<Event>> m_events;

public:
	template<class T, class ParamT>
	void addEvent(std::weak_ptr<T> host, void(T::*functionptr)(ParamT), ParamT defaultParam);

	void Trigger();

	~EventSystem();
};


//============================================
//Event Definition
//============================================

template<class T, class ParamT>
void EventSystem::TypedEvent<T, ParamT>::Call() {
	(*m_Function)(m_Param);
}

template<class T, class ParamT>
EventSystem::TypedEvent<T, ParamT>::TypedEvent(std::weak_ptr<T> host, void(T::* functionptr)(ParamT), ParamT defaultParam) {
	m_HostObject = host;
	m_Function = std::bind(&T::functionptr, host, std::placeholders::_1);
	m_Param = defaultParam;
}



//============================================
//Event Controller Definition
//============================================

template<class T, class ParamT>
void EventSystem::addEvent(std::weak_ptr<T> host, void(T::* fuctionptr)(ParamT), ParamT defaultParam) {
	std::unique_ptr<Event> newEvent = std::make_unique<TypedEvent<T, ParamT>>(host, fuctionptr, defaultParam);
	m_events.push_back(std::move(newEvent));
}
