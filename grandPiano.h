#pragma once
#include "global.h"
#include "modelParser.h"

extern LoadedModel	*GrandPianoBase,
					*GrandPianoStrings,
					*GrandPianoLid,
					*GrandPianoHammer[22],
					*leftKey, 
					*rightKey,
					*middleKey,
					*blackKey;

extern const std::string GrandPianoHammerFiles[17];

extern int GrandPianoHammerFrame[17];

extern float		GrandHammerTimes[87];

extern bool			GrandPianoButtonsPress[87],
					GrandPianoMalletReleased[87];

extern double		GrandButtonAnimationTime[87],
					GrandButtonAnimationTimeEnd;

extern float		GrandPianoButtonDepthEnd,
					GrandButtonOffsets[87];


void grandPianoInit();

void grandPianoPress(int i);
void grandPianoRelease(int i);

void grandAnimation();