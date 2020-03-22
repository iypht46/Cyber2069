#include "TextRenderer.hpp"
#include "Graphic/GLRenderer.h"
#include "Core/EC/GameObject.hpp"

TextRenderer::TextRenderer() 
{
	glGenVertexArrays(1, &(this->VAO));
	glGenBuffers(1, &(this->VBO));
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Factory<TextRenderer>::Add(this);
}

void TextRenderer::Init() {
	LoadFont(fontPath, fontSize);
}

void TextRenderer::LoadFont(string path, float DefautFontSize) {
	
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, path.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, DefautFontSize);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		GLCharacter character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		
		Characters.insert(std::pair<GLchar, GLCharacter>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::SetText(string text) 
{
	this->text = text;
}

void TextRenderer::SetColor(glm::vec3 color) {
	this->color = color;
}

void TextRenderer::Render() 
{
	//// Activate corresponding render state	
	GLuint modeId = GLRenderer::GetInstance()->GetModeUniformId();
	GLuint vmodeId = GLRenderer::GetInstance()->GetvModeUniformId();
	GLuint ColorUniformId = GLRenderer::GetInstance()->GetColorUniformId();
	GLuint modelMatixId = GLRenderer::GetInstance()->GetModelMatrixAttrId();

	GLuint offsetXId = GLRenderer::GetInstance()->GetOffsetXUniformId();
	GLuint offsetYId = GLRenderer::GetInstance()->GetOffsetYUniformId();

	glUniform1f(offsetXId, 0.0f);
	glUniform1f(offsetYId, 0.0f);
	

	glm::mat4 currentMatrix;

	//glm::mat4 modelMatrix = GetGameObject()->m_transform->GetModelMatrix();
	glm::mat4 projectionMatrix = GLRenderer::GetInstance()->GetprojectionMatrix();

	currentMatrix = projectionMatrix;
	

	glUniform1i(modeId, 2);
	glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
	
	glUniform3f(ColorUniformId, color.x, color.y, color.z);

	glUniformMatrix4fv(glGetUniformLocation(GLRenderer::GetInstance()->GetProgramId(), "projection"), 1, GL_FALSE, glm::value_ptr(GLRenderer::GetInstance()->GetprojectionMatrix()));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	float x = GetGameObject()->m_transform->GetPosition().x;
	float y = GetGameObject()->m_transform->GetPosition().y;

	glm::vec2 scale(GetGameObject()->m_transform->GetScale().x, GetGameObject()->m_transform->GetScale().y);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		GLCharacter ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale.x;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale.y;

		GLfloat w = ch.Size.x * scale.x;
		GLfloat h = ch.Size.y * scale.y;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		
		
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale.x; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

}