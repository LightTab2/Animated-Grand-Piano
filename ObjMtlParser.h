#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h> 
#include <assimp/postprocess.h> 


class ObjMtlParser
{
public:
static GLuint readTexture(
		const char* filename
	);
};

class LoadedObjModel {
public:
	bool _dataPointedOutside = false;
	float* _vertices = NULL;
	float* _normals = NULL;
	float* _texCoords = NULL;
	float* _colors = NULL;
	unsigned int* _indexes;
	int _verticesCount;
	GLuint _tex;
	GLuint* _tex_ptr;
	GLuint _texspec;
	GLuint* _texspec_ptr;
	unsigned int hasIndexes = 0;

public:
	LoadedObjModel();

	~LoadedObjModel();

	LoadedObjModel(
		float* vertices,
		float* normals,
		float* texCords,
		int verticesCount,
		GLuint* tex_ptr = NULL,
		GLuint* texspec_ptr = NULL);

	LoadedObjModel(
		const char* model_path,
		GLuint* tex_ptr = NULL,
		GLuint* texspec_ptr = NULL);
	LoadedObjModel(
		const aiScene* sc,
		int id = 0
	);

	GLuint tex();

	GLuint tex_spec();

	void setTexPtr(
		GLuint* tex_ptr
	);

	void setTex_specPtr(
		GLuint* texspec_ptr
	);

	void setTex(
		GLuint tex
	);

	void setTex_spec(
		GLuint texspec
	);

	void loadTex(
		const char* filename
	);

	void loadTex_spec(
		const char* filename
	);

	void delete_tex();
	void delete_texspec();

	void setColors(
		float* colors
	);
};

bool loadObj(const std::string& pFile, LoadedObjModel** model, int id=0,int aibit=0);
bool loadMultipleObj(const std::string& pFile, LoadedObjModel*** model, int n, int aibit=0);
bool loadAnimationObj(const std::string* pFile, LoadedObjModel** model, int n, int aibit = 0);

