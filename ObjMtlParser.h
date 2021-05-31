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
static bool loadOBJ(
		const char* path,
		std::vector < glm::vec4 >& out_vertices,
		std::vector < glm::vec2 >& out_uvs,
		std::vector < glm::vec4 >& out_normals
	);


static GLuint readTexture(
		const char* filename
	);
};

class LoadedObjModel {
public:
	float* _vertices;
	float* _normals;
	float* _texCoords;
	float* _colors = NULL;
	int _verticesCount;
	GLuint* _tex;
	GLuint* _texspec;
	std::vector <glm::vec4> vecVertices;
	std::vector <glm::vec2> vecTexCoords;
	std::vector <glm::vec4> vecNormals;

public:
	LoadedObjModel();

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
		const aiScene* sc);

	GLuint tex();

	GLuint tex_spec();

	void setTex(
		GLuint* tex_ptr
	);

	void setTex_spec(
		GLuint* texspec_ptr
	);

	void setColors(
		float* colors
	);
};

bool DoTheImportThing(const std::string& pFile, LoadedObjModel* model);

