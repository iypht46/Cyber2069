#pragma once

#include <vector>

template <class T>
class Factory {
private:
	static std::vector<T*> m_Collection;

public:
	static T* Create();
	static std::vector<T*>& getCollection();
};

template <class T>
std::vector<T*> Factory<T>::m_Collection;

template <class T>
T* Factory<T>::Create() {
	T* newT = new T();
	m_Collection.push_back(newT);

	return newT;
}

template <class T>
std::vector<T*>& Factory<T>::getCollection() {
	return m_Collection;
}