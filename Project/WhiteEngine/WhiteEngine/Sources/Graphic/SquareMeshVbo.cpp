
#include "SquareMeshVbo.h"

string const SquareMeshVbo::MESH_NAME = "square";

void SquareMeshVbo::LoadData(float NumframeX, float NumFrameY)
{
	//VBO data
	GLfloat vertexData[] =
	{
	  -0.5f, -0.5f,
	  0.5f, -0.5f,
	  0.5f,  0.5f,
	  -0.5f,  0.5f
	};

	float StartX = 1.0f / NumframeX;
	float StartY = (NumFrameY - 1.0f) / NumFrameY;

	GLfloat texData[] =
	{
	  0.0f, StartY,
	  StartX, StartY,
	  StartX, 1.0f,
	  0.0f, 1.0f
	};

	glGenVertexArrays(1, &(this->posVaoId));


	glGenBuffers(1, &(this->posVboId));
	glGenBuffers(1, &(this->texVboId));

	glBindVertexArray(this->posVaoId);

	//Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->posVboId);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, this->texVboId);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), texData, GL_STATIC_DRAW);


	printf("Load Data\n");
}

SquareMeshVbo::SquareMeshVbo()
{
}

string SquareMeshVbo::GetMeshName()
{
	return MESH_NAME;
}

void SquareMeshVbo::Render()
{
	if (this->posAttribId != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, this->posVboId);
		glVertexAttribPointer(this->posAttribId, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
		glEnableVertexAttribArray(0);
	}
	if (this->texAttribId != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, this->texVboId);
		glVertexAttribPointer(this->texAttribId, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
		glEnableVertexAttribArray(1);
	}

	glBindVertexArray(this->posVaoId);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}