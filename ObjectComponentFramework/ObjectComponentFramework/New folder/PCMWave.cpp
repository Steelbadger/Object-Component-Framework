/*
	File:	PCMWave.cpp
	Version:	2.2.1
	Date:	5th February 2014; March 2013; September 2012.
	Author:	Allan c. Milne.

	Namespace:	AllanMilne::Audio
	Exposes:	PCMWave.
	Requires:	WaveFmt.

	Description:
	Implementation of the PCMWave class / IPCMWave interface that represents a sampled sound in PCM wave format.
	See PCMWave.hpp for further information on the design.

*/

#include <fstream>
#include <string>
using namespace std;

#include "PCMWave.hpp"
#include "WaveFmt.hpp"

namespace AllanMilne {
namespace Audio {

//=== Constructor methods.

// explicit definition of all attributes and sample value data.
// Sample value data will be copied to a new buffer.
PCMWave::PCMWave (const string &aFilename, unsigned short aChannels, unsigned short aBitDepth, unsigned long aSampleRate, size_t aSize, char *aData)
{
	mFileName = aFilename;
	mWaveFormat.wFormatTag = 1;		// WAVE_FORMAT_PCM
	mWaveFormat.nChannels = aChannels;
	mWaveFormat.nSamplesPerSec = aSampleRate;
	mWaveFormat.wBitsPerSample = aBitDepth;
	mWaveFormat.nBlockAlign = aChannels * ((aBitDepth + 7) / 8);
	mWaveFormat.nAvgBytesPerSec = aSampleRate * mWaveFormat.nBlockAlign;
	mSize = aSize;
	mWaveData = new char [mSize];
	if (mWaveData != NULL) {
		// NB if data == NULL this will cause a parameter exception.
		memcpy_s (mWaveData, mSize, aData, mSize);
		mStatus = OK;
	} else {
		// error in allocating buffer so make size 0 to be consistent.
		mSize = 0;
		mStatus = BufferAllocationError;
	}
} // end explicit attribute constructor.

// loading all attributes and sample value data from a specified .wav file.
PCMWave::PCMWave (const string &aFilename)
{
	mFileName = aFilename;
	memset ((void*)&mWaveFormat, 0, sizeof (WaveFmt));
	mSize = 0;
	mWaveData = NULL;
	mStatus = Undefined;

	ifstream infile (aFilename, ios::binary);
	if (!infile.good()) {
		mStatus = FileOpenError;
		return;		// error opening file.
	}
	ReadWaveFile (&infile);
	infile.close();
} // end file load constructor.

// Destructor - releases sample data buffer.
PCMWave::~PCMWave() 
{
	delete [] mWaveData;
	mWaveData = NULL;
} // end destructor.


//=== Accessor (getter) member functions.

unsigned int PCMWave::GetNumberOfSamples () const
{
	if (mSize == 0) {
		return 0;
	} else {
		return mSize / mWaveFormat.nBlockAlign * mWaveFormat.nChannels;
	}
} // end GetNumberOfSamples function.

unsigned int PCMWave::GetSampleLengthInMilliseconds () const 
{
	if (mSize == 0) {
		return 0;
	} else {
		return (1000 * mSize) / (mWaveFormat.nBlockAlign * mWaveFormat.nSamplesPerSec);
	}
} // end GetSampleLengthInMilliseconds  function.

//=== Saves the PCMWave object to a file in .wav format; returns true if write operation successful.
//=== If the file already exists then it is overwritten.

// use mFileName as the file name.
bool PCMWave::SaveToFile () 
{
	ofstream outfile (mFileName, ios::binary);
	if (!outfile.good()) {
		mStatus = FileOpenError;
		return false;
	}
	WriteWaveFile (&outfile);
	outfile.close();
	if (mStatus == OK) {
		return true;		// write operation has worked.
	} else {
		return false;		// error occurred writing to the file.
	}
} // end SaveToFile function.

// saves to the supplied file name; mFileName is changed.
bool PCMWave::SaveToFileAs (const string &aFilename) {
	mFileName = aFilename;
	return SaveToFile ();
} // end SaveToFileAs function.

//=== Returns status as a descriptive string.
string& PCMWave::GetStatusDescription () const 
{
	static string descriptions[] = {
	"OK",
	"Undefined",
	"Buffer allocation (malloc) error",
	"File open error",
	"End of file",
	"Not a RIFF/WAVE file",
	"Chunk not found",
	"Error writing RIFF header",
	"fmt chunk not found",
	"Invalid fmt chunk",
	"Not in PCM format",
	"Invalid fmt chunk size (should be 16 bytes)",
	"Error writing fmt chunk",
	"No data chunk",
	"Error reading data chunk",
	"Error writing data chunk"
};

	return descriptions[mStatus];
} // end GetStatusDescription function.


//=== .wav file I/O helper functions.

// read from a .wav file and set up class fields; mStatus indicates success/failure.
void PCMWave::ReadWaveFile (ifstream *anInfile)
{
	unsigned long riffTag = 'FFIR';		// compress four chars into 4 bytes.
	unsigned long waveTag = 'EVAW';
	unsigned long fmtTag = ' tmf';
	unsigned long dataTag = 'atad';
	anInfile->seekg (0);		// rewind to start of file.
	// Format should be four character RIFF tag, 4 byte chunk size and 4 character WAVE tag.
	// character tags are packed into a long for ease of checking.
	unsigned long riff[3];
	anInfile->read ((char*)riff, 12);
	if (!anInfile->good()) {
		mStatus = EndOfFile;
		return;
	}
	if (riff[0]!=riffTag || riff[2]!=waveTag) {
		mStatus = RiffError;
		return;
	}
	if (!FindChunk (anInfile, fmtTag)) {
		mStatus = NoFmtChunk;
		return;
	}
	// now positioned after the 'fmt ' tag so read and check chunk size.
	unsigned long chunkSize;
	anInfile->read ((char*)(&chunkSize), 4);
	// allow for extended format chunk sizes but must be at least 16 bytes.
	if (chunkSize < 16) {
		mStatus = InvalidFmtChunkSize;
		return;
	}
	// read in format fields into WaveFmt struct.
	anInfile->read ((char*)&mWaveFormat, sizeof(WaveFmt));
	if (!anInfile->good()) {
		memset ((void*)&mWaveFormat, 0, sizeof (WaveFmt));
		mStatus = InvalidFmt;
		return;
	}
	// now read data chunk; note that reading in data chunk allocates the space for the data buffer.
	if (!FindChunk (anInfile, dataTag)) {
		mStatus = NoDataChunk;
		return;
	}
	// now positioned after the 'data' tag so read chunk size.
	anInfile->read((char*)(&chunkSize), 4);
	mWaveData = new char [chunkSize];
	if (mWaveData == NULL) {
		mStatus = BufferAllocationError;
		return;
	}
	anInfile->read (mWaveData, chunkSize);
	if (!anInfile->good()) {
		delete [] mWaveData;		// free allocated buffer.
		mWaveData = NULL; 
		mSize = 0;
		mStatus = DataChunkError;
		return;
	}
	mSize = (size_t)chunkSize;
	mStatus = OK;		// data chunk read without error. 
} // end ReadWaveFile function.

//--- find the chunk with tag = supplied ID (as  packed 4 character tag); 
//--- file is positioned after the tag; if not found then file position is undefined.
bool PCMWave::FindChunk (ifstream *anInfile, unsigned long aTag)
{
	unsigned long inTag;
	// if already at correct chunk then return.
	anInfile->read ((char*)(&inTag), 4);
	if (anInfile->good() && inTag==aTag) {
		return true;		// we have found the tag.
	}
	// else start looking from the start of file, after the RIFF header fields (12 bytes).
	anInfile->seekg (12);
	anInfile->read ((char*)(&inTag), 4);
	while (anInfile->good() && inTag!=aTag) {		// repeat until EOF or correct chunk found.
		// incorrect chunk so  get this chunk size and move to end of chunk.
		unsigned long chunkSize;
		anInfile->read ((char*)(&chunkSize), 4);
		anInfile->seekg (chunkSize, ios_base::cur);
		anInfile->read ((char*)(&inTag), 4);		// read in next tag.
	}
	if (anInfile->good()) {
		return true;		// found the chunk tag.
	} else {
		return false;		// chunk not found.
	}
} // end FindChunk function.

//--- write a .wav file from state of this object.
void PCMWave::WriteWaveFile (ofstream *anOutfile)
{
	unsigned long fields[3];		// use to create chunk header fields.
	// write RIFF header fields.
	fields[0] = 'FFIR';
	fields[1] = mSize + 36;		// buffer size + 'wave' (4) + fmt chunk 24() + data chunk header (8).
	fields[2] = 'EVAW';
	anOutfile->write ((char*)fields, 12);
	if (!anOutfile->good()) {
		mStatus = RiffWriteError;
		return;
	}
	// write fmt chunk.
	fields[0] = ' tmf';
	fields[1] = 16;
	anOutfile->write ((char*)fields, 8);
	if (!anOutfile->good()) {
		mStatus = FmtWriteError;
		return;
	}
	anOutfile->write ((char*)(&mWaveFormat), sizeof(WaveFmt));
	if (!anOutfile->good()) {
		mStatus = FmtWriteError;
		return;
	}
	// write data chunk.
	fields[0] = 'atad';
	fields[1] = mSize;
	anOutfile->write ((char*)fields, 8);
	if (!anOutfile->good()) {
		mStatus = DataWriteError;
		return;
	}
	anOutfile->write (mWaveData, mSize);
	if (!anOutfile->good()) {
		mStatus = DataWriteError;
		return;
	}
	mStatus = OK;		// all write operations succeeded.
} // end WriteWaveFile function.

/* Version history.
=== 2.2
Removed checks for NULL before calls to delete since delete works ok for null references.
*/

} // end Audio namespace.
} // end AllanMilne namespace.