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
	float CoherentNoise(float x, float y);

	//  Improved Perlin Noise
	float Improved2DPerlin(float x, float y);
	Vector3 ImprovedPerlinNormal(float x, float y, NoiseObject n, float step);
	float ImprovedFractal2DPerlin(float x, float y, NoiseObject n);

	//  3D Improved Perlin Noise
	float Improved3DPerlin(float x, float y, float z);
	float ImprovedFractal3DPerlin(float x, float y, float z, NoiseObject n);


	//  Basic 4-octave SIMD coherent noise function
	float CoherentNoiseOpt(float x, float y, NoiseObject n, int iteration);
	float CoherentNoiseOpt(float x, float y, float zoom, float persistance, int base);
	//  Fractal coherent noise function
	float FractalCoherentNoise(float x, float y, NoiseObject n);
	float FractalCoherentNoise(float x, float y, int octaves, float zoom, float persistance, float amplitude);
	float SIMDCoherentFractalNoise(float x, float y, NoiseObject n);

	//  Find normals to the coherent noise function
	Vector3 CoherentNoiseNormal(float x, float y, int octaves, float zoom, float persistance, float amplitude, float step);
	Vector3 CoherentNoiseNormal(float x, float y, NoiseObject n, float step);
	Vector3 SIMDCoherentNoiseNormal(float x, float y, NoiseObject n, float step);

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

	inline float Fade(float t) const { return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f); }
	inline float Lerp(float t, float a, float b) const { return a + t * (b - a); }
	inline float Grad(int hash, float x, float y, float z) const {
		int h = hash & 15;
		float u = (h<8 ? x : y);
		float v = (h<4 ? y : (h==12||h==14 ? x : z));
		return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
	}

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
