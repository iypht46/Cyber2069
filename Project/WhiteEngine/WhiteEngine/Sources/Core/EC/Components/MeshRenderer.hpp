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
namespace Tools { class MeshRendererEC; }

class MeshRenderer : public Component
{
	friend class Tools::MeshRendererEC;
private:
	bool m_isUI = false;

	std::string sr_texturePath;
	float sr_NumFrameX;
	float sr_NumFrameY;

	bool isReplaceColor = false;
	MeshVbo* mesh;
	Animator* anim;
	unsigned int texture;
	bool m_texLoaded = false;
	glm::vec3 ReplaceColor;

public:
	int layer = -1;
	bool inSet = false;

	MeshRenderer();
	MeshRenderer(std::string texture_path, float NumframeX, float NumFrameY);
	~MeshRenderer();

	virtual void Init();

	void CreateMesh(float NumframeX, float NumFrameY);
	void SetTexture(std::string path);
	void SetTexture(unsigned int tex);
	void SetLayer(unsigned int layer);
	void SetUI(bool ui);
	void SetReplaceColor(glm::vec3 color);
	void RemoveReplaceColor();

	int GetLayer();
	bool IsUI();

	void Render(glm::mat4 globalModelTransform);
	unsigned int GetTexture();
//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			m_isUI,
			sr_texturePath,
			sr_NumFrameX,
			sr_NumFrameY,
			layer,
			inSet
		);
	}
};

CEREAL_REGISTER_TYPE(MeshRenderer);
