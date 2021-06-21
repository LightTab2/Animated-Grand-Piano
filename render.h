#pragma once
#include "global.h"
#include "shaderprogram.h"
#include "inputManagement.h"
#include "modelParser.h"

extern float aspectRatio;
class RenderingSystem
{
public:
	RenderingSystem();
	~RenderingSystem();
	void drawScene(float angle_x, float angle_y);
	bool shouldClose();
	void getCursorPos(double &angle_y, double &angle_x);
	void setCursorPos(double angle_y, double angle_x);
	void updatePos(glm::vec3 pos) { this->pos += pos; };

private:
	void drawModel(LoadedModel* ptrModel);
	void drawAnimatedModel(LoadedModel** ptrModel, int frame);
	void LoadModels();
	
	GLFWwindow* window;
	ShaderProgram* sp;
	GLuint tex0, tex1;

	glm::vec3	light1 = glm::vec3(0.f, 4.f, 8.f),
				light2 = glm::vec3(1.f, 20.f, -3.28f);
	glm::vec3	pos = glm::vec3(0, 1.7f, 2.4f);
};

