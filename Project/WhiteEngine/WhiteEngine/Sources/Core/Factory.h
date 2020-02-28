#pragma once

#include <set>
#include <memory>
#include "EC/Components/BehaviourScript.h"

template <class T>
class Factory {
private:
	static std::set<std::shared_ptr<T>> m_Collection;
	static std::set<T*> m_RawCollection;

public:
	static std::shared_ptr<T> Create();
	static void Add(std::shared_ptr<T>);
	static std::set<T*>& getCollection();
};

template <class T>
std::set<std::shared_ptr<T>> Factory<T>::m_Collection;
template <class T>
std::set<T*> Factory<T>::m_RawCollection;

template <class T>
std::shared_ptr<T> Factory<T>::Create() {
	std::shared_ptr<T> newT = std::make_shared<T>();

	m_Collection.insert(newT);
	m_RawCollection.insert(newT.get());

	return newT;
}

template <class T>
void Factory<T>::Add(std::shared_ptr<T> pointer) {
	m_Collection.insert(pointer);
	m_RawCollection.insert(pointer.get());
}

template <class T>
std::set<T*>& Factory<T>::getCollection() {
	return m_RawCollection;
}