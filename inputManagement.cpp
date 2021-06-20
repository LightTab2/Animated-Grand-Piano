#include "inputManagement.h"

float	speed_x = 0,
		speed_y = 0,
		speed = 1;

SoundSystem* soundPtr = nullptr;

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		switch (key)
		{
		case GLFW_KEY_LEFT:
			speed_x = 1;
			break;
		case GLFW_KEY_RIGHT:
			speed_x = -1;
			break;
		case GLFW_KEY_UP:
			speed_y = 1;
			break;
		case GLFW_KEY_DOWN:
			speed_y = -1;
			break;
		case GLFW_KEY_RIGHT_SHIFT:
			speed = 5;
			break;
		case GLFW_KEY_Z:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::C4);
			break;
		case GLFW_KEY_S:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::Cis4);
			break;
		case GLFW_KEY_X:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::D4);
			break;
		case GLFW_KEY_D:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::Dis4);
			break;
		case GLFW_KEY_C:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::E4);
			break;
		case GLFW_KEY_V:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::F4);
			break;
		case GLFW_KEY_G:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::Fis4);
			break;
		case GLFW_KEY_B:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::G4);
			break;
		case GLFW_KEY_H:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::Gis4);
			break;
		case GLFW_KEY_N:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::A5);
			break;
		case GLFW_KEY_J:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::Ais5);
			break;
		case GLFW_KEY_M:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::B5);
			break;
		case GLFW_KEY_COMMA:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::C5);
			break;
		case GLFW_KEY_L:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::Cis5);
			break;
		case GLFW_KEY_PERIOD:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::D5);
			break;
		case GLFW_KEY_SEMICOLON:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::Dis5);
			break;
		case GLFW_KEY_SLASH:
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::E5);
			break;
		case GLFW_KEY_O:
			lidOpen = !lidOpen;
			if (lidAnimationTime <= lidAnimationTimeEnd)
				lidAnimationTime = lidAnimationTimeEnd - lidAnimationTime;
			else
				lidAnimationTime = 0;
			break;
		case GLFW_KEY_Q:
			lilPianoPress(0);
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::C4);
			break;
		case GLFW_KEY_W:
			lilPianoPress(1);
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::D4);
			break;
		case GLFW_KEY_E:
			lilPianoPress(2);
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::E4);
			break;
		case GLFW_KEY_R:
			lilPianoPress(3);
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::F4);
			break;
		case GLFW_KEY_T:
			lilPianoPress(4);
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::G4);
			break;
		case GLFW_KEY_Y:
			lilPianoPress(5);
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::A5);
			break;
		case GLFW_KEY_U:
			lilPianoPress(6);
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::B5);
			break;
		case GLFW_KEY_I:
			lilPianoPress(7);
			if (soundPtr) soundPtr->playNote(SoundSystem::Note::C5);
			break;
		default:
			break;
		}
	break;
	case GLFW_RELEASE:
		switch (key)
		{
		case GLFW_KEY_LEFT:
			if (speed_x == 1) speed_x = 0;
			break;
		case GLFW_KEY_RIGHT:
			if (speed_x == -1) speed_x = 0;
			break;
		case GLFW_KEY_UP:
			if (speed_y == 1) speed_y = 0;
			break;
		case GLFW_KEY_DOWN:
			if (speed_y == -1) speed_y = 0;
			break;
		case GLFW_KEY_RIGHT_SHIFT:
			speed = 1;
			break;
		case GLFW_KEY_Z:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::C4);
			break;
		case GLFW_KEY_S:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::Cis4);
			break;
		case GLFW_KEY_X:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::D4);
			break;
		case GLFW_KEY_D:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::Dis4);
			break;
		case GLFW_KEY_C:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::E4);
			break;
		case GLFW_KEY_V:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::F4);
			break;
		case GLFW_KEY_G:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::Fis4);
			break;
		case GLFW_KEY_B:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::G4);
			break;
		case GLFW_KEY_H:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::Gis4);
			break;
		case GLFW_KEY_N:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::A5);
			break;
		case GLFW_KEY_J:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::Ais5);
			break;
		case GLFW_KEY_M:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::B5);
			break;
		case GLFW_KEY_COMMA:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::C5);
			break;
		case GLFW_KEY_L:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::Cis5);
			break;
		case GLFW_KEY_PERIOD:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::D5);
			break;
		case GLFW_KEY_SEMICOLON:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::Dis5);
			break;
		case GLFW_KEY_SLASH:
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::E5);
			break;
		case GLFW_KEY_Q:
			lilPianoRelease(0);
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::C4);
			break;
		case GLFW_KEY_W:
			lilPianoRelease(1);
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::D4);
			break;
		case GLFW_KEY_E:
			lilPianoRelease(2);
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::E4);
			break;
		case GLFW_KEY_R:
			lilPianoRelease(3);
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::F4);
			break;
		case GLFW_KEY_T:
			lilPianoRelease(4);
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::G4);
			break;
		case GLFW_KEY_Y:
			lilPianoRelease(5);
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::A5);
			break;
		case GLFW_KEY_U:
			lilPianoRelease(6);
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::B5);
			break;
		case GLFW_KEY_I:
			lilPianoRelease(7);
			if (soundPtr) soundPtr->stopPlayingNote(SoundSystem::Note::C5);
			break;
		default:
			break;
		}
		break;
	case GLFW_REPEAT:
		break;
	default:
		break;
	}
}

