#include "MeshRenderer.hpp"
#include "Graphic/SquareMeshVbo.h"
#include "Graphic/GLRenderer.h"
#include "Core/EC/GameObject.hpp"

MeshRenderer::MeshRenderer() 
{
}

MeshRenderer::~MeshRenderer()
{
}


MeshRenderer::MeshRenderer(std::string texture_path,float NumframeX,float NumFrameY)
{
	//SetGameObject(m_gameObject);
	SetTexture(texture_path);

	mesh = new SquareMeshVbo();
	mesh->LoadData(NumframeX, NumFrameY);
	GLRenderer::GetInstance()->SetMeshAttribId(mesh);

}

void MeshRenderer::SetTexture(std::string path)
{
	texture = GLRenderer::GetInstance()->LoadTexture(path);
}

void  MeshRenderer::CreateMesh(float NumframeX, float NumFrameY)
{
	mesh = new SquareMeshVbo();
	mesh->LoadData(NumframeX, NumFrameY);
	GLRenderer::GetInstance()->SetMeshAttribId(mesh);
}

void MeshRenderer::Render(glm::mat4 globalModelTransform)
{
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *>(this->mesh);

	GLuint modelMatixId = GLRenderer::GetInstance()->GetModelMatrixAttrId();
	GLuint modeId = GLRenderer::GetInstance()->GetModeUniformId();

	GLuint offsetXId = GLRenderer::GetInstance()->GetOffsetXUniformId();
	GLuint offsetYId = GLRenderer::GetInstance()->GetOffsetYUniformId();

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

	vector<glm::mat4> matrixStack;

	glm::mat4 currentMatrix = GetGameObject()->m_transform.GetModelMatrix();

	if (squareMesh != nullptr)
	{
		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(modeId, 1);

		//-------Animation--------

		if (GetGameObject()->GetComponent<Animator>() != nullptr) 
		{
			glUniform1f(offsetXId, GetGameObject()->GetComponent<Animator>()->GetCurrentUVFrame().x);
			glUniform1f(offsetYId, GetGameObject()->GetComponent<Animator>()->GetCurrentUVFrame().y);
		}
		else {
			glUniform1f(offsetXId, 0.0f);
			glUniform1f(offsetYId, 0.0f);
		}

		/*glUniform1f(offsetXId, 0);
		glUniform1f(offsetYId, 0);*/

		glBindTexture(GL_TEXTURE_2D, texture);
		squareMesh->Render();
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