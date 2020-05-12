#pragma once
//White Engine
#include "Graphic/Shader.h"
#include "Graphic/SquareMeshVbo.h"
#include "Graphic/Framebuffer.hpp"
#include "Graphic/CameraObject.hpp"
#include "Graphic/Texture.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/Components/Collider.hpp"
//Third Party Library
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <SDL_surface.h>
#include <SDL_image.h>
//Standard Library
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <queue>

using namespace std;

enum FBO_STATE {MAIN, SUB};
#ifndef LAYERCOMP
#define LAYERCOMP
struct LayerComparator
{
	bool operator()(const MeshRenderer* lhs, const MeshRenderer* rhs) const
	{
		return lhs->layer < rhs->layer;
	}
};
#endif

class LineVertex {
public:
	float x1, y1, x2, y2, r, g, b;
	LineVertex(float x1, float y1, float x2, float y2,float r,float g,float b)
	{
		this->x1 = x1; this->y1 = y1; this->x2 = x2; this->y2 = y2; this->r = r;this->g = g; this->b = b;
	}
};

class CircleVertex {
public:
	float x, y, radius, r, g, b;
	CircleVertex(float x, float y, float radius, float r, float g, float b) {
		this->x = x; this->y = y; this->radius = radius; this->r = r; this->g = g; this->b = b;
	}
};

class GLRenderer
{
protected:
	static GLRenderer * instance;
	multiset <MeshRenderer*, LayerComparator > MeshSet;
	queue <LineVertex*> Lineq;
	queue <CircleVertex*> Circleq;

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

	GLuint colVBO = -1;
	GLuint colVAO = -1;

	GLuint cirVBO = -1;
	GLuint cirVAO = -1;

	GLuint gProgramId;
	GLuint gProgramId2;
	int gPos2DLocation = -1;
	int gTex2DLocation = -1;
	void PrintProgramLog(GLuint program);
	bool Initialize(string vertexShaderFile, string fragmentShaderFile);
	bool AsgnLayer = false;

	Shader *vertexShader = nullptr;
	Shader *fragmentShader = nullptr;
	Shader *vertexShaderT = nullptr;
	Shader *fragmentShaderT = nullptr;
	glm::vec4* viewPort = nullptr;
public:
	void Render(Graphic::CameraObject* cam);
	void Render(glm::mat4 globalModelTransform);
	static GLRenderer* GetInstance();
	bool InitGL(string vertexShaderFile, string fragmentShaderFile);
	bool drawDebug = false;


	~GLRenderer();

	void SetMeshAttribId(MeshVbo * shape);

	void SetOrthoProjection(float left, float right, float bottom, float top);
	void SetViewPort(int x, int y, int w, int h);
	void SetClearColor(float r, float g, float b);
	void SetClearColor(float r, float g, float b, float w);
	void SetDefaultViewport();

	void AssignLayer();
	void ResetAssignLayer();
	void SetAsgnLayer(bool asgn);

	void AddMeshToSet(MeshRenderer* mesh);

	void RenderDebugCollider(BoxCollider* col, glm::mat4 mat);
	void RenderLine(LineVertex* vertex);
	void RenderCircle(CircleVertex* vertex);

	void DrawDebug_Line(float x1, float y1, float x2, float y2, float r, float g, float b);
	void DrawDebug_Circle(float x, float y, float radius, float r, float g, float b);


	glm::mat4 GetprojectionMatrix();

	GLuint GetModelMatrixAttrId();
	GLuint GetProjectionMatrixAttrId();
	GLuint GetColorUniformId();
	GLuint GetModeUniformId();
	GLuint GetvModeUniformId();
	GLuint GetOffsetXUniformId();
	GLuint GetOffsetYUniformId();
	//GLuint GetProgramID();

	int GetgTex2DLocation();
	int GetgPos2DLocation();

	GLuint GetProgramId();

	GLuint LoadTexture(string path);
	Graphic::Texture LoadTextureNew(std::string path);

	//MeshRenderer* test;

};
