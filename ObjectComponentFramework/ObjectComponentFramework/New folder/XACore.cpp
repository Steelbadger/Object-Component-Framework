/* 
	file:	XACore.cpp
	Version:	2.3
	Date:	18th February 2014; April 2013; 2012.
	Authors:	Stuart and Allan Milne.

	Namespace:	AllanMilne::Audio
	Exposes:	XACore implementation.
	
	Description:
	This is the implementation of the concrete XACore class that encapsulates the core XAudio2 and X3DAudio functionality
	as defined in the XACore definitions.
	* See XACore.hpp for class declaration;
	* see IXACore.hpp for specification of behaviour.

	Version history:
	2.3	corrected bug in Apply3D.
		Did not free memory allocated for the output matrix in DSP settings.

*/
// Include library dependencies
#pragma comment(lib, "X3DAudio.lib")

// system includes.
#include <cmath>

// framework includes.
#include "XACore.hpp"

namespace AllanMilne {
namespace Audio {

//=== static members.

//--- initialize fields to indicate instance not yet created.
XACore *XACore::mInstance = NULL;
XACore::Status XACore::mStatus = XACore::Undefined;
	
//--- Create the instance, initializing XAudio2, mastering voice and X3DAudio.
//--- Only does this if not already attempted; returns true if instance is valid, false otherwise.
bool XACore::CreateInstance ()
{
	if (mStatus == Undefined)
	{ 
		// the constructor will set the mStatus field to indicate success or failure.
		mInstance = new XACore(); 
	}
	return (mStatus == OK);
} // end CreateInstance function.

//--- Delete the instance, releasing resources as appropriate.
void XACore::DeleteInstance ()
{
	delete mInstance;
	mInstance = NULL;
	mStatus = Undefined;
} // end DeleteInstance function.


//=== Instance members.

//--- Calculate and apply 3D audio DSP settings to a voice.
//--- handles output matrix, doppler effect and filter flags.
//--- Only applies those settings compatible with the supplied flags and source voice creation flags.
void XACore::Apply3D (IXAudio2SourceVoice *aVoice, const X3DAUDIO_EMITTER* anEmitter, const X3DAUDIO_LISTENER* aListener, const unsigned int flags) const
{
	// Guard against invalid initialisation.
	if (mStatus != OK) return;

	// get relevant details from the source voice.
	XAUDIO2_VOICE_DETAILS voiceDetails;
	aVoice->GetVoiceDetails(&voiceDetails);
	// Set up DSP settings.
	X3DAUDIO_DSP_SETTINGS DSPSettings;
	SecureZeroMemory (&DSPSettings, sizeof(X3DAUDIO_DSP_SETTINGS));
	DSPSettings.SrcChannelCount = voiceDetails.InputChannels;
	DSPSettings.DstChannelCount = mChannelCount;
	if (flags & X3DAUDIO_CALCULATE_MATRIX) {
		// only allocate matrix space if calculate flag is set.
		DSPSettings.pMatrixCoefficients = new FLOAT32 [DSPSettings.SrcChannelCount * DSPSettings.DstChannelCount]; 
	}

	X3DAudioCalculate (m3DHandle, aListener, anEmitter, flags, &DSPSettings );

	// Apply the DSP settings identified by the flags to the supplied voice.
	// check that the voice has the relevant capability through its creation flags.
	if (flags & X3DAUDIO_CALCULATE_MATRIX) {
		aVoice->SetOutputMatrix ( NULL, DSPSettings.SrcChannelCount, DSPSettings.DstChannelCount, DSPSettings.pMatrixCoefficients); 
		// Free output matrix space that was allocated.
		delete [] DSPSettings.pMatrixCoefficients;
	}
	if ((flags & X3DAUDIO_CALCULATE_DOPPLER) && !(voiceDetails.CreationFlags & XAUDIO2_VOICE_NOPITCH)) {
		aVoice->SetFrequencyRatio (DSPSettings.DopplerFactor); 
	}
	if ((flags & X3DAUDIO_CALCULATE_LPF_DIRECT) && (voiceDetails.CreationFlags & XAUDIO2_VOICE_USEFILTER)) {
		XAUDIO2_FILTER_PARAMETERS FilterParameters = { 
			LowPassFilter, 
			2.0f * sinf(X3DAUDIO_PI/6.0f * DSPSettings.LPFDirectCoefficient), 
			1.0f 
		}; 
		aVoice->SetFilterParameters (&FilterParameters);
	}
} // end Apply3D function.

//--- private constructor to hide it from explicit instantiation;
//--- initializes XAudio2, X3DAudio and the listener.
//--- sets the status flag.
XACore::XACore () 
	: mXAEngine(NULL), mMasteringVoice(NULL), mChannelCount(0)
{
	HRESULT hr;		// use to catch XAudio2 function call results.
	CoInitializeEx( NULL, COINIT_MULTITHREADED );
	hr = XAudio2Create (&mXAEngine, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED (hr))
	{
		MessageBox (NULL, TEXT (" XAudio2Create() failed to create the XAudio2 engine."), TEXT ("XACore constructor() - Failed"), MB_OK | MB_ICONERROR );
		CoUninitialize();
		mXAEngine = NULL;
		mStatus = EngineError;
		return;
	}
	mXAEngine->StartEngine();
	// Retrieve information about the audio device.
	// Note the Windows 8 SdK distribution of XAudio2 requires an alternative approach to retrieving this information.
	mXAEngine->GetDeviceDetails (0, &mDetails); 
	mChannelCount = mDetails.OutputFormat.Format.nChannels;

	hr = mXAEngine->CreateMasteringVoice (&mMasteringVoice);
	if (FAILED (hr))
	{
		MessageBox (NULL, TEXT (" CreateMasteringVoice() failed to create the voice."), TEXT ("XACore constructor() - Failed"), MB_OK | MB_ICONERROR );
		CoUninitialize();
		mXAEngine->Release();
		mXAEngine = NULL;
		mMasteringVoice = NULL;
		mStatus = MasterError;
		return;
	}

	// Set up X3DAudio.
	DWORD channelMask;
	channelMask = mDetails.OutputFormat.dwChannelMask; 
	X3DAudioInitialize (channelMask, X3DAUDIO_SPEED_OF_SOUND, m3DHandle);

	mStatus = OK;
} // end XACore constructor method.

//--- destructor method.
//--- Note only the engine is a COM object and therefore should be released; other elements are under control of the engine.
XACore::~XACore ()
{
	if (mXAEngine != NULL)
		mXAEngine->StopEngine ();
	if (mMasteringVoice != NULL)
		mMasteringVoice->DestroyVoice ();
	if (mXAEngine != NULL) 
		mXAEngine->Release();
	CoUninitialize();
	mInstance = NULL;
	mStatus = Undefined;
} // end destructor method.

} // end Audio namespace.
} // end AllanMilne namespace.