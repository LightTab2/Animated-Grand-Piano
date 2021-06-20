#include "global.h"

glm::vec3 calcDir(float kat_x, float kat_y, glm::vec4 dir)
{
	glm::mat4 M = glm::rotate(glm::mat4(1), kat_y, glm::vec3(0, 1, 0));
	M = glm::rotate(M, kat_x, glm::vec3(1, 0, 0));
	dir = M * dir;
	return glm::vec3(dir);
}
