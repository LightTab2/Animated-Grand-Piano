#include "sound.h"
#include "grandPiano.h"

float SoundSystem::sineTable[SINE_RESOLUTION];
SoundSystem::SoundData SoundSystem::data;

SoundSystem::SoundSystem()
{
	float pianAmps[] = { 1.0f, 0.399064778f, 0.229404484f, 0.151836061f,
						0.196754229f, 0.093742264f, 0.060871957f,
						0.138605419f, 0.010535002f, 0.071021868f};
	
	sineTable[0] = 0.f;
	for (unsigned i = 1; i != SINE_RESOLUTION; ++i)
	{
		for (unsigned j = 0; j != 10; ++j)
			sineTable[i] += std::sin(((j+1) * static_cast<float>(i) / SINE_RESOLUTION) * 2.f * static_cast<float>(PI)) / 10.f * pianAmps[j];
		sineTable[i] *= 2.75f; //nieco glosniej
	}

	for (int i = 0; i != SN; ++i)
	{
		data.index[i] = data.note[i] = data.attackTime[i] = data.releaseTime[i] = data.disabled[i] = data.active[i] = 0;
		data.freq[i] = 0.f;
	}

	PaError err = Pa_Initialize();
	if (err) std::cout << Pa_GetErrorText(err) << std::endl;
	err = Pa_OpenDefaultStream(&stream,
		0,					/* no input channels */
		2,					/* stereo output */
		paFloat32,			/* 32 bit floating point output */
		44100,
		paFramesPerBufferUnspecified,	/* frames per buffer, i.e. the number
						   of sample frames that PortAudio will
						   request from the callback.
						   paFramesPerBufferUnspecified
						   tells PortAudio to pick the best,
						   possibly changing, buffer size.*/
		patestCallback,		/* this is your callback function */
		&data);				/*This is a pointer that will be passed to
						   your callback*/
	if (err) std::cout << Pa_GetErrorText(err) << std::endl;
	err = Pa_StartStream(stream);
	if (err) std::cout << Pa_GetErrorText(err) << std::endl;
	playMIDI("givingup.mid");
}

SoundSystem::~SoundSystem()
{
	Pa_Terminate();
}

void SoundSystem::playNote(unsigned int whichOne, int volume)
{
	bool bSilenced = false;
	for (int i = 0; i != SN; ++i)
		if (data.note[i] == whichOne)
		{
			if (data.disabled[i])
			{
				#pragma warning(suppress : 4244)
				data.attackTime[i] *= 0.66f; //Dzięki temu słychac dobrze kazde nowe nacisniecie klawisza
				data.releaseTime[i] = 0;
				data.disabled[i] = false;
				data.active[i] = true;
				data.volume[i] = volume;
				data.duration[i] = 0;
			}
			bSilenced = data.active[i];
			break;
		}

	if (!bSilenced)
		for (int i = 0; i != SN; ++i)
			if (!data.active[i])
			{
				data.active[i] = true;
				data.note[i] = whichOne;
				data.volume[i] = volume;
				data.duration[i] = 0;
				break;
			}
}

void SoundSystem::playMIDI(std::string pathToMIDIFile)
{
	smf::MidiFile midifile;
	midifile.read(pathToMIDIFile);
	midifile.doTimeAnalysis();
	midifile.linkNotePairs();
	std::cout << "TPQ: " << midifile.getTicksPerQuarterNote() << std::endl;
	int tracks = midifile.getTrackCount();
	for (int track = 0; track != tracks; ++track)
		for (int event = 0; event < midifile[track].size(); ++event)
		{
			if (midifile[track][event].isNoteOn())
			{
				data.notes.emplace_back(static_cast<int>(midifile[track][event].seconds*44100), 
					static_cast<int>((midifile[track][event].seconds+midifile[track][event].getDurationInSeconds())*44100),
					static_cast<int>(midifile[track][event][1] - 12),
					static_cast<int>(midifile[track][event][2]));
			}
		}
	data.MIDITime = 0;
	data.MIDInextCheck = 0;
	data.MIDInextDeleteCheck = 0;
	data.notes.sort();
}

void SoundSystem::stopPlayingNote(unsigned int whichOne)
{
	for (int i = 0; i != SN; ++i)
		if (data.active[i] && data.note[i] == whichOne)
		{
			data.disabled[i] = true;
			break;
		}
}

static int patestCallback(const void* inputBuffer, void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData)
{
	SoundSystem::SoundData* data = (SoundSystem::SoundData*)userData;
	float* out = (float*)outputBuffer;

	for (unsigned int i = 0; i != framesPerBuffer; ++i)
	{
		if (bRunMidi && data->notes.size())
		{
			if (data->MIDITime == data->MIDInextDeleteCheck)
			{
				data->MIDInextDeleteCheck = INT_MAX;
				for (auto it = data->notes.begin(); it != data->notes.end();)
				{
					if (it->end == data->MIDITime)
					{
						grandPianoRelease(it->note);
						SoundSystem::stopPlayingNote(it->note);
						data->notes.erase(it++);
						continue;
					}
					else if (it->end < data->MIDInextDeleteCheck)
						data->MIDInextDeleteCheck = it->end;
					else if (it->start >= data->MIDITime)
						break;
					++it;
				}
			}
			if (data->MIDITime == data->MIDInextCheck)
				for (auto it = data->notes.begin(); it != data->notes.end(); ++it)
				{
					if (it->start == data->MIDInextCheck)
					{
						grandPianoPress(it->note);
						SoundSystem::playNote(it->note, it->volume);
					}
					else if (it->start > data->MIDInextCheck)
					{
						data->MIDInextCheck = it->start;
						if (it->end < data->MIDInextDeleteCheck)
							data->MIDInextDeleteCheck = it->end;
						break;
					}
				}
			++data->MIDITime;
		}
		float output = 0.f;
		for (unsigned int n = 0; n != SN; ++n)
		{
			if (!data->active[n])
				continue;
			float freq = 1603.63636363f / SINE_RESOLUTION / pow(2.f, (data->note[n]) / 12.f);
			
			float note = 0.f;
			note = SoundSystem::sineTable[data->index[n]];
			if (++data->freq[n] >= freq)
			{
				if ((data->index[n] += static_cast<int>(data->freq[n] / freq)) >= SINE_RESOLUTION)
					data->index[n] = data->index[n] % SINE_RESOLUTION;
				data->freq[n] = fmod(data->freq[n], freq);
			}
			if (data->disabled[n])
			{
				if (data->attackTime[n] < RELEASE_TIME2)
				{
					data->releaseTime[n] = RELEASE_TIME2 - data->attackTime[n];
					data->attackTime[n] = RELEASE_TIME2;
				}
				if (data->releaseTime[n] < RELEASE_TIME2)
					++data->releaseTime[n];
				else 
				{
					data->releaseTime[n] = 0;
					//data->freq[n] = 0;
					data->disabled[n] = false;
					data->active[n] = false;
				}
				note *= static_cast<float>(RELEASE_TIME - data->releaseTime[n]) / RELEASE_TIME;
			}
			else
				if (data->attackTime[n] < RELEASE_TIME2 && ++data->attackTime[n] < ATTACK_TIME)
					note *= static_cast<float>(data->attackTime[n]) / ATTACK_TIME;
			if (data->duration[n] < FADE) ++data->duration[n];
			if (data->duration[n] >= FADEGATE)
				note *= (FADEGATE + FADEMAX - data->duration[n]) / static_cast<float>(FADEMAX);
			output += note * (data->volume[n] / 255.f);
		}
		//earrape protection
		output *= 0.35f;
		//Mozna przemnozyc przez jakas funkcje odleglosci od obserwatora i katu
		//W ten sposob bysmy slyszeli lepiej pianino czym blizej jestesmy, 
		//a dzwiek w zaleznosci jak jestesmy obroceni do niego (stereo)
		*(out++) = output;
		*(out++) = output;
	}

	//Sleep(100);
	return 0;
}