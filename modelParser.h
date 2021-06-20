#pragma once
#include "global.h"
#include "lodepng.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLuint readTexture(const char* filename);

class LoadedModel
{
public: //przemielić na private
	float		*vertices = nullptr,
				*normals = nullptr,
				*texCoords = nullptr,
				*colors = nullptr;

	unsigned	*indexes = nullptr;

	int			verticesCount = 0;

	GLuint		tex = NULL,
				texSpec = NULL;

	bool		dataPointedOutside = false;

	unsigned	hasIndexes = 0;

public:
	LoadedModel() = default;

	LoadedModel(float* vertices, float* normals, float* texCoords,
		int verticesCount, GLuint* tex_ptr = NULL, GLuint* texspec_ptr = NULL) :
		vertices(vertices), normals(normals), texCoords(texCoords),
		verticesCount(verticesCount)
	{
		this->dataPointedOutside = true;
	}

	LoadedModel(const char* model_path, GLuint* tex_ptr = NULL, GLuint* texspec_ptr = NULL);

	LoadedModel(const aiScene* sc, int id = 0);

	~LoadedModel();

	GLuint getTexture() { return tex; }
	GLuint getSpecialTexture() { return texSpec; }

	void setTexture(GLuint tex) { this->tex = tex; }
	void setSpecialTexture(GLuint texSpec) { this->texSpec = texSpec; }

	void loadTex(const char* filename) { tex = readTexture(filename); }

	void loadTexSpec(const char* filename) { texSpec = readTexture(filename); }

	static bool loadObj(const std::string &pFile, LoadedModel *&model, int id = 0, int aibit = 0);
};

