#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>

#include "EC/Components/Component.hpp"

#include "Serialization/Serialization.h"

//Normal Factory ===========================================================
template <class Base, class T>
class Factory {
private:
	static std::set<T*> m_Collection;

public:
	static std::shared_ptr<T> Create(std::string);
	static std::shared_ptr<T> Create();

	static void Add(T*);
	
	static std::set<T*>& getCollection();
};

template <class Base, class T>
std::set<T*> Factory<Base, T>::m_Collection;

//create and load data from file (must be the path of same object type)
template <class Base, class T>
std::shared_ptr<T> Factory<Base, T>::Create(std::string prefabPath) {
	std::shared_ptr<T> newT = std::make_shared<T>();

	Serialization::LoadObject<T>(*(newT.get()), prefabPath);

	m_Collection.insert(newT.get());

	return newT;
}

template <class Base, class T>
std::shared_ptr<T> Factory<Base, T>::Create() {
	std::shared_ptr<T> newT = std::make_shared<T>();

	m_Collection.insert(newT.get());

	return newT;
}

template <class Base, class T>
void Factory<Base, T>::Add(T* element) {
	m_Collection.insert(element);
}

template <class Base, class T>
std::set<T*>& Factory<Base, T>::getCollection() {
	return m_Collection;
}
//-----------------------------------------------------------

//Component Factory ===========================================================
template <class T>
class Factory<Component, T> {
private:
	static std::map<int, T*> m_Collection;

public:
	static std::shared_ptr<T> Create(std::string);
	static std::shared_ptr<T> Create();

	static void Add(T*);
	static void Remove(T*);

	static std::map<int, T*>& getCollection();
};

template <class T>
std::map<int, T*> Factory<Component, T>::m_Collection;

//create and load data from file (must be the path of same object type)
template <class T>
std::shared_ptr<T> Factory<Component, T>::Create(std::string prefabPath) {
	std::shared_ptr<T> newT = std::make_shared<T>();

	Serialization::LoadObject<T>(*(newT.get()), prefabPath);

	std::shared_ptr <Component> comp = std::dynamic_pointer_cast<Component>(newT);
	m_Collection[comp->getComponentID()] = newT.get();

	return newT;
}

template <class T>
std::shared_ptr<T> Factory<Component, T>::Create() {
	std::shared_ptr<T> newT = std::make_shared<T>();

	std::shared_ptr <Component> comp = std::dynamic_pointer_cast<Component>(newT);
	m_Collection[comp->getComponentID()] = newT.get();

	return newT;
}

template <class T>
void Factory<Component, T>::Add(T* element) {
	Component* comp = dynamic_cast<Component*>(element);
	m_Collection[comp->getComponentID()] = element;
}

template <class T>
void Factory<Component, T>::Remove(T* element) {
	Component* comp = dynamic_cast<Component*>(element);
	m_Collection.erase(m_Collection.find(comp->getComponentID()));
}

template <class T>
std::map<int, T*>& Factory<Component, T>::getCollection() {
	return m_Collection;
}
//-----------------------------------------------------------