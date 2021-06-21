#pragma once
#include "global.h"
#include "sound.h"
#include "lilPiano.h"
#include "grandPiano.h"

extern float	speed_x,
				speed_y,
				speed;

extern SoundSystem* soundPtr;

void mouseCallback(GLFWwindow* window, int button, int action, int mods);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);