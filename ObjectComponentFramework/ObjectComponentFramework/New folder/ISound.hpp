/*
	file:	ISound.hpp
	Version:	2.0
	Date:	20th March 2013; August 2012.
	Authors:	Stuart & Allan Milne.

	Namespace:	AllanMilne::Audio.
	Exposes:	ISound.

	Description:
	this interface (pure abstract class) defines the basic playing behaviour
	that would be expected of a class modelling an audio sample.
	This defines only behaviour (with no implementation) through pure virtual functions.
	The interface is technology agnostic; it makes no assumptions regarding the representation, rendering or implementation of the sound sample.

	this defines a playable audio sample with pausing, volume and looping controls.
	A sound is ither stopped, paused or playing with the controls' behaviour being defined for each of these states.

*/

#ifndef __ISOUND_HPP__
#define __ISOUND_HPP__

namespace AllanMilne {
namespace Audio {

class ISound
{
public:

	//--- Play the sound with associated flags; default is 0.
	//--- The meaning of the flags argument will be implementation dependant.
	//--- Always plays from the start of the sound sample, even if previously paused.
	//--- No action if already playing.
	virtual void Play (int aFlags=0) = 0;

	//--- Returns true if sound is currently playing; i.e. not stopped or paused.
	virtual bool IsPlaying () const = 0;

	//--- Stop the sound plaing; play position is reset back to the start of the sound sample.
	//--- If paused then sound position and pause state are reset.
	//--- No action if already stopped.
	virtual void Stop () = 0;

	//--- Pause the sound playing; play position is retained.
	//--- No action if sound is stopped or already paused.
	virtual void Pause () = 0;

	//--- Returns true if in paused state; false if in playing or stopped state.
	virtual bool IsPaused () const = 0;

	//--- Resume playing from a paused state.
	//--- No action if sound is stopped or playing.
	virtual void Unpause () = 0;

	//--- Toggle between play and pause state.
	//--- No action if the sound is stopped.
	virtual void TogglePause () = 0;

	//--- Getter and setter to indicate if the sound should be played in looping mode.
	//--- Setter is only actioned if the sound is in the stopped state.
	virtual void SetLooped (const bool aLooped) = 0;
	virtual bool IsLooped () const = 0;

	//--- Volume controls; actioned for all states.
	//--- The volume scale and units will be defined by a specific technology implementation.
	virtual float GetVolume () const = 0;
	virtual void SetVolume (const float aVolume) = 0;
	// Adjust by an amount relative to current setting.
	virtual void AdjustVolume (const float anIncrement) = 0;

}; // end ISound interface.

/*
=== Version 2 changes.
Play flag parameter is now optional.
*/

} // end Audio namespace.
} // end AllanMilne namespace.

#endif