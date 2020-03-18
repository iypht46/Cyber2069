#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Component.hpp"
#include "Graphic/MeshVbo.h"
#include <iostream>

#include "Core/EC/Components/Animator.hpp"

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>

class GameObject;
class Component;

class MeshRenderer : public Component
{
private:
	bool isUI;
	
	string sr_texturePath;
	float sr_NumFrameX;
	float sr_NumFrameY;

	Animator* anim;
	unsigned int texture;
	MeshVbo* mesh;

public:
	int layer = -1;
	bool inSet = false;

	MeshRenderer();
	MeshRenderer(std::string texture_path, float NumframeX, float NumFrameY);
	~MeshRenderer();
	void CreateMesh(float NumframeX, float NumFrameY);
	void SetTexture(std::string path);
	void SetTexture(unsigned int tex);
	void SetLayer(unsigned int layer);
	void SetUI(bool ui);

	int GetLayer();

	void Render(glm::mat4 globalModelTransform);

//serialization
private:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			isUI,
			sr_texturePath,
			sr_NumFrameX,
			sr_NumFrameY,
			layer,
			inSet
		);
	}
};

CEREAL_REGISTER_TYPE(MeshRenderer);