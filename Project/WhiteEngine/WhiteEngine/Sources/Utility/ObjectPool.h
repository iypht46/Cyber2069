#pragma once
#include <vector>
#include <memory>
#include "../Core/EC/GameObject.hpp"

#include <cereal/types/string.hpp>

//template <class OBJ>
class ObjectPool {
private:
	string objectPath;
	std::vector<std::shared_ptr<GameObject>> m_objects;
public:
	//void setPrefab();
	//void init(int amount);
	void AddObject(shared_ptr<GameObject>);
	int GetPoolSize();
	GameObject* GetGameObject();
	GameObject* GetInactiveObject();

	//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			objectPath
		);
	}
};