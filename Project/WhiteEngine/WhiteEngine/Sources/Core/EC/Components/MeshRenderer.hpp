#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Component.hpp"
#include "Graphic/MeshVbo.h"
#include <iostream>

#include "Core/EC/Components/Animator.hpp"

#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

class GameObject;
class Component;

#define SpritePath(path) "Sources/Assets/Sprites/" path ".png" 

class MeshRenderer : public Component
{
private:
	bool isUI = false;
	
	std::string sr_texturePath;
	float sr_NumFrameX = 1;
	float sr_NumFrameY = 1;

	MeshVbo* mesh;

	Animator* anim;
	unsigned int texture;

	glm::vec3 ReplaceColor;

public:
	int layer = 0; //higher layer is infront
	bool inSet = false;

	MeshRenderer();
	MeshRenderer(std::string texture_path, float NumframeX, float NumFrameY);
	~MeshRenderer();

	virtual void Init();

	void CreateMesh(float NumframeX, float NumFrameY);
	void SetTexture(std::string path);
	void SetTexture(unsigned int tex);
	//higher layer is in front, player is on 0
	void SetLayer(unsigned int layer);
	void SetUI(bool ui);
	void SetReplaceColor(glm::vec3 color);
	void SetReplaceColor(std::string hexcode);
	void RemoveReplaceColor();

	int GetLayer();

	void Render(glm::mat4 globalModelTransform);

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			isUI,
			sr_texturePath,
			sr_NumFrameX,
			sr_NumFrameY,
			layer,
			ReplaceColor,
			inSet
		);
	}
};

CEREAL_REGISTER_TYPE(MeshRenderer);