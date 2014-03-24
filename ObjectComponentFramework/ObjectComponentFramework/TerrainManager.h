#pragma once
#include "GameObject.h"

#include <ppl.h>
#include <concurrent_vector.h>

namespace rabd
{
	static const int NUMCHUNKS = 4;
	class TerrainManager
	{
	public:
		TerrainManager();
		~TerrainManager();

		void UpdateTerrain();
	private:
		ObjectID chunks[NUMCHUNKS*NUMCHUNKS];
		Concurrency::concurrent_vector<ObjectID> chunksCon;
	};
};
