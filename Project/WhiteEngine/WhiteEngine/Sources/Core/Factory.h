#pragma once

#include <string>
#include <set>
#include <memory>

#include "Serialization/Serialization.h"
#include "EC/Components/BehaviourScript.h"

//normal factory =========================================

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

//gameObject factory =========================================

template <>
class Factory<GameObject> {
private:
	static std::set<std::shared_ptr<GameObject>> m_Collection;
	static std::set<GameObject*> m_RawCollection;

public:
	static std::shared_ptr<GameObject> Instantiate(std::string);
	static std::shared_ptr<GameObject> Create();
	static void Add(std::shared_ptr<GameObject>);
	static std::set<GameObject*>& getCollection();
};

std::shared_ptr<GameObject> Factory<GameObject>::Instantiate(std::string prefabPath) {
	std::shared_ptr<GameObject> newT = std::make_shared<GameObject>();

	Serialization::LoadObject<GameObject>(*newT, prefabPath);

	m_Collection.insert(newT);
	m_RawCollection.insert(newT.get());

	return newT;
}