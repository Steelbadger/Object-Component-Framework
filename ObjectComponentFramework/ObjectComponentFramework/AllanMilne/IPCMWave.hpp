/*
	File:	IPCMWave.hpp
	Version:	2.2
	Date:	22nd April 2013; September 2012.
	Authors:	Allan & stuart Milne.

	Namespace:	AllanMilne::Audio
	Exposes:	IPCMWave.
	Requires:	WaveFmt

	Description:
	Represents A sampled sound in PCM wave format.
	The design is defined here as an interface (abstract class) with pure virtual functions

	Notes:
	1.	This interface is technology neutral;
		it does not refer to any specific implementation.
	2.	Only a virtual destructor is defined to allow correct cascade of destructors.
	3.	The abstract class reflects the design (not implementation) of the problem space;
		in design terms an abstract class and an interface are treated as the same thing.

	A PCMWave object will include a file name string, wave format information and the sample data values.
	Once instantiated only the file name and sample values can be modified;
	the format attributes and sample data buffer cannot be modified by the client.
	Accessor member functions are exposed to provide read-only access to the attributes;
	the wave format information is presented as a WaveFmt struct reference that can then provide access to the wave format fields.

	The pointer and size of the buffer containing the sample value data is accessible but cannot be modified.
	Allowing client access to the buffer pointer allows the client to amend the data values of the buffer although the client cannot change either the address of the buffer or its size.

	A PCMWave object can also be saved to a file in .wav format.

	The object also exposes a status attribute that indicates if an error was detected when instantiating the object;
	possible values for this attribute are enumerated via the public 'status' type within this class
	and a method provides a textual description of this status.

*/


#ifndef __IPCMWave_hpp__
#define __IPCMWave_hpp__

#include <string>
using std::string;

namespace AllanMilne {
namespace Audio {

// forward declaration.
struct WaveFmt;

class IPCMWave
{
public:

	//=== Enumeration defining possible status values; must be defined here as no mechanism to virtualise this type.
	enum Status {
		OK,
		Undefined,
		BufferAllocationError,
		FileOpenError,
		EndOfFile,
		RiffError,
		ChunkNotFound,
		RiffWriteError,
		NoFmtChunk,
		InvalidFmt,
		NotPcmFormat,
		InvalidFmtChunkSize,
		FmtWriteError,
		NoDataChunk,
		DataChunkError,
		DataWriteError
	};

	//--- virtual destructor to allow correct operation of cascaded destructors.
	virtual ~IPCMWave (){}

	// file name has a getter and setter.
	virtual void SetFileName (const string &aFileName) = 0;
	virtual const string& GetFileName () const = 0;

	// Only getters are exposed for other attributes.
	virtual const WaveFmt& GetWaveFormat () const = 0;
	virtual size_t GetDataSize () const = 0;
	virtual char* const GetWaveData () const = 0;
	virtual Status GetStatus() const = 0;

	// computed from data size, number of channels and bit depth.
	virtual unsigned int GetNumberOfSamples () const = 0;
	virtual unsigned int GetSampleLengthInMilliseconds () const = 0;

	// Saves the PCMWave object to a file in .wav format; 
	// returns false if unsuccessful and sets the status field.
	// If file already exists then it is overwritten.
	virtual bool SaveToFile () = 0;				// saves to mFileName.
	virtual bool SaveToFileAs (const string &aFileName) = 0;		// saves to the supplied file name; mFileName is changed.

	// return PCM wave status value as a string.
	virtual string& GetStatusDescription () const = 0;

}; // end IPCMWave interface.

} // end Audio namespace.
} // end AllanMilne namespace.

#endif