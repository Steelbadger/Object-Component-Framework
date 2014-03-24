/*
	file:	XACore.hpp
	Version:	2.3
	Date:	23rd April 2013; 2012.
	Authors:	Stuart and Allan Milne.

	Namespace:	AllanMilne::Audio
	Exposes:	XACore
	Requires:	IXACore.

	Description:
	this is the concrete XACore class that encapsulates the core XAudio2 functionality
	as defined in the IXACore interface;
	* see IXACore.hpp for details.
	this introduces the static members required for the singleton pattern.
	
*/

#ifndef __XACORE_HPP__
#define __XACORE_HPP__

// system includes.
#include <XAudio2.h>
#include <X3DAudio.h>

// framework includes
#include "IXACore.hpp"

namespace AllanMilne {
namespace Audio {

class XACore : public IXACore
{
public:

	//--- return the pointer to the single instance , NULL if not created.
	static inline IXACore* GetInstance() { return mInstance; }

	//--- Create the instance, initializing XAudio2, mastering voice and X3DAudio.
	//--- Only does this if not already attempted; returns true if instance is valid, false otherwise.
	static bool CreateInstance ();

	//--- Delete the instance, releasing resources as appropriate.
	static void DeleteInstance ();

	//--- Return current status.
	static inline Status GetStatus () { return mStatus; }


	//--- instance members.

	//--- Access to the XAudio2 engine, mastering voice and X3DAudio handle; return NULL if errors.
	inline IXAudio2 *GetEngine () const { return mXAEngine; }
	inline IXAudio2MasteringVoice *GetMasterVoice () const { return mMasteringVoice; }
	inline const X3DAUDIO_HANDLE& Get3DHandle () const { return m3DHandle; }

	//--- Accessors for other global/device attributes.
	inline int GetChannelCount () const { return mChannelCount; }
	inline const XAUDIO2_DEVICE_DETAILS& GetDeviceDetails () { return mDetails; }

	//--- Calculate and apply 3D audio DSP settings to a voice.
	//--- Default is to calculate for output matrix only
	void Apply3D (IXAudio2SourceVoice* aVoice, const X3DAUDIO_EMITTER* anEmitter, const X3DAUDIO_LISTENER* aListener, const unsigned int flags=X3DAUDIO_CALCULATE_MATRIX) const;


private:

	//--- The static instance reference and status indicator.
	static XACore *mInstance;
	static Status mStatus;

	//--- private constructor to hide it from explicit instantiation;
	//--- initializes XAudio2, X3DAudio and the listener.
	XACore ();
	virtual ~XACore ();

	//--- The fundamental audio elements.
	IXAudio2 *mXAEngine;
	IXAudio2MasteringVoice *mMasteringVoice;
	X3DAUDIO_HANDLE m3DHandle;

	//--- Referenced attributes provided here for efficiency.
	int mChannelCount;			// No. of channels on audio device
	XAUDIO2_DEVICE_DETAILS mDetails;	// Audio device information

}; // end XACore interface.

} // end Audio namespace.
} // end AllanMilne namespace.

#endif