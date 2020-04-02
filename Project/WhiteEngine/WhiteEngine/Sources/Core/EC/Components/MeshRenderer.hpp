#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Component.hpp"
#include "Graphic/MeshVbo.h"
#include <iostream>

#include "Core/EC/Components/Animator.hpp"

class GameObject;
class Component;

class MeshRenderer : public Component
{
private:
	unsigned int texture;

	bool isUI;
	bool isReplaceColor;
	MeshVbo* mesh;

	Animator* anim;

	glm::vec3 ReplaceColor;

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
	void SetReplaceColor(glm::vec3 color);
	void RemoveReplaceColor();

	int GetLayer();

	void Render(glm::mat4 globalModelTransform);
};
