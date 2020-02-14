#pragma once

#include <vector>
#include "EC/Components/BehaviourScript.h"

template <class T>
class Factory {
private:
	static std::vector<T*> m_Collection;

public:
	static T* Create();
	static void Add(T*);
	static std::vector<T*>& getCollection();
};

template <class T>
std::vector<T*> Factory<T>::m_Collection;

template <class T>
T* Factory<T>::Create() {
	T* newT = new T();

	BehaviourScript* script = dynamic_cast<BehaviourScript*>(newT);
	if (script != nullptr) {
		Factory<BehaviourScript>().Add(script);
	}

	m_Collection.push_back(newT);

	return newT;
}

template <class T>
void Factory<T>::Add(T* pointer) {
	m_Collection.push_back(pointer);
}

template <class T>
std::vector<T*>& Factory<T>::getCollection() {
	return m_Collection;
}