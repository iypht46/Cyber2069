#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Component.hpp"
#include "Graphic/MeshVbo.h"
#include <iostream>
#include <map>

#include "ft2build.h"
#include FT_FREETYPE_H

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
	string text;
	glm::vec3 color;
	float fontSize;
	string fontPath;

	GLuint VAO;
	GLuint VBO;

	std::map<GLchar, GLCharacter> Characters;
public:
	void Render();
	
	void SetText(string text);
	void SetColor(glm::vec3 color);
	void LoadFont(string path,float DefautFontSize);

	TextRenderer();

//serialization
private:
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
