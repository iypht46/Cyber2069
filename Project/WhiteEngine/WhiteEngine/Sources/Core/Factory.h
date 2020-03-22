#pragma once

#include <string>
#include <set>
#include <memory>

#include "Serialization/Serialization.h"

template <class T>
class Factory {
private:
	static std::set<T*> m_Collection;

public:
	T* Instantiate(std::string);
	static std::shared_ptr<T> Create();

	static void Add(T*);
	
	static std::set<T*>& getCollection();
};

template <class T>
std::set<T*> Factory<T>::m_Collection;

template <class T>
T* Factory<T>::Instantiate(std::string prefabPath) {
	std::shared_ptr<T> newT = std::make_shared<T>();

	Serialization::LoadObject<T>(*(newT.get()), prefabPath);

	m_Collection.insert(newT.get());

	return newT.get();
}

template <class T>
std::shared_ptr<T> Factory<T>::Create() {
	std::shared_ptr<T> newT = std::make_shared<T>();

	m_Collection.insert(newT.get());

	return newT;
}

template <class T>
void Factory<T>::Add(T* rawPointer) {
	m_Collection.insert(rawPointer);
}

template <class T>
std::set<T*>& Factory<T>::getCollection() {
	return m_Collection;
}
