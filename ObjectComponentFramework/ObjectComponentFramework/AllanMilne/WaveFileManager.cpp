/*
	file:	WaveFileManager.cpp
	Version:	2.2
	Date:	31st March 2013.
	Authors:	Stuart & Allan Milne.

	Namespace:	AllanMilne::Audio
	Exposes: WaveFileManager.
	Requires:	PCMWave.

	Description:
	Caches PCMWave objects representing raw audio data loaded from .wav files.
	this is the implementation file - see WaveFileManager.hpp for details.

*/

#include <string>
#include <map>
using std::string;
using std::map;

#include "WaveFileManager.hpp"
#include "PCMWave.hpp"

namespace AllanMilne {
namespace Audio {

	//--- Initialise the static instance field.
	WaveFileManager *WaveFileManager::mInstance = NULL;

//--- Get the singleton instance - use lazy evaluation.
WaveFileManager& WaveFileManager::GetInstance () 
{
	// Instance already created so return it.
	if (mInstance) { return *mInstance; }
	// First time called so instantiate the object.
	mInstance = new WaveFileManager ();
	return *mInstance;
} // end static GetInstance function.

//--- Delete manager instance, will call instance destructor and hence all PCMWave destructors.
void WaveFileManager::DeleteInstance ()
{
	delete mInstance;
	mInstance = NULL;
} // end static DeleteInstance function.

//--- Returns true if named file is in the cache; false otherwise.
bool WaveFileManager::IsLoaded (const string &aFileName)
{
	if (mWaveFileCache.count(aFileName) == 0) {
		return false;
	} else {
		return true;
	}
} // end IsLoaded function.

//--- Retrieve the PCMWave object for the named file; loads it into the cache if not already loaded.
PCMWave *WaveFileManager::LoadWave (const string &aFileName)
{
	// If this file has not been loaded then add it to the cache.
	if (mWaveFileCache.count(aFileName) == 0) {
		PCMWave *wave = new PCMWave (aFileName);
		mWaveFileCache.insert (std::pair<string, PCMWave*> (aFileName, wave) );
	}
	return mWaveFileCache[aFileName];
} // end LoadWave function.

//--- delete named PCMWave object from cache; returns false if named file not loaded into cache.
//--- NB will cause destructor of the relevant PCMWave object to be called and hence all associated sound sample data will be invalid.
bool WaveFileManager::UnloadWave (const string &aFileName)
{
	// If this file has not been loaded we can return false now.
	if (mWaveFileCache.count(aFileName) == 0) { return false; }
	// delete the PCMWave object.
	delete mWaveFileCache[aFileName];
	// Remove entry from the cache.
	mWaveFileCache.erase (aFileName);
	return true;
} // end UnloadWave function.

// Constructor & destructor are hidden to implement singleton patern.
WaveFileManager::WaveFileManager ()
{
} // end WaveFileManager constructor.

WaveFileManager::	~WaveFileManager ()
{
	// Delete all the PCMWave objects in the cache.
	for (map<string, PCMWave*>::iterator wave=mWaveFileCache.begin(); wave!=mWaveFileCache.end(); ++wave) {
		delete wave->second;		// The PCMWave object pointer of the map <key,value> pair.
	}
} // end WaveFileManager destructor.

} // end Audio namespace.
} // end AllanMilne namespace.