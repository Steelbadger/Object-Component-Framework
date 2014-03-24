/*
	file:	IXACore.hpp
	Version:	2.2
	Date:	June 2013; April 2013; September 2012.
	Authors:	Stuart and Allan Milne.

	Namespace:	AllanMilne::Audio
	Exposes:	IXACore.
	
	Description:
	this is a design for the XACore class that will encapsulate core XAudio2 functionality
	related to the XAudio2 processing engine, mastering voice and X3DAudio.

	The design is presented as an interface (abstract class - pure virtual functions) defining only behaviour.
	* See the XACore class for implementation details.

	this version 
	* utilises a singleton design pattern;
	* creates the XAudio2 engin, mastering voice and X3DAudio handle;
	* exposes status information;
	* exposes relevant get and set behaviour;
	* exposes a function for calculating and applying 3D audio DSP settings to a voice.

	Note that the static members implementing the singleton pattern cannot be defined in this abstract class;
	these are defined in the concrete XACore class.

*/

#ifndef __IXACORE_HPP__
#define __IXACORE_HPP__

// system includes.
#include <XAudio2.h>
#include <X3DAudio.h>

namespace AllanMilne {
namespace Audio {

class IXACore
{
public:

	//--- The status enumeration type.
	enum Status {
		Undefined, OK, EngineError, MasterError
	};

	//--- Virtual destructor allows correct operation of cascaded destructors.
	virtual ~IXACore(){}

	//--- Access to the XAudio2 engine, mastering voice and 3D handle; return NULL if errors.
	virtual IXAudio2* GetEngine () const = 0;
	virtual IXAudio2MasteringVoice* GetMasterVoice () const = 0;
	virtual const X3DAUDIO_HANDLE& Get3DHandle () const = 0;

	//--- Accessors for other global/device attributes.
	virtual int GetChannelCount () const =0;
	virtual const XAUDIO2_DEVICE_DETAILS& GetDeviceDetails () = 0;

	virtual void Apply3D (IXAudio2SourceVoice* aVoice, const X3DAUDIO_EMITTER* anEmitter, const X3DAUDIO_LISTENER* aListener, const unsigned int flags) const = 0;

}; // end IXACore interface.

} // end Audio namespace.
} // end AllanMilne namespace.

/*
=== Version 2 Changes.
Adopts a singleton design pattern.
Protected fields are changed to private.
Adds a virtual destructor.
Removes the factory method for creating a XASound object.
Exposes explicit status information.
incorporates X3DAudio functionality:
- X3DAudio handle;
- wrapping a listener;
- exposing a function for calculating and applying 3D audio DSP settings.

---- 11/6/13 changes
Removed listener entries from interface, it is now client responsibility to manage listener
and supply XAudio2 listener type to Apply3D methods exposed by core.
*/

#endif