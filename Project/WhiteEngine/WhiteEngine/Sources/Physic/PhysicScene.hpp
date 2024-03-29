#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <bitset>
#include <utility>
#include <algorithm>
#include <string>

#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/Logger.hpp"
#include "Physic/Collision.hpp"

namespace Physic
{
#define GRAVITY_MUL 1.0f
#define LAYER_BIT 8

	enum class Layer : unsigned
	{
		PHYSIC_LAYER_1 = 0x01,
		PHYSIC_LAYER_2 = 0x02,
		PHYSIC_LAYER_3 = 0x04,
		PHYSIC_LAYER_4 = 0x08,
		PHYSIC_LAYER_5 = 0x10,
		PHYSIC_LAYER_6 = 0x20,
		PHYSIC_LAYER_7 = 0x40,
		PHYSIC_LAYER_8 = 0x80,
		PHYSIC_LAYER_NUM = 0x09,
		LAYER_INVALID = 0x11
	};

	//Collider
	using LayerBit = std::bitset<LAYER_BIT>;
	//Store pair of the main layerbit and the set collision layerbit
	using LayerBitPair = std::pair<LayerBit, LayerBit>;
	using Colliders = std::vector<Collider*>;
	using Bodies = std::vector<Rigidbody*>;
	using CollidersMap = std::unordered_map<Layer, Colliders>;

	//Collision
	using CollisionLayer = std::unordered_map<Layer, LayerBitPair>;
	using Manifolds = std::vector<Manifold>;
	using LayerStringMap = std::unordered_map<std::string, Layer>;
	using CollisionMsgLists = std::vector<Collision>;
	using RayHits = std::vector<RayHit>;

	class PhysicScene
	{
	private:
		static PhysicScene* instance;
		//Store layerbit as key and collider as value.
		CollidersMap m_colliders;
		Bodies m_bodies;
		//Store layer enum as key and layerbit as value.
		CollisionLayer m_collisionLayer;
		//Store possible collision
		Manifolds m_possibleCollision;
		Manifolds m_finalCollision;
		LayerStringMap m_layerString;

		//Store Collision Callback Lists
		CollisionMsgLists m_collisionMsg;
		CollisionMsgLists m_stayState;

		//Physics Setting
		glm::vec3 m_gravity;

	protected:
		//Use in class only//
		//Update layer collision and generate pair
		void UpdateLayerCollision();
		//Resolve layer collision pair
		void ResolveLayerCollision(float dt);
		//Update all bodies transform
		void UpdateTransform(float dt);
		//Apply Gravity
		void ApplyGravity(float dt);
		//Check for and remove duplicate collision pair
		void CheckDuplicatePair();
		//Check collision of a layer with another one
		void CheckLayerCollision(Layer, Layer, RESOLVE_TYPE);
		//Check collision state and put into each container
		void SetCollisionState(Collision);

	public:
		//Main update loop
		void Update(float);

		//Interface//
		//@Adding and Removing Collider
		//Add collider to layer
		void Add(Collider*, Layer);
		void Add(Collider*, std::string);
		void Add(Rigidbody*);
		//Remove collider from layer
		void Remove(Collider*, Layer);
		void Remove(Collider*, std::string);
		void Remove(Rigidbody*);
		//Send Collision Message to Collider in list
		void SendCollisionMsg(void);

		//@Utility
		//Get Layer to collide with
		LayerBit GetLayerCollisions(Layer);
		LayerBit GetLayerCollisions(std::string);
		//Map string with layer enum
		void SetLayerName(std::string, Layer);
		//Get layer enum from layer string map
		Layer GetLayerFromString(std::string);
		//Get string from layer
		std::string GetStringFromLayer(Layer);
		//Convert Layer Enum to number
		static uint32_t LayerToNum(Layer);
		//Get Colliders of specified Layer
		Colliders GetColliderLayer(Layer);
		Colliders GetColliderLayer(std::string);

		RayHits RaycastAll(Ray, Layer);
		RayHit Raycast(Ray, Layer);

		//@Physic Settings
		//Set Scene Gravity
		void SetGravity(glm::vec3);
		//Set Layer to Collide With
		void SetLayerCollisions(Layer, Layer, RESOLVE_TYPE);
		void SetLayerCollisions(std::string, std::string, RESOLVE_TYPE);
		//Reset Layer to Collide With
		void ResetLayerCollisions(Layer, Layer);
		void ResetLayerCollisions(std::string, std::string);

		static PhysicScene* GetInstance();

		PhysicScene();
		~PhysicScene() {}
	};
}
