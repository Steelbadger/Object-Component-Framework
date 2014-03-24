/*
	file:	XASound.hpp
	Version:	2.1
	Date:	23rd April 2013; September 2012.
	Authors:	Stuart & Allan Milne.

	Namespace:	AllanMilne::Audio.
	Exposes:	XASound.
	Requires:	ISound, PCMWave.

	Description:
	This class models a single sound that encapsulates an XAudio2 source voice with an associated sound buffer.
	It exposes basic playing behaviour is defined through the ISound interface
	and extends this with XAudio2 specific behaviour.
	* See ISound.hpp for the basic behaviour.
	* note that volume units are in dB in this implementation.

	the sound is represented by an XAudio2 source voice and a single XAudio2 buffer.

	Extended XAudio2-specific bahaviour introduces 
	* access to XAudio2 source voice and buffer;
	* routing to a submix voice;
	* panning; 
	* frequency adjustment; and 
	* application of filters.

	The get/set/adjust functions for pan are defined as virtual so that a different panning implementation can be used if desired.
	mutator functions for pan, pitch and filtering are defined as virtual;
	this allows XA3DSound to override with empty functions since these attributes are not relevant if using 3D.

*/

#ifndef __XASOUND_HPP__
#define __XASOUND_HPP__

// system includes.
#include <XAudio2.h>
#include <string>
using std::string;

// framework include.
#include "ISound.hpp"

namespace AllanMilne {
namespace Audio {

// forward declaration.
	class PCMWave;

class XASound : public ISound
{
public:
	//=== Behaviour extending the ISound interface.

	//--- Overloaded constructor methods:
	//--- Provide a .wav file name.
	XASound (const string &aFileName, const bool filtered=false);
	//--- Provide an encapsulated wave buffer.
	XASound (PCMWave *aWave, const bool filtered=false);

	//--- Query if sound is valid; has a sourcevoice that can play back audio
	bool IsValid() const { return GetSourceVoice() != nullptr; }

	//--- destructor.
	virtual ~XASound ();

	//--- Access to the source voice to allow other methods to be called.
	inline IXAudio2SourceVoice* GetSourceVoice () const { return mSourceVoice; }

	//--- access to the XAudio2 buffer
	inline XAUDIO2_BUFFER& GetBuffer () { return mXABuffer; }

	//--- Route this sound through the specified submix voice.
	void RouteToSubmixVoice (IXAudio2SubmixVoice* aSubmixVoice);

	//--- Panning - is only applied if we have stereo device channels; assumes output channels = device channels.
	//--- Works best for mono sources - multiple channels will be merged.
	//--- Value is between -1 and 1;
	//--- 0 = middle; -1 = far left; 1 = far right.
	//--- setting values outside this range has no effect.
	inline virtual float GetPan () const { return mPan; }
	virtual void SetPan (const float aPan);
	virtual void AdjustPan (const float anAmount);

	//--- Pitch control; values are in semitones.
	//--- this adjusts the frequency ratio of the source voice; if you want to use the native frequency ratio then access the source voice directly.
	float GetPitch () const;
	virtual void SetPitch (const float aPitch);
	virtual void AdjustPitch (const float anAmount);

	//--- Filtering behaviour; if source voice has not been enabled with filtering then these functions have no effect.
	//--- cut-off frequency is in Hz; if <0 then set to 0.
	//--- 1/Q value is between 0 and 1.0; if outside range then set to 0 or 1 respectively.
	bool IsFiltered () const;
	float GetFilterCutoffFrequency () const;
	virtual void SetFilterCutoffFrequency (const float aFrequency);
	virtual void AdjustFilterCutoffFrequency (const float anAmount);
	float GetFilter1OverQ () const;
	virtual void SetFilter1OverQ (const float a1OverQ);
	virtual void AdjustFilter1OverQ (const float anAmount);
	XAUDIO2_FILTER_TYPE GetFilterType () const;
	virtual void SetFilterType (const XAUDIO2_FILTER_TYPE aType);
	virtual void SetFilter (const XAUDIO2_FILTER_TYPE aType, const float aFrequency, const float a1OverQ=1.0f);
	// Accessing the underlying XAudio2 voice's filter parameters struct.
	XAUDIO2_FILTER_PARAMETERS GetFilterParameters () const;
	virtual void SetFilterParameters (XAUDIO2_FILTER_PARAMETERS &aParameters);

	//=== Implementation of the ISound interface.

	//--- Play the sound with associated flags;
	//--- always plays from the start of the sound sample, even if previously paused.
	//--- No action if already playing.
	void Play (int aFlags=0);

	//--- Returns true if sound is actually playing; i.e. not stopped or paused.
	bool IsPlaying () const;

	//--- Stop the sound plaing; play position is reset back to the start of the sound sample.
	//--- If paused then sound position and pause state are reset.
	//--- No action if already stopped.
	void Stop ();

	//--- Pause the sound playing; play position is retained.
	//--- No action if sound is stopped or already paused.
	void Pause ();
	inline bool IsPaused () const { return mIsPaused; }

	//--- Resume playing from a paused state.
	//--- No action if sound is stopped or playing.
	void Unpause ();

	//--- Toggle between play and pause state.
	//--- No action if the sound is stopped.
	void TogglePause ();

	//--- Getter and setter to indicate if the sound should be played in looping mode.
	//--- Setter is only actioned if the sound is in the stopped state.
	void SetLooped (const bool aLooped);
	bool IsLooped () const;

	//--- Volume controls; actioned for all states.
	//--- Note volume units are in dB; if you want to use the XAudio2 amplitude multiplier units then access directly via the source voice.
	//--- Set and adjust methods do nothing if supplied value is out of XAudio2 allowable range.
	float GetVolume () const;
	void SetVolume (const float aVolume);
	void AdjustVolume (const float anAmount);

protected:		// Fields are protected to allow a subclass to extend this functionality.

	IXAudio2SourceVoice *mSourceVoice;
	XAUDIO2_BUFFER		mXABuffer;
	bool				mIsPaused;
	float mPan;

}; // end XASound class.

} // end Audio namespace.
} // end AllanMilne namespace.

/* Version history.
=== 2.0
Reflect version 2 of ISound.
Constructor changed to take full responsibility for creating the source voice and buffer;
- exposes overloaded constructors;
- introduces optional filter enable flag.
Volume getter/setter uses dB units rather than the amplitude multiplier.
Introduces panning, pitch, and filtering  behaviour.
*/

#endif