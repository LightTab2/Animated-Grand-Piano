#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <ctime>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE
#define PI 3.141592653589793
#define TO_RAD 0.0174532925199
extern glm::vec3 calcDir(float kat_x, float kat_y, glm::vec4 dir = glm::vec4(0, 0, 1, 0));
extern bool	pink,
			l1,
			l2,
			bRunMidi;