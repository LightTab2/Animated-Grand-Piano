#pragma once
#include "global.h"
#include "modelParser.h"

extern glm::vec3	HammerPositions[8];

extern float		HammerAngles[8];

extern LoadedModel  *ToyPianoBase,
					*ToyPianoHammer,
					*ToyPianoLid,
					*Floor;

extern double		lidAngle,
 					lidAnimationTime,
					lidAnimationTimeEnd;
extern bool			lidOpen;

extern LoadedModel	*ToyPianoButtons[8];
extern bool			ToyPianoButtonsPress[8];
extern double		ButtonAnimationTime[8],
 					ButtonAnimationTimeEnd;
extern float		ToyPianoButtonsDepth[8],
					ToyPianoBUttonDepthEnd;

void lilPianoPress(int i = 0);

void lilPianoRelease(int i = 0);

void lilPianoAnim();