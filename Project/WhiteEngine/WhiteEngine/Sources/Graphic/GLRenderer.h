#pragma once

#include <iostream>
#include <GL\glew.h>
#include "Graphic/Shader.h"
#include <map>
#include <vector>
#include <string>
#include <set>
#include "glm/glm.hpp"
#include "SDL_surface.h"
#include "SDL_image.h"
#include "Graphic/SquareMeshVbo.h"

#include "Core/EC/Components/MeshRenderer.hpp"

using namespace std;

#ifndef LAYERCOMP
#define LAYERCOMP
struct LayerComparator
{
	bool operator()(const MeshRenderer* lhs, const MeshRenderer* rhs)
	{
		return lhs->layer < rhs->layer;
	}
};
#endif

class GLRenderer
{
protected:
	static GLRenderer * instance;
	multiset <MeshRenderer*, LayerComparator > MeshSet;
	
	GLRenderer(int w, int h);

	int winWidth;
	int winHeight;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	GLuint mMatrixId = -1;
	GLuint pMatrixId = -1;
	GLuint colorUniformId = -1;
	GLuint modeUniformId = -1;
	GLuint vmodeUniformId = -1;
	GLuint offSetXId = -1;
	GLuint offSetYId = -1;

	GLuint gProgramId;
	GLuint gProgramId2;
	int gPos2DLocation = -1;
	int gTex2DLocation = -1;
	void PrintProgramLog(GLuint program);
	bool Initialize(string vertexShaderFile, string fragmentShaderFile);

	Shader *vertexShader;
	Shader *fragmentShader;
	
	Shader *vertexShaderT;
	Shader *fragmentShaderT;
public:
	void Render();
	static GLRenderer* GetInstance();
	bool InitGL(string vertexShaderFile, string fragmentShaderFile);

	~GLRenderer();

	void SetMeshAttribId(MeshVbo * shape);

	void SetOrthoProjection(float left, float right, float bottom, float top);
	void SetViewPort(int x, int y, int w, int h);
	void SetClearColor(float r, float g, float b);

	void AddMeshToSet(MeshRenderer* mesh);

	glm::mat4 GetprojectionMatrix();

	GLuint GetModelMatrixAttrId();
	GLuint GetProjectionMatrixAttrId();
	GLuint GetColorUniformId();
	GLuint GetModeUniformId();
	GLuint GetvModeUniformId();
	GLuint GetOffsetXUniformId();
	GLuint GetOffsetYUniformId();

	int GetgTex2DLocation();
	int GetgPos2DLocation();
	
	GLuint GetProgramId();

	GLuint LoadTexture(string path);

	//MeshRenderer* test;

};
