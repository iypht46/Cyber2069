#pragma once
#include <vector>
#include "../Core/EC/GameObject.hpp"

//template <class OBJ>
class ObjectPool {
private:
	std::vector<GameObject*> m_objects;
public:
	//void setPrefab();
	//void init(int amount);
	void AddObject(GameObject*);
	int GetPoolSize();
	GameObject* GetGameObject();
	GameObject* GetInactiveObject();
};