#include "lilPiano.h"

glm::vec3 HammerPositions[8] =
{
	glm::vec3(-0.574033f, 0.564779f, 0.566373f),
	glm::vec3(-0.4389f, 0.564779f, 0.566373f),
	glm::vec3(-0.285781f, 0.564779f, 0.566373f),
	glm::vec3(-0.127831f, 0.564779f, 0.566373f),

	glm::vec3(0.051862f, 0.564779f, 0.566373f),
	glm::vec3(0.204517f, 0.564779f, 0.566373f),
	glm::vec3(0.366092f, 0.564779f, 0.566373f),
	glm::vec3(0.520791f, 0.564779f, 0.566373f)
};

float HammerAngles[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

LoadedModel* ToyPianoBase,
			* ToyPianoHammer,
			* ToyPianoLid,
			* Floor;

float lidAngle = static_cast<float>(PI) / 12.f;
double lidAnimationTime = 3;
double lidAnimationTimeEnd = 2;
bool lidOpen = false;

LoadedModel* ToyPianoButtons[8];
bool ToyPianoButtonsPress[8]	= { false, false, false, false, false, false, false, false, };
double ButtonAnimationTime[8]	= { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };
double ButtonAnimationTimeEnd	= 0.1;
float ToyPianoButtonsDepth[8]	= { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
float ToyPianoBUttonDepthEnd	= 0.066f;

void lilPianoPress(int i)
{
	ToyPianoButtonsPress[i] = true;
	if (ButtonAnimationTime[i] <= ButtonAnimationTimeEnd)
		ButtonAnimationTime[i] = ButtonAnimationTimeEnd - ButtonAnimationTime[i];
	else
		ButtonAnimationTime[i] = 0;
}

void lilPianoRelease(int i)
{
	ToyPianoButtonsPress[i] = false;
	if (ButtonAnimationTime[i] <= ButtonAnimationTimeEnd)
		ButtonAnimationTime[i] = ButtonAnimationTimeEnd - ButtonAnimationTime[i];
	else
		ButtonAnimationTime[i] = 0;
}

void lilPianoAnim()
{
	if (lidAnimationTime <= lidAnimationTimeEnd)
	{
		lidAnimationTime = lidAnimationTime + glfwGetTime();
		if (lidOpen)
		{
			if (lidAnimationTime >= lidAnimationTimeEnd)
				lidAngle = 120.0 * TO_RAD;
			else
				lidAngle = (15.0 + (105.0 * (lidAnimationTime / lidAnimationTimeEnd))) * TO_RAD;
		}
		else
		{
			if (lidAnimationTime >= lidAnimationTimeEnd)
				lidAngle = 15.0 * TO_RAD;
			else
				lidAngle = (120.0 - (105.0 * (lidAnimationTime / lidAnimationTimeEnd))) * TO_RAD;
		}
	}

	for (unsigned i = 0; i != 8; ++i)
	{
		if (ButtonAnimationTime[i] <= ButtonAnimationTimeEnd)
		{
			ButtonAnimationTime[i] = ButtonAnimationTime[i] + glfwGetTime();
			if (ToyPianoButtonsPress[i])
			{
				if (ButtonAnimationTime[i] >= ButtonAnimationTimeEnd)
				{
					HammerAngles[i] = 13.5f * static_cast<float>(TO_RAD);
					ToyPianoButtonsDepth[i] = ToyPianoBUttonDepthEnd;
				}
				else
				{
					HammerAngles[i] = (13.5f * (ButtonAnimationTime[i] / ButtonAnimationTimeEnd)) * TO_RAD;
					ToyPianoButtonsDepth[i] = ToyPianoBUttonDepthEnd * (ButtonAnimationTime[i] / ButtonAnimationTimeEnd);
				}
			}
			else
			{
				if (ButtonAnimationTime[i] >= ButtonAnimationTimeEnd)
				{
					HammerAngles[i] = 0.0f;
					ToyPianoButtonsDepth[i] = 0;
				}
				else
				{
					HammerAngles[i] = (13.5f - (13.5f * (ButtonAnimationTime[i] / ButtonAnimationTimeEnd))) * TO_RAD;
					ToyPianoButtonsDepth[i] = ToyPianoBUttonDepthEnd * (1.0 - ButtonAnimationTime[i] / ButtonAnimationTimeEnd);
				}
			}
		}
	}
}