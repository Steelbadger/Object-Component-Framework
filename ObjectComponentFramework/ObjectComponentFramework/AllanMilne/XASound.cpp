/*
	file:	XASound.cpp
	Version:	2.0
	Date:	22nd April 2013; September 2012.
	Authors:	Stuart & Allan Milne.

	Namespace:	AllanMilne::Audio.
	Exposes:	XASound implementation.
	Requires:	XACore, WaveFileManager, PCMWave.

	Description:
	This is the implementation of the XASound class that models an XAudio2 sound encapsulating a source voice and associated sound buffer.
	It exposes playing behaviour is defined through the ISound interface
	and extends this with XAudio2 specific behaviour.
	* SEE XASound.hpp for details.

*/

// system includes.
#define XAUDIO2_HELPER_FUNCTIONS
#include <XAudio2.h>
#include <string>
using std::string;

// framework includes.
#include "XACore.hpp"
#include "WaveFileManager.hpp"
#include "PCMWave.hpp"
#include "XASound.hpp"

//=== anonymous namespace to encapsulate private helper functions.
namespace {
	using namespace AllanMilne::Audio;
	
	//--- Initialise source voice and buffer from a PCMWave object; called from constructors.
	void InitXASound (PCMWave * aWave, bool filtered, IXAudio2SourceVoice **aVoice, XAUDIO2_BUFFER *aBuffer)
	{
		// guard against invalid wave format.
		if (aWave->GetStatus() != PCMWave::OK)  return;

		// copy windows wave format struct from the PCMWave struct field.
		WAVEFORMATEX wFmt;
		memcpy_s (&wFmt, sizeof (WaveFmt), &(aWave->GetWaveFormat()), sizeof (WaveFmt));
		// Create the source voice for the specified wave format; return if failed.
		// Source voice will be routed directly to the mastering voice since no target voice is specified.
		unsigned int flag = (filtered) ? XAUDIO2_VOICE_USEFILTER : 0;
		HRESULT hr = XACore::GetInstance()->GetEngine()->CreateSourceVoice (aVoice, &wFmt, flag );
		if( FAILED( hr ) ) return;

		// Create and initialise the XAudio2 buffer struct from the PCMWave object.
		ZeroMemory (aBuffer, sizeof(XAUDIO2_BUFFER));
		aBuffer->AudioBytes = aWave->GetDataSize ();
		aBuffer->pAudioData = (BYTE*)(aWave->GetWaveData ());

		/*--- Following listed here for reference - have already been set to 0 via ZeroMemory above.
		aBuffer->Flags = 0;			// almost always 0; =XAUDIO2_END_OF_STREAM to suppress some debug warnings re buffer starvation.
		aBuffer->PlayBegin = 0;		// First sample in the buffer that should be played.
		aBuffer->PlayLength = 0;		// Number of samples to play; 0=entire buffer (begin must also be 0).
		aBuffer->LoopBegin = 0;		// First sample to be looped; must be <(PlayBegin+PlayLength); can be <PlayBegin.
		aBuffer->LoopLength = 0;		// Number of samples in loop; =0 indicates entire sample; PlayBegin > (LoopBegin+LoopLength) < PlayBegin+PlayLength).
		aBuffer->LoopCount = 0;		// Number of times to loop; =XAUDIO2_LOOP_INFINITE to loop forever; if 0 then LoopBegin and LoopLength must be 0.
		aBuffer->pContext = NULL;		// context to be passed to the client in callbacks.
		---*/
	} // end InitXASound function.


	//--- Apply pan value to supplied source voice.
	//--- Assumes stereo output channels.
	//--- Centred sound has output volumes at { 0.5, 0.5 }.
	void DoPan (const float aPan, IXAudio2SourceVoice *aVoice)
	{
		XAUDIO2_VOICE_DETAILS details;
		aVoice->GetVoiceDetails(&details);
		int matrixSize = details.InputChannels * 2;			// assuming stereo output channels.
		float *matrix = new float[matrixSize];
		float halfPan = aPan / 2.0f;
		for (int i=0; i<matrixSize; i+=2)
			matrix[i] = 0.5f - halfPan;
		for (int i=1; i<matrixSize; i+=2)
			matrix[i] = 0.5f + halfPan;
		// 1st param = NULL indicates single destination voice.
		aVoice->SetOutputMatrix(NULL, details.InputChannels, 2, matrix);
		delete [] matrix;
	} // end doPan function.

} // end anonymous namespace.


namespace AllanMilne {
namespace Audio {

//=== Behaviour extending the ISound interface.

//--- Overloaded constructor methods:
//--- Provide a .wav file name.
XASound::XASound (const string &aFileName, const bool filtered)
	: mSourceVoice(NULL), mIsPaused(false), mPan(0)
{
	PCMWave *waveBuffer = WaveFileManager::GetInstance().LoadWave (aFileName);
	InitXASound (waveBuffer, filtered, &mSourceVoice, &mXABuffer);
} // end constructor with file name.
//--- Provide an encapsulated wave buffer.
XASound::XASound (PCMWave *aWave, const bool filtered)
	: mSourceVoice(NULL), mIsPaused(false), mPan(0)
{
	InitXASound (aWave, filtered, &mSourceVoice, &mXABuffer);
} // end constructor with PCMWave.
//--- end overloaded constructor functions.

//--- Destructor function.
//--- Note we must not delete mSourceVoice here; a source voice is owned by the XAudio2 engine and will be deleted by it.
//--- Do not delete data buffer referenced by mXABuffer since we were not responsible for creating it.
XASound::~XASound ()
{
	if (mSourceVoice != NULL) {
		mSourceVoice->Stop();
		mSourceVoice->FlushSourceBuffers();
		mSourceVoice->DestroyVoice();
	}
} // end destructor function.

//--- Route this sound through the specified submix voice.
void XASound::RouteToSubmixVoice (IXAudio2SubmixVoice* aSubmixVoice) 
{
	// Guard against null submix or source voices.
	if (aSubmixVoice==NULL || mSourceVoice==NULL) { return; }

	XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0, aSubmixVoice };
	XAUDIO2_VOICE_SENDS sendList = { 1, &sendDesc };
	mSourceVoice->SetOutputVoices (&sendList);
} // end RouteToSubmixVoice function.

//--- Panning - is only applied if we have stereo device channels; assumes output channels = device channels.
//--- Works best for mono sources - multiple channels will be merged.
//--- Value is between -1 and 1;
//--- 0 = middle; -1 = far left; 1 = far right.
//--- setting values outside this range has no effect.

void XASound::SetPan (const float aPan)
{
	// Only applied if stereo channels and valid pan value.
	if (XACore::GetInstance()->GetChannelCount() != 2) return;
	if (aPan<-1.0f || aPan>1.0f) return;

	mPan = aPan;
	DoPan (mPan, mSourceVoice);
} // end SetPan function.

void XASound::AdjustPan (const float anAmount)
{
	// Only applied if stereo channels.
	if (XACore::GetInstance()->GetChannelCount() != 2) return;

	// only adjust pan to limits if amount is too great.
	mPan += anAmount;
	if (mPan < -1.0f) mPan = -1.0f;
	if (mPan >1.0f) mPan = 1.0f;
	DoPan (mPan, mSourceVoice);
} // end AdjustPan function.


//--- Pitch control; values are in semitones.
//--- this adjusts the frequency ratio of the source voice; if you want to use the native frequency ratio then access the source voice directly.

float XASound::GetPitch () const
{
	float freqRatio;
	mSourceVoice->GetFrequencyRatio (&freqRatio);
	return XAudio2FrequencyRatioToSemitones (freqRatio);
} // end GetPitch function.

void XASound::SetPitch (const float aPitch)
{
	float freqRatio = XAudio2SemitonesToFrequencyRatio (aPitch);
	mSourceVoice->SetFrequencyRatio (freqRatio);
} // end SetPitch function.

void XASound::AdjustPitch (const float anAmount)
{
	float semi = GetPitch();
	semi += anAmount;
	SetPitch (semi);
} // end AdjustPitch function.


//--- Filtering behaviour; if source voice has not been enabled with filtering then these functions have no effect.

bool XASound::IsFiltered () const
{
	XAUDIO2_VOICE_DETAILS details;
	mSourceVoice->GetVoiceDetails (&details);
	return ((details.CreationFlags & XAUDIO2_VOICE_USEFILTER) != 0);
} // end IsFiltered function.

//--- Returns cut-off frequency in Hz.
float XASound::GetFilterCutoffFrequency () const
{
	if (!IsFiltered()) return 0.0f;
	XAUDIO2_FILTER_PARAMETERS params;
	mSourceVoice->GetFilterParameters (&params);
	XAUDIO2_VOICE_DETAILS details;
	mSourceVoice->GetVoiceDetails (&details);
	return XAudio2RadiansToCutoffFrequency (params.Frequency, static_cast<float>(details.InputSampleRate));
} // end GetFilterCutoffFrequency function.

//--- Sets cut-off frequency in Hz; if <0 then sets to 0Hz.
void XASound::SetFilterCutoffFrequency (const float aFrequency)
{
	if (!IsFiltered()) return;
	XAUDIO2_FILTER_PARAMETERS params;
	mSourceVoice->GetFilterParameters (&params);
	XAUDIO2_VOICE_DETAILS details;
	mSourceVoice->GetVoiceDetails (&details);
	float freq = (aFrequency<0) ? 0.0f : XAudio2CutoffFrequencyToRadians (aFrequency, details.InputSampleRate);
	params.Frequency = freq;
	mSourceVoice->SetFilterParameters (&params);
	return;
} // end SetFilterCutoffFrequency function.

//--- Adjust cutoff frequency in Hz; will not go below 0.
void XASound::AdjustFilterCutoffFrequency (const float anAmount)
{
	float cutoff = GetFilterCutoffFrequency() + anAmount;
	if (cutoff < 0.0f) cutoff = 0.0f;
	SetFilterCutoffFrequency(cutoff);
} // end AdjustFilterCutoffFrequency function.

//--- In range 0 to 1.
float XASound::GetFilter1OverQ () const
{
	XAUDIO2_FILTER_PARAMETERS params;
	mSourceVoice->GetFilterParameters (&params);
	return params.OneOverQ;
} // end GetFilter1OverQ function.

//--- if < 0 set to 0; if >1 set to 1.
void XASound::SetFilter1OverQ (const float a1OverQ)
{
	float q = (a1OverQ<0.0f) ? 0.0f : a1OverQ;
	if (q > 1.0f) q = 1.0f;
	XAUDIO2_FILTER_PARAMETERS params;
	mSourceVoice->GetFilterParameters (&params);
	params.OneOverQ = q;
	mSourceVoice->SetFilterParameters (&params);
} // end SetFilter1OverQ function.

//--- Range as for above.
void XASound::AdjustFilter1OverQ (const float anAmount)
{
	float q = GetFilter1OverQ () + anAmount;
	SetFilter1OverQ (q);
} // end AdjustFilter1OverQ function.

XAUDIO2_FILTER_TYPE XASound::GetFilterType () const
{
	XAUDIO2_FILTER_PARAMETERS params;
	mSourceVoice->GetFilterParameters (&params);
	return params.Type;
} // end GetFiltertype function.

void XASound::SetFilterType (const XAUDIO2_FILTER_TYPE aType)
{
	XAUDIO2_FILTER_PARAMETERS params;
	mSourceVoice->GetFilterParameters (&params);
	params.Type = aType;
	mSourceVoice->SetFilterParameters (&params);
} // end SetFilterType function.

//--- Set all filter parameters explicitly; frequency is in Hz; limits as above.
void XASound::SetFilter (const XAUDIO2_FILTER_TYPE aType, const float aFrequency, const float a1OverQ)
{
	if (!IsFiltered()) return;
	XAUDIO2_VOICE_DETAILS details;
	mSourceVoice->GetVoiceDetails (&details);
	XAUDIO2_FILTER_PARAMETERS params;
	params.Frequency = (aFrequency<0.0f) ? 0.0f : XAudio2CutoffFrequencyToRadians (aFrequency, details.InputSampleRate);
	params.Type = aType;
	float q = (a1OverQ<0.0f) ? 0.0f : a1OverQ;
	if (q > 1.0f) q = 1.0f;
	params.OneOverQ = q;
	mSourceVoice->SetFilterParameters (&params);
} // end SetFilter function.

//--- returns the underlying XAudio2 filter struct.
XAUDIO2_FILTER_PARAMETERS XASound::GetFilterParameters () const
{
	XAUDIO2_FILTER_PARAMETERS params;
	ZeroMemory (&params, sizeof(XAUDIO2_FILTER_PARAMETERS));
	if (!IsFiltered()) return params;
	mSourceVoice->GetFilterParameters(&params);
	return params;
} // end GetFilterParameters function.

//--- Set using an XAudio2 filter parameters struct.
void XASound::SetFilterParameters (XAUDIO2_FILTER_PARAMETERS &aParameters)
{
	if (!IsFiltered()) return;
	mSourceVoice->SetFilterParameters (&aParameters);
} // end SetFilterParameters function.


//=== Implementing the ISound interface.

//--- Play the sound with associated flags;
//--- always plays from the start of the sound sample, even if previously paused.
//--- No action if already playing.
void XASound::Play (int aFlags)
{
	if (IsPlaying()) { return; }
	if (mIsPaused)
	{	// reset buffer.
		mSourceVoice->FlushSourceBuffers();
		mIsPaused = false;
	}
	mSourceVoice->SubmitSourceBuffer (&mXABuffer);
	mSourceVoice->Start (aFlags, XAUDIO2_COMMIT_NOW);
} // end Play method.

//--- Stop the sound plaing; play position is reset back to the start of the sound sample.
//--- If paused then sound position and pause state are reset.
//--- No action if already stopped.
void XASound::Stop ()
{
	if (IsPlaying() || mIsPaused)
	{
		mSourceVoice->Stop();
		mSourceVoice->FlushSourceBuffers();
		mIsPaused = false;
	}
} // end Stop method.

//--- Pause the sound playing; play position is retained.
//--- No action if sound is stopped or already paused.
void XASound::Pause ()
{
	if(IsPlaying())
	{
		mSourceVoice->Stop();
		mIsPaused = true;
	}
} // end Pause method.

//--- Resume playing from a paused state.
//--- No action if sound is stopped or playing.
void XASound::Unpause ()
{
	if (mIsPaused)
	{
		mSourceVoice->Start(0);
		mIsPaused = false;
	}
} // end Unpause method.

//--- Toggle between play and pause state.
//--- No action if the sound is stopped.
void XASound::TogglePause ()
{
	(mIsPaused) ? Unpause() : Pause();
} // end TogglePause method.

//--- Returns true if sound is actually playing; i.e. not stopped or paused.
bool XASound::IsPlaying () const
{
	if (mIsPaused) { return false; }
	XAUDIO2_VOICE_STATE vState;
	mSourceVoice->GetState (&vState);
	return ( vState.BuffersQueued > 0 );
} // end IsPlaying method.

//--- Volume controls; actioned for all states.
//--- Note volume units are in dB; if you want to use the XAudio2 amplitude multiplier units then access directly via the source voice.
//--- Set and adjust methods do nothing if supplied value is out of XAudio2 allowable range.

float XASound::GetVolume () const
{
	float volume;
	mSourceVoice->GetVolume (&volume);
	return XAudio2AmplitudeRatioToDecibels (volume);
} // end GetVolume method.

//--- Does nothing if supplied value is out of XAudio2 allowable range.
void XASound::SetVolume (const float aVolume)
{
	float ampRatio = XAudio2DecibelsToAmplitudeRatio (aVolume);
	// Guard against to large or small values.
	if (ampRatio<-XAUDIO2_MAX_VOLUME_LEVEL || ampRatio>XAUDIO2_MAX_VOLUME_LEVEL) { return; }
	mSourceVoice->SetVolume (ampRatio);
} // end SetVolume method.

//--- Does nothing if supplied value is out of XAudio2 allowable range.
//--- Uses the XASound get/set functions to work in decibels here and use the volume level guard in SetVolume.
void XASound::AdjustVolume (const float anAmount)
{
	float volume = GetVolume ();
	volume += anAmount;
	SetVolume (volume);
} // end AdjustVolume method.

//--- Getter to indicate if the sound isbe played in looping mode.
bool XASound::IsLooped() const
{
	return (mXABuffer.LoopCount > 0);
} // end IsLooped method.

//--- Setter is only actioned if the sound is in the stopped state.
void XASound::SetLooped (const bool aLooped)
{
	// Guard against playing or paused.
	if (IsPlaying() || mIsPaused) { return; }
	if (aLooped) {
		mXABuffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else {
		mXABuffer.LoopCount = 0;
	}
} // end SetLooped method.

} // end audio namespace.
} // end AllanMilne namespace.