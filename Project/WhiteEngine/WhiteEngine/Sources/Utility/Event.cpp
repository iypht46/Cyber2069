#include "Event.h"

void EventSystem::Trigger() {
	//unique vector have to be iterate like this, I don't fulley understand what this means
	for (auto&& event : m_events) {
		event->Call();
	}
}

EventSystem::~EventSystem() {

}
