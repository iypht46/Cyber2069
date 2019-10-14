#include "MeshRenderer.hpp"
#include "Graphic/SquareMeshVbo.h"
#include "Graphic/GLRenderer.h"


MeshRenderer::~MeshRenderer()
{
}

MeshRenderer::MeshRenderer(std::string texture_path)
{
	//SetGameObject(m_gameObject);
	//SetTexture(texture_path);

	/*mesh = new SquareMeshVbo();
	mesh->LoadData();
	GLRenderer::GetInstance()->AddMesh(SquareMeshVbo::MESH_NAME, mesh);*/
}

void MeshRenderer::SetTexture(std::string path)
{
	//texture = GLRenderer::GetInstance()->LoadTexture(path);
}

void MeshRenderer::Render(glm::mat4 globalModelTransform)
{
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *>(GLRenderer::GetInstance()->GetMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = GLRenderer::GetInstance()->GetModelMatrixAttrId();
	GLuint modeId = GLRenderer::GetInstance()->GetModeUniformId();
	GLuint colorId = GLRenderer::GetInstance()->GetColorUniformId();

	if (modelMatixId == -1)
	{
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (modeId == -1)
	{
		cout << "Error: Can't set mode in ImageObject " << endl;
		return;
	}
	if (colorId == -1) {
		cout << "Error: Can't set color " << endl;
		return;
	}

	vector<glm::mat4> matrixStack;

	glm::vec3 pos = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 size = glm::vec3(2.0, 1.0, 1.0);
	glm::mat4 transform = glm::mat4(1.0);

	transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0));
	transform = glm::scale(transform, glm::vec3(size.x, size.y, 1));

	glm::mat4 currentMatrix = transform;

	//glm::mat4 currentMatrix = this->getTransform();

	/*if (squareMesh != nullptr)
	{
		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(modeId, 1);
		glBindTexture(GL_TEXTURE_2D, texture);
		squareMesh->Render();
	}*/

	//glm::mat4 currentMatrix = glm::mat4(1.0);

	if (squareMesh != nullptr) {

		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, 0.0f, 1.0f, 0.0f);
		glUniform1i(modeId, 0);
		squareMesh->Render();

	}
	else
	{
		cout << "squareMesh = NULL\n";
	}
}

void MeshRenderer::OnAwake()
{
}

void MeshRenderer::OnEnable()
{
}

void MeshRenderer::OnStart()
{
}

void MeshRenderer::OnDisable()
{
}

void MeshRenderer::OnUpdate(float dt)
{
}

void MeshRenderer::OnFixedUpdate(float dt)
{
}

void MeshRenderer::OnDestroy()
{
}