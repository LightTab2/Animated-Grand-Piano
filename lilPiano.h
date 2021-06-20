#pragma once
#include "global.h"
#include "modelParser.h"

extern glm::vec3 HammerPositions[8];

extern float HammerAngles[8];

extern LoadedModel  *ToyPianoBase,
					*ToyPianoHammer,
					*ToyPianoLid;

extern float lidAngle;
extern double lidAnimationTime;
extern double lidAnimationTimeEnd;
extern bool lidOpen;

extern LoadedModel* ToyPianoButtons[8];
extern bool ToyPianoButtonsPress[8];
extern double ButtonAnimationTime[8];
extern double ButtonAnimationTimeEnd;
extern float ToyPianoButtonsDepth[8];
extern float ToyPianoBUttonDepthEnd;

void lilPianoPress(int i = 0);

void lilPianoRelease(int i = 0);

void lilPianoAnim();