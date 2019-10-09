#pragma once

#include <iostream>
#include <GL\glew.h>
#include "Graphic/Shader.h"
#include <map>
#include <vector>
#include <string>
#include "glm/glm.hpp"

using namespace std;

class GLRenderer
{
protected:
	static GLRenderer * instance;

	int winWidth;
	int winHeight;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	GLuint mMatrixId = -1;
	GLuint pMatrixId = -1;
	GLuint colorUniformId = -1;
	GLuint modeUniformId = -1;

	GLuint gProgramId;
	int gPos2DLocation;
	int gTex2DLocation;
	void PrintProgramLog(GLuint program);
	bool Initialize(string vertexShaderFile, string fragmentShaderFile);
	Shader *vertexShader;
	Shader *fragmentShader;
	//map <string, MeshVbo*> Mesh;
	//void SetMeshAttribId(MeshVbo * shape);

public:
	void Render();
	static GLRenderer* GetInstance();
	GLRenderer(int w, int h);
	bool InitGL(string vertexShaderFile, string fragmentShaderFile);

	//void AddMesh(string name, MeshVbo* shape);
	//MeshVbo * GetMesh(string name);

	~GLRenderer();

	void SetOrthoProjection(float left, float right, float bottom, float top);
	void SetViewPort(int x, int y, int w, int h);
	void SetClearColor(float r, float g, float b);

	GLuint GetModelMatrixAttrId();
	GLuint GetProjectionMatrixAttrId();
	GLuint GetColorUniformId();
	GLuint GetModeUniformId();

	//GLuint LoadTexture(string path);

};
