#include "Event.h"

void EventSystem::Trigger() {
	for (Event* event : m_events) {
		event->Call();
	}
}

EventSystem::~EventSystem() {
	delete[] m_events.data();
	m_events.clear();
}
