#pragma once
#include "grandPiano.h"

LoadedModel		*GrandPianoBase,
				*GrandPianoStrings,
				*GrandPianoLid,
				*GrandPianoHammer[22],
				*leftKey,
				*rightKey,
				*middleKey,
				*blackKey;

LoadedModel		*GrandPianoButtons[87];
bool			GrandPianoButtonsPress[87],
				GrandPianoMalletReleased[87];
double			GrandButtonAnimationTime[87],
				GrandButtonAnimationTimeEnd = 0.05f;
float			GrandPianoButtonDepthEnd = -0.025f,
				GrandButtonOffsets[87];


void grandPianoInit()
{
	for (int i = 0; i != 87; ++i)
		GrandPianoButtonsPress[i] = GrandPianoMalletReleased[i] = false;

	for (int i = 0; i != 87; ++i)
		GrandButtonAnimationTime[i] = 0.0;

	for (int i = 0; i != 87; ++i)
		GrandButtonOffsets[i] = 0.f;
}

void grandPianoPress(int i)
{
	if (i > 86)
		return;
	GrandPianoButtonsPress[i] = true;
}

void grandPianoRelease(int i)
{
	if (i > 86)
		return;
	GrandPianoButtonsPress[i] = false;
	GrandPianoMalletReleased[i] = true;
}

void grandAnimation()
{
	for (unsigned i = 0; i != 87; ++i)
	{
		if (GrandPianoButtonsPress[i])
		{
			if (GrandButtonAnimationTime[i] < GrandButtonAnimationTimeEnd)
				GrandButtonAnimationTime[i] += glfwGetTime();

			if (GrandButtonAnimationTime[i] > GrandButtonAnimationTimeEnd)
				GrandButtonAnimationTime[i] = GrandButtonAnimationTimeEnd;
		}
		else
		{
			if (GrandButtonAnimationTime[i] > 0)
				GrandButtonAnimationTime[i] -= glfwGetTime();
			
			if (GrandButtonAnimationTime[i] < 0) GrandButtonAnimationTime[i] = 0;
		}
		GrandButtonOffsets[i] = static_cast<float>(GrandButtonAnimationTime[i] / GrandButtonAnimationTimeEnd) * GrandPianoButtonDepthEnd;
	}
}