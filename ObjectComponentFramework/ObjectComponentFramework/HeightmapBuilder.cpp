#include "HeightmapBuilder.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <ppl.h>


//  A structure for holding the variables needed for win32 threading functions
struct ThreadData
{
	ThreadData():n(8, 500.0f, 0.41f, 70.0f, -1563.155f){}
	ThreadData(unsigned short* p, int baseX, int baseY, int size, float mbaseX, float mbaseY, float msize, int ssize, NoiseObject noise) :
		start(p), imageBaseX(baseX), imageBaseY(baseY), imageSize(size), mapBaseX(mbaseX), mapBaseY(mbaseY),	mapSize(msize),
			sectionSize(ssize),	n(noise), completed(false) {}
	unsigned short* start;
	int imageBaseX;
	int imageBaseY;
	int imageSize;
	float mapBaseX;
	float mapBaseY;
	float mapSize;
	int sectionSize;
	NoiseObject n;
	bool completed;
};


HeightmapBuilder::HeightmapBuilder()
{}

HeightmapBuilder::~HeightmapBuilder()
{}


void HeightmapBuilder::GenHeightsSIMD(float x, float y, NoiseObject n, float square)
/*-------------------------------------------------------------------------*\
|	Purpose:	Generate a texture (size x size) using SIMD instructions	|
|																			|
|	Parameters:	The x and y position of the base of the texture within the 	|
|				noise function, the noise parameters and the size of the	|
|				texture in global space (square x square)					|
|																			|
|	Returns:	Writes a texture image file in the executable directory		|
|																			|
\*-------------------------------------------------------------------------*/
{
	unsigned short* map = new unsigned short[size*size*4];
	float step = float((square+2)/size);

	float maxAmp = 0;

	NoiseGenerator noise;
	maxAmp = noise.MaxAmplitude(n);
	for (int j = 0; j < size; j++) {
		int counter = j*size;
		int currentpixel = counter*4;
		for (float i = 0; i < size; i++) {
			float height = 0;
			for (int k = 0; k < n.octaves; k+=4) {
				height += noise.Perlin2DFourPass(i*step + x, j*step + y, n.zoom, n.persistance, k);
			}
			
			height /= maxAmp;
			height *= n.amplitude;

			//  Convert the numbers to short int
			map[currentpixel] = unsigned short(((height+n.amplitude)/(2*n.amplitude)) * 65535);			//  R
			map[currentpixel + 1] = unsigned short(((height+n.amplitude)/(2*n.amplitude)) * 65535);		//  G
			map[currentpixel + 2] = unsigned short(((height+n.amplitude)/(2*n.amplitude)) * 65535);		//  B
			map[currentpixel + 3] = 65535;		// A

			counter++;
			currentpixel = counter*4;
		}	
	}

//	write_tga("SIMDPerlinNoise.tga", size, map);

	delete[] map;
}

unsigned short* HeightmapBuilder::GenerateHeightField(float x, float y, NoiseObject n, float square)
/*-------------------------------------------------------------------------*\
|	Purpose:	Generate a texture (size x size) using Win32 threading		|
|																			|
|	Parameters:	The x and y position of the base of the texture within the 	|
|				noise function, the noise parameters and the size of the	|
|				texture in global space (square x square)					|
|																			|
|	Returns:	A pointer to the texture array as a collection of short ints|
|																			|
\*-------------------------------------------------------------------------*/
{
	const int subdivs = 256;  //  256, 512, 1024
	const int threads = size/subdivs;

	unsigned short* map = new unsigned short[size*size*4];

	NoiseGenerator noise;
	float max = n.amplitude;
	int counter = 0;
	int currentpixel = 0;
	float step = float(square/size);

	ThreadData dataArray[threads][threads];
	HANDLE threadHandles[threads*threads];
	
	for(int i = 0; i < threads; i++) {
		for(int j = 0; j < threads; j++) {
			dataArray[i][j] = ThreadData(map, i*size/threads, j*size/threads, size, i*step*subdivs, j*step*subdivs, square, subdivs, n);
			threadHandles[j+i*threads] = (HANDLE) _beginthreadex(NULL,0,&HeightmapBuilder::GenerateSection,(void*)&dataArray[i][j],0,NULL);
		}
	}

	for (int i = 0; i < threads*threads; i+=64) {
		if (threads*threads - i > 64) {
			WaitForMultipleObjects(64, &threadHandles[i], TRUE, INFINITE);
		} else {
			WaitForMultipleObjects(threads*threads-i, &threadHandles[i], TRUE, INFINITE);
		}
	}

//	write_tga("ThreadedSimdPerlin.tga", size, map);


	return map;
}

unsigned short* HeightmapBuilder::GenerateHeights(float x, float y, NoiseObject n, float square)
/*-------------------------------------------------------------------------*\
|	Purpose:	Generate a texture (size x size) using Win32 threading		|
|																			|
|	Parameters:	The x and y position of the base of the texture within the 	|
|				noise function, the noise parameters and the size of the	|
|				texture in global space (square x square)					|
|																			|
|	Returns:	A pointer to the texture array as a collection of short ints|
|																			|
\*-------------------------------------------------------------------------*/
{
	const int subdivs = 256;  //  256, 512, 1024
	const int threads = size/subdivs;

	unsigned short* map = new unsigned short[size*size*4];

	NoiseGenerator noise;
	float max = n.amplitude;
	int counter = 0;
	int currentpixel = 0;
	float step = float(square/size);

	ThreadData dataArray[threads][threads];
	HANDLE threadHandles[threads*threads];
	
	for(int i = 0; i < threads; i++) {
		for(int j = 0; j < threads; j++) {
			dataArray[i][j] = ThreadData(map, i*size/threads, j*size/threads, size, i*step*subdivs, j*step*subdivs, square, subdivs, n);
			threadHandles[j+i*threads] = (HANDLE) _beginthreadex(NULL,0,&HeightmapBuilder::GenerateSection,(void*)&dataArray[i][j],0,NULL);
		}
	}

	for (int i = 0; i < threads*threads; i+=64) {
		if (threads*threads - i > 64) {
			WaitForMultipleObjects(64, &threadHandles[i], TRUE, INFINITE);
		} else {
			WaitForMultipleObjects(threads*threads-i, &threadHandles[i], TRUE, INFINITE);
		}
	}

	return map;
}

void HeightmapBuilder::GenerateHeightmapAsync(unsigned short** output, float x, float y, NoiseObject n, float square)
{
	using namespace Concurrency;


}

void HeightmapBuilder::write_tga(const char *filename, int size, unsigned char* base)
{
	std::ofstream o(filename, std::ios::out | std::ios::binary);

	//Write the header
	o.put(0);
   	o.put(0);
   	o.put(2);					/* uncompressed RGB */
   	o.put(0);	o.put(0);
   	o.put(0); 	o.put(0);
   	o.put(0);
   	o.put(0); 	o.put(0);		/* X origin */
   	o.put(0); 	o.put(0);		/* y origin */
   	o.put((size & 0x00FF));
   	o.put((size & 0xFF00) / 256);
   	o.put((size & 0x00FF));
   	o.put((size & 0xFF00) / 256);
   	o.put(32);					/* 32 bit bitmap */
   	o.put(0);
   	
	//Write the pixel data
	for (int i=0;i<size*size;i++) {
		o.put(base[i*4]);
		o.put(base[i*4+1]);
		o.put(base[i*4+2]);
		o.put(base[i*4+3]);
	}   
	
	//close the file
	o.close();

}

void HeightmapBuilder::write_tga(const char *filename, int size, unsigned short* base)
{
	std::ofstream o(filename, std::ios::out | std::ios::binary);

	//Write the header
	o.put(0);
   	o.put(0);
   	o.put(2);					/* uncompressed RGB */
   	o.put(0);	o.put(0);
   	o.put(0); 	o.put(0);
   	o.put(0);
   	o.put(0); 	o.put(0);		/* X origin */
   	o.put(0); 	o.put(0);		/* y origin */
   	o.put((size & 0x00FF));
   	o.put((size & 0xFF00) / 256);
   	o.put((size & 0x00FF));
   	o.put((size & 0xFF00) / 256);
   	o.put(32);					/* 32 bit bitmap */
   	o.put(0);
   	
	//Write the pixel data
	for (int i=0;i<size*size;i++) {
		o.put(char(base[i*4]/256));
		o.put(char(base[i*4+1]/256));
		o.put(char(base[i*4+2]/256));
		o.put(char(base[i*4+3]/256));
	}   
	
	//close the file
	o.close();
}

unsigned __stdcall HeightmapBuilder::GenerateSection(void *data)
/*-------------------------------------------------------------------------*\
|	Purpose:	Generate a portion of a larger texture, single thread		|
|																			|
|	Parameters:	The x and y position of the base of the texture within the 	|
|				noise function, the noise parameters and the size of the	|
|				texture in global space (square x square)					|
|																			|
|	Returns:	Updates portion of texture array with noise values			|
|																			|
\*-------------------------------------------------------------------------*/
{
	ThreadData args = *(ThreadData*)data;

	float height = 0;
	Vector3 normal(0,0,0);

	NoiseGenerator noise;
	float step = args.mapSize/args.imageSize;
	float x = args.mapBaseX;
	float y = args.mapBaseY;
	int counter = 0;
	int currentpixel = 0;
	float max = args.n.amplitude;

	for (float j = 0; j < args.sectionSize; j++) {
		counter = (j+args.imageBaseY) * args.imageSize + args.imageBaseX;
		currentpixel = counter*4;
		for (float i = 0; i < args.sectionSize; i++) {
			height = noise.SIMDPerlin2D(i*step + x, j *step + y, args.n);
			normal = noise.SIMDPerlinNormal(i*step + x, j*step + y, args.n, step);

			//  Convert the numbers to short int
			args.start[currentpixel] = unsigned short((normal.x+1)/2 * 65535);			//  R
			args.start[currentpixel + 1] = unsigned short((normal.y+1)/2 * 65535);		//  G
			args.start[currentpixel + 2] = unsigned short((normal.z+1)/2 * 65535);		//  B
			args.start[currentpixel + 3] = unsigned short(((height+max)/(2*max)) * 65535);		// A

			counter++;
			currentpixel = counter*4;
		}
	}

	((ThreadData*)data)->completed = true;

	return 0;
}