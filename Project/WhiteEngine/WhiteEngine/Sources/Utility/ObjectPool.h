#pragma once
#include <vector>
#include "../Core/EC/GameObject.hpp"

//template <class OBJ>
class ObjectPool {
private:
	string objectPath;
	std::vector<shared_ptr<GameObject>> m_objects;
public:
	//void setPrefab();
	//void init(int amount);
	void AddObject(shared_ptr<GameObject>);
	int GetPoolSize();
	shared_ptr<GameObject> GetGameObject();
	shared_ptr<GameObject> GetInactiveObject();

	//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			objectPath
		);
	}
};