#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Core/EC/Components/Component.hpp"
#include "Graphic/MeshVbo.h"
#include <iostream>
#include <map>
#include <string>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Serialization/glmCereal.h"
#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

class GameObject;
class Component;

#ifndef GLCHARACTER
#define GLCHARACTER
struct GLCharacter {
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};
#endif

class TextRenderer : public Component 
{
protected:
	std::string text;
	glm::vec3 color;
	float fontSize;
	std::string fontPath;

	GLuint VAO;
	GLuint VBO;

	std::map<GLchar, GLCharacter> Characters;
public:
	virtual void Init();

	void Render();

	void SetText(string text);
	void SetColor(glm::vec3 color);
	void LoadFont(string path,float DefautFontSize);

	TextRenderer();
	~TextRenderer();

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			text,
			color,
			fontSize,
			fontPath
		);
	}
};

CEREAL_REGISTER_TYPE(TextRenderer);
