#pragma once
///////////////////////// By Ross Davies /////////////////////////
//	Noise generation class, this class can generate both		//
//	non-coherent and coherent noise using Perlin or Simplex		//
//	methods, can generate Perlin noise 4-octaves at a time		//
//	using SIMD instructions.									//
//////////////////////////////////////////////////////////////////
#include "simd.h"

class Vector3;

class NoiseObject
{
public:
	NoiseObject(int octaves, float zoom, float persistance, float amplitude, float seed);
	int octaves;
	float zoom;
	float persistance;
	float amplitude;
	float seed;
};

class NoiseGenerator
{
public:
	NoiseGenerator(void);
	~NoiseGenerator(void);
	//  Basic Perlin noise function
	float Perlin2DSinglePass(float x, float y);
	//  Basic 4-octave SIMD perlin noise function
	float Perlin2DFourPass(float x, float y, NoiseObject n, int iteration);
	float Perlin2DFourPass(float x, float y, float zoom, float persistance, int base);
	//  Fractal perlin function
	float Perlin2D(float x, float y, NoiseObject n);
	float Perlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude);
	float SIMDPerlin2D(float x, float y, NoiseObject n);

	//  Find normals to the perlin noise function
	Vector3 NormalToPerlin2D(float x, float y, int octaves, float zoom, float persistance, float amplitude, float step);
	Vector3 NormalToPerlin2D(float x, float y, NoiseObject n, float step);
	Vector3 SIMDPerlinNormal(float x, float y, NoiseObject n, float step);

	//  Basic Simplex Noise function
	float Simplex(float x, float y);
	//  Fractal simplex noise function
	float FractalSimplex(float x, float y, NoiseObject n);
	//  Find normals for the simplex noise function
	Vector3 FractalSimplexNormal(float x, float y, NoiseObject n, float step);

	//  Generate the simplex permutation table
	static void GeneratePermutationTable();
	//  Seed the hashing generator
	void Seed(float s){seed = s;}

	//  Find the max amplitude after a given number of octaves (for normalization)
	float MaxAmplitude(NoiseObject n);
private:

	//  Hashing function
	float NonCoherentNoise2D(float x, float y);
	SIMD::Floats NonCoherentNoise2D(SIMD::Floats& x, SIMD::Floats& y);
	float NonCoherentNoise1D(float x);

	//  Interpolation functions
	float Interpolate(float a, float b, float x);
	SIMD::Floats Interpolate (SIMD::Floats& a, SIMD::Floats& b, SIMD::Floats& x);


	float seed;

	//  Look-up table data
	static const int SIZE = 256;
	static unsigned char permutation[SIZE];
	static unsigned char perm[SIZE*2];
	static float gradX[SIZE];
	static float gradY[SIZE];
	static bool pTableBuilt;

	static int grads[12][3];
};

