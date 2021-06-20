#pragma once
#include "global.h"
#include "portaudio.h"
#include "MidiFile.h"
#include <list>
#include <algorithm>

#define SINE_RESOLUTION 401
#define ATTACK_TIME 1200
#define RELEASE_TIME 14000
#define RELEASE_TIME2 10000
#define FADEGATE 2500
#define FADE (20000 + FADEGATE)
#define FADEMAX (30000 + FADEGATE)
#define SN 20 //Skrót od Simultaneous Notes, czyli ile nut na raz można grac jednoczesnie

static int patestCallback(const void* inputBuffer, void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData);

class SoundSystem
{
	friend static int patestCallback(const void* inputBuffer, void* outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* userData);
public:
	SoundSystem();
	~SoundSystem();
	static void playNote(unsigned int whichOne, int volume = 255);
	void playMIDI(std::string pathToMIDIFile);
	static void stopPlayingNote(unsigned int whichOne);
	enum Note
	{
		A1,
		Ais1,
		B1,
		C1,
		Cis1,
		D1,
		Dis1,
		E1,
		F1,
		Fis1,
		G1,
		Gis1,
		A2,
		Ais2,
		B2,
		C2,
		Cis2,
		D2,
		Dis2,
		E2,
		F2,
		Fis2,
		G2,
		Gis2,
		A3,
		Ais3,
		B3,
		C3,
		Cis3,
		D3,
		Dis3,
		E3,
		F3,
		Fis3,
		G3,
		Gis3,
		A4,
		Ais4,
		B4,
		C4,
		Cis4,
		D4,
		Dis4,
		E4,
		F4,
		Fis4,
		G4,
		Gis4,
		A5,
		Ais5,
		B5,
		C5,
		Cis5,
		D5,
		Dis5,
		E5,
		F5,
		Fis5,
		G5,
		Gis5,
		A6,
		Ais6,
		B6,
		C6,
		Cis6,
		D6,
		Dis6,
		E6,
		F6,
		Fis6,
		G6,
		Gis6,
		A7,
		Ais7,
		B7,
		C7,
		Cis7,
		D7,
		Dis7,
		E7,
		F7,
		Fis7,
		G7,
		Gis7,
		A8,
		Ais8,
		B8,
		C8,
		Cis8,
		D8,
		Dis8,
		E8,
		F8,
		Fis8,
		G8,
		Gis8
	};
private:
	struct MIDINote
	{
		MIDINote(int start, int end, int note, int volume) : start(start), end(end), note(note), volume(volume) {};
		int start, end, note, volume;
		bool operator<(const MIDINote& rhs) const noexcept
		{
			if (this->start == rhs.start)
				return this->end < rhs.end;
			return this->start < rhs.start;
		}
	};
	static float sineTable[SINE_RESOLUTION];
	static float sinMin;
	PaStream* stream;

	static struct SoundData
	{
		int index[SN];
		int note[SN];
		int volume[SN];
		int attackTime[SN];
		int releaseTime[SN];
		int duration[SN];
		int MIDITime;
		int MIDInextCheck;
		int MIDInextDeleteCheck;
		float freq[SN];
		bool active[SN];
		bool disabled[SN];
		std::list<MIDINote> notes;
	} data;
};