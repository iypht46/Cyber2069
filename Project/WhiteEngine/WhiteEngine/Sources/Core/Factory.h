#pragma once

#include <vector>
#include <memory>
#include "EC/Components/BehaviourScript.h"

template <class T>
class Factory {
private:
	static std::vector<std::shared_ptr<T>> m_Collection;
	static std::vector<T*> m_RawCollection;

public:
	static std::shared_ptr<T> Create();
	static void Add(std::shared_ptr<T>);
	static std::vector<T*>& getCollection();
};

template <class T>
std::vector<std::shared_ptr<T>> Factory<T>::m_Collection;
template <class T>
std::vector<T*> Factory<T>::m_RawCollection;

template <class T>
std::shared_ptr<T> Factory<T>::Create() {
	std::shared_ptr<T> newT = std::make_shared<T>();

	m_Collection.push_back(newT);
	m_RawCollection.push_back(newT.get());

	return newT;
}

template <class T>
void Factory<T>::Add(std::shared_ptr<T> pointer) {
	m_Collection.push_back(pointer);
	m_RawCollection.push_back(pointer.get());
}

template <class T>
std::vector<T*>& Factory<T>::getCollection() {
	return m_RawCollection;
}