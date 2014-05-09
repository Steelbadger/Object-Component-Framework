#include "noisegenerator.h"
#include <math.h>
#include "Vector4.h"
#include "Matrix4x4.h"
#include <stdlib.h>
#include <algorithm>

unsigned char NoiseGenerator::permutation[SIZE];
unsigned char NoiseGenerator::perm[SIZE*2];
float NoiseGenerator::gradX[SIZE];
float NoiseGenerator::gradY[SIZE];

int NoiseGenerator::grads[12][3] = {{1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0},
									{1,0,1},{-1,0,1},{1,0,-1},{-1,0,-1},
									{0,1,1},{0,-1,1},{0,1,-1},{0,-1,-1}};

bool NoiseGenerator::pTableBuilt = false;




void NoiseGenerator::GeneratePermutationTable()
/*-------------------------------------------------------------------------*\
|	Purpose:	Generate the permutation table used for simplex noise		|
|				This should ONLY happen ONCE per application execution		|
|																			|
|	Parameters:	None.														|
|																			|
|	Returns:	Fills the permutations tables with pseudorandom numbers		|
|																			|
\*-------------------------------------------------------------------------*/
{
	int j;
	int tempSwap;
	for(int i = 0; i < SIZE; i++) {
		permutation[i] = i;
	}

	for(int i = 0; i < SIZE; i++) {
		j = rand() % SIZE;
		tempSwap = permutation[i];
		permutation[i]  = permutation[j];
		permutation[j]  = tempSwap;
	}

	for(int i = 0; i < SIZE; i++) {
		gradX[i] = rand()/float(RAND_MAX/2) - 1.0f; 
		gradY[i] = rand()/float(RAND_MAX/2) - 1.0f;
	}

	for (int i = 0; i < SIZE*2; i++) {
		perm[i] = permutation[i&255];
	}

	pTableBuilt = true;
}


NoiseObject::NoiseObject(int o, float z, float p, float a, float s):
	octaves(o),
	zoom(z),
	persistance(p),
	amplitude(a),
	seed(s)
{}

NoiseGenerator::NoiseGenerator(void)
{
	if (!pTableBuilt) {
		GeneratePermutationTable();
	}
	seed = 1.0f;
}


NoiseGenerator::~NoiseGenerator(void)
{
}

float NoiseGenerator::CoherentNoise(float x, float y)
/*-------------------------------------------------------------------------*\
|	Purpose:	Do a single coherent noise calculation				|
|																			|
|	Parameters:	the x and y position of the point to sample					|
|																			|
|	Returns:	the magnitude of the perlin function at that point			|
|																			|
\*-------------------------------------------------------------------------*/
{
	float floorX = float(int(x));
	float floorY = float(int(y));

	float s, t, u, v;							//The four corners

	s = NonCoherentNoise2D(floorX,floorY); 
	t = NonCoherentNoise2D(floorX+1,floorY);
	u = NonCoherentNoise2D(floorX,floorY+1);	//Get the surrounding pixels to calculate the transition.
	v = NonCoherentNoise2D(floorX+1,floorY+1);

	float int1 = Interpolate(s,t,x-floorX);		//Interpolate between the values.
	float int2 = Interpolate(u,v,x-floorX);		//Here we use x-floorx, to get 1st dimension.
	return Interpolate(int1, int2, y-floorY);	//Here we use y-floory, to get the 2nd dimension.
}

float NoiseGenerator::Improved2DPerlin(float x, float y)
/*-------------------------------------------------------------------------*\
|	Purpose:	Do a single Perlin coherent noise calculation				|
|																			|
|	Parameters:	the x and y position of the point to sample					|
|																			|
|	Returns:	the magnitude of the perlin function at that point			|
|																			|
\*-------------------------------------------------------------------------*/
{
		int X = (int)x & 255;				// FIND UNIT CUBE THAT
		int Y = (int)y & 255;				// CONTAINS POINT.
		x -= (float)int(x);					// FIND RELATIVE X,Y,Z
		y -= (float)int(y);					// OF POINT IN CUBE.

		float u = Fade(x);					// COMPUTE FADE CURVES
		float v = Fade(y);					// FOR EACH OF X,Y,Z.

		int A = perm[X]+Y;
		int AA = perm[A];
		int AB = perm[A+1];
		int B = perm[X+1]+Y;	
		int BA = perm[B];
		int BB = perm[B+1];

		float s1 = Lerp(u, Grad(perm[AA], x, y, 0.0f), Grad(perm[BA], x-1, y, 0.0f));
		float s2 = Lerp(u, Grad(perm[AB], x, y-1, 0.0f), Grad(perm[BB], x-1, y-1, 0.0f));

		return Lerp(v, s1, s2);
}

Vector3 NoiseGenerator::ImprovedPerlinNormal(float x, float y, NoiseObject n, float step)
/*-------------------------------------------------------------------------*\
|	Purpose:	Normal sampling function for perlin noise					|
|				Samples 3 points around point of interest					|
|				and interpolates normal based on this information			|
|																			|
|	Parameters:	the x and y position of the point to sample and the noise	|
|				parameters to use											|					
|																			|
|	Returns:	A normalised vectorthat describes the normal of the perlin	|
|				noise function at that point								|
|																			|
\*-------------------------------------------------------------------------*/
{

	float offs = step;
	float xtrioffs = offs * 0.86602540378;
	float ytrioffs = offs * 0.5;
	float maxamp = MaxAmplitude(n);
	Vector4 A(x, (ImprovedFractal2DPerlin(x,y+xtrioffs, n)*n.amplitude)/maxamp, y+xtrioffs, 1.0f);
	Vector4 B(x-xtrioffs, (ImprovedFractal2DPerlin(x-xtrioffs,y-ytrioffs, n)*n.amplitude)/maxamp, y-ytrioffs, 1.0f);
	Vector4 C(x+xtrioffs, (ImprovedFractal2DPerlin(x+xtrioffs,y-ytrioffs, n)*n.amplitude)/maxamp, y-ytrioffs, 1.0f);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = Cross(AC, AB);
	Normal.NormaliseSelf();
	Vector3 output(Normal);
	return output;
}

float NoiseGenerator::ImprovedFractal2DPerlin(float x, float y, NoiseObject n)
/*-------------------------------------------------------------------------*\
|	Purpose:	Sample the fractal perlin function at point x,y using		|
|				noise decribed by noise parameters							|
|																			|
|	Parameters:	the x and y position of the point to sample and the noise	|
|				parameters to use											|					
|																			|
|	Returns:	The magnitude of the fractal perlin noise function at that	|
|				point														|
|																			|
\*-------------------------------------------------------------------------*/
{
	float noise = 0;
	for(int i = 0; i < n.octaves; i++) {
		float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
		float amplitude = pow(n.persistance,i);//This decreases the amplitude with every loop of the octave.

		noise += Improved2DPerlin(x*frequency/n.zoom, y/n.zoom*frequency)*amplitude;
	}

	return noise;
}

float NoiseGenerator::Improved3DPerlin(float x, float y, float z)
/*-------------------------------------------------------------------------*\
|	Purpose:	Do a single Perlin coherent noise calculation				|
|																			|
|	Parameters:	the x and y position of the point to sample					|
|																			|
|	Returns:	the magnitude of the perlin function at that point			|
|																			|
\*-------------------------------------------------------------------------*/
{
		int X = (int)x & 255;				// FIND UNIT CUBE THAT
		int Y = (int)y & 255;				// CONTAINS POINT.
		int Z = (int)z & 255;
		x -= (float)int(x);					// FIND RELATIVE X,Y,Z
		y -= (float)int(y);					// OF POINT IN CUBE.
		z -= (float)int(z);

		float u = Fade(x);					// COMPUTE FADE CURVES
		float v = Fade(y);					// FOR EACH OF X,Y,Z.
		float w = Fade(z);

		int A = perm[X]+Y;
		int AA = perm[A]+Z;
		int AB = perm[A+1]+Z;		// HASH COORDINATES OF
		int B = perm[X+1]+Y;			// THE 8 CUBE CORNERS,
		int BA = perm[B]+Z;
		int BB = perm[B+1]+Z;

		float s1 = Lerp(u, Grad(perm[AA], x, y, z), Grad(perm[BA], x-1, y, z));
		float s2 = Lerp(u, Grad(perm[AB], x, y-1, z), Grad(perm[BB], x-1, y-1, z));

		float s = Lerp(v, s1, s2);

		s1 = Lerp(u, Grad(perm[AA+1], x, y, z-1), Grad(perm[BA+1], x-1, y, z-1));
		s2 = Lerp(u, Grad(perm[AB+1], x  , y-1, z-1 ), Grad(perm[BB+1], x-1, y-1, z-1));

		return Lerp(w, s, Lerp(v, s1, s2));
}


float NoiseGenerator::ImprovedFractal3DPerlin(float x, float y, float z, NoiseObject n)
/*-------------------------------------------------------------------------*\
|	Purpose:	Sample the fractal perlin function at point x,y,z using		|
|				noise decribed by noise parameters							|
|																			|
|	Parameters:	the x, y & z position of the point to sample and the noise	|
|				parameters to use											|					
|																			|
|	Returns:	The magnitude of the fractal perlin noise function at that	|
|				point														|
|																			|
\*-------------------------------------------------------------------------*/
{
	float noise = 0;
	for(int i = 0; i < n.octaves; i++) {
		float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
		float amplitude = pow(n.persistance,i);//This decreases the amplitude with every loop of the octave.

		noise += Improved3DPerlin(x*frequency/n.zoom, y/n.zoom*frequency, z/n.zoom*frequency)*amplitude;
	}

	return noise;
}


float NoiseGenerator::CoherentNoiseOpt(float xin, float yin, float zoom, float persistance, int base)
/*-------------------------------------------------------------------------*\
|	Purpose:	Do 4 Perlin checks for a given point (Fractal)				|
|																			|
|	Parameters:	The x and y position of the point, the zoom and persistance	|
|				levels of the sampling function, and the depth into the		|
|				fractal that this point is currently sampling				|
|																			|
|	Returns:	The magnitude of the fractal perlin function at that point	|
|																			|
\*-------------------------------------------------------------------------*/
{
	using namespace SIMD;

	Floats scaleFactor(pow(2.0, base), pow(2.0, base+1), pow(2.0, base+2), pow(2.0, base+3));
	scaleFactor /= zoom;
	Floats ampFactor(pow(persistance, base), pow(persistance, base+1), pow(persistance, base+2), pow(persistance, base+3));

	Floats x = xin * scaleFactor;
	Floats y = yin * scaleFactor;

	Floats floorX = Integers(x);
	Floats floorY = Integers(y);
	
	Floats s = NonCoherentNoise2D(floorX,floorY); 
	Floats t = NonCoherentNoise2D(floorX+1.0f,floorY);
	Floats u = NonCoherentNoise2D(floorX,floorY+1.0f);	//Get the surrounding pixels to calculate the transition.
	Floats v = NonCoherentNoise2D(floorX+1.0f,floorY+1.0f);

	Floats int1 = Interpolate(s, t, x-floorX);
	Floats int2 = Interpolate(u, v, x-floorX);

	Floats out = Interpolate(int1, int2, y-floorY);

	out *= ampFactor;
	
	return out.Sum();


}

SIMD::Floats NoiseGenerator::Interpolate (SIMD::Floats& a, SIMD::Floats& b, SIMD::Floats& x)
/*-------------------------------------------------------------------------*\
|	Purpose:	Interpolate 4 sets of values simultaneously using SIMD		|
|				Uses Cosine interpolation									|
|																			|
|	Parameters:	Two sets of values to interpolate between and a set of	 	|
|				distances along the curves between each point				|
|																			|
|	Returns:	The interpolated value										|
|																			|
\*-------------------------------------------------------------------------*/
{
	using namespace SIMD;
	Floats ft = x * 3.1415927;
	Floats f = (1.0 - Cosine(ft)) * 0.5;
	return (a * (1.0 - f) + b * f);
}

float NoiseGenerator::SIMDCoherentFractalNoise(float x, float y, NoiseObject n)
/*-------------------------------------------------------------------------*\
|	Purpose:	Sample the fractal perlin function at point x,y using		|
|				noise decribed by noiseobject n								|
|																			|
|	Parameters:	the x and y position of the point to sample and the noise	|
|				parameters to use											|					
|																			|
|	Returns:	The magnitude of the fractal perlin noise function at that	|
|				point														|
|																			|
\*-------------------------------------------------------------------------*/
{
	float height = 0;

	for (int k = 0; k < n.octaves; k+=4) {
		height += CoherentNoiseOpt(x, y, n.zoom, n.persistance, k);
	}

	return height;
}

float NoiseGenerator::MaxAmplitude(NoiseObject n)
/*-------------------------------------------------------------------------*\
|	Purpose:	Using the noise parameters find the maximum amplitude of 	|
|				any noise function (for normalisation purposes)				|
|																			|
|	Parameters:	The noise parameters (octaves, and persistance primarily)	|
|																			|
|	Returns:	The maximum value that a noise function using these params	|
|				could give													|
|																			|
\*-------------------------------------------------------------------------*/
{
	float maxAmp = 0;
	for (int i = 0; i < n.octaves; i++) {
		maxAmp += pow(n.persistance,i);		//This decreases the amplitude with every loop of the octave.
	}

	return maxAmp;
}


float NoiseGenerator::FractalCoherentNoise(float x, float y, int octaves, float zoom, float persistance, float amp)
/*-------------------------------------------------------------------------*\
|	Purpose:	Sample the fractal coherent function at point x,y using		|
|				noise decribed by noise parameters							|
|																			|
|	Parameters:	the x and y position of the point to sample and the noise	|
|				parameters to use											|					
|																			|
|	Returns:	The magnitude of the fractal coherent noise function at that|
|				point														|
|																			|
\*-------------------------------------------------------------------------*/
{
	float noise = 0;
	float maxamp = 0;

	for(int i = 0; i < octaves; i++) {
		float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
		float amplitude = pow(persistance,i);//This decreases the amplitude with every loop of the octave.
		maxamp += amplitude;
		noise += CoherentNoise(x*frequency/(zoom*2), y*frequency/(zoom*2))*amplitude;
	}
	noise /= maxamp;

	return noise*amp;	
}

Vector3 NoiseGenerator::CoherentNoiseNormal(float x, float y, int octaves, float zoom, float persistance, float amp, float step)
/*-------------------------------------------------------------------------*\
|	Purpose:	High accuracy normal sampling function for perlin noise		|
|				samples 7 points around point of interest and interpolates	|
|				normal based on this information							|
|																			|
|	Parameters:	the x and y position of the point to sample and the noise	|
|				parameters to use											|					
|																			|
|	Returns:	A normalised vectorthat describes the normal of the perlin	|
|				noise function at that point								|
|																			|
\*-------------------------------------------------------------------------*/
{

	float x1 = x-step;
	float x2 = x;
	float x3 = x+step;
	float y1 = y-step;
	float y2 = y;
	float y3 = y+step;

	//  Find the six corners
	Vector4 A(x1, FractalCoherentNoise(x1,y1,octaves,zoom,persistance,amp), y1, 1.0f);
	Vector4 B(x2, FractalCoherentNoise(x2,y1,octaves,zoom,persistance,amp), y1, 1.0f);

	Vector4 C(x1, FractalCoherentNoise(x1,y2,octaves,zoom,persistance,amp), y2, 1.0f);
	Vector4 D(x2, FractalCoherentNoise(x2,y2,octaves,zoom,persistance,amp), y2, 1.0f);
	Vector4 E(x3, FractalCoherentNoise(x3,y2,octaves,zoom,persistance,amp), y2, 1.0f);

	Vector4 F(x2, FractalCoherentNoise(x2,y3,octaves,zoom,persistance,amp), y3, 1.0f);
	Vector4 G(x3, FractalCoherentNoise(x2,y3,octaves,zoom,persistance,amp), y3, 1.0f);

	//  Find all edges
	Vector4 DC = C - D;
	Vector4 DA = A - D;
	Vector4 DB = A - D;
	Vector4 DE = E - D;
	Vector4 DG = G - D;
	Vector4 DF = F - D;
	
	//  Find 6 neighbouring triangle normals
	Vector4 Normal1 = Cross(DC, DA);
	Vector4 Normal2 = Cross(DA, DB);
	Vector4 Normal3 = Cross(DB, DE);
	Vector4 Normal4 = Cross(DE, DG);
	Vector4 Normal5 = Cross(DG, DF);
	Vector4 Normal6 = Cross(DF, DC);

	//  find resultant
	Vector4 Normal = Normal1 + Normal2 + Normal3 + Normal4 + Normal5 + Normal6;

	Normal.NormaliseSelf();

	return Normal;
}

float NoiseGenerator::FractalCoherentNoise(float x, float y, NoiseObject n)
/*-------------------------------------------------------------------------*\
|	Purpose:	Sample the fractal perlin function at point x,y using		|
|				noise decribed by noise parameters							|
|																			|
|	Parameters:	the x and y position of the point to sample and the noise	|
|				parameters to use											|					
|																			|
|	Returns:	The magnitude of the fractal perlin noise function at that	|
|				point														|
|																			|
\*-------------------------------------------------------------------------*/
{
	return FractalCoherentNoise(x, y, n.octaves, n.zoom, n.persistance, n.amplitude);
}

Vector3 NoiseGenerator::CoherentNoiseNormal(float x, float y, NoiseObject n, float step)
/*-------------------------------------------------------------------------*\
|	Purpose:	High accuracy normal sampling function for perlin noise		|
|				samples 7 points around point of interest and interpolates	|
|				normal based on this information							|
|																			|
|	Parameters:	the x and y position of the point to sample and the noise	|
|				parameters to use											|					
|																			|
|	Returns:	A normalised vectorthat describes the normal of the perlin	|
|				noise function at that point								|
|																			|
\*-------------------------------------------------------------------------*/
{
	return CoherentNoiseNormal(x, y, n.octaves, n.zoom, n.persistance, n.amplitude, step);
}

float NoiseGenerator::Interpolate(float a, float b, float x)
/*-------------------------------------------------------------------------*\
|	Purpose:	Interpolate between two values using cosine interpolation	|
|																			|
|	Parameters:	Two values to interpolate between and a					 	|
|				distance along the curve									|
|																			|
|	Returns:	The interpolated value										|
|																			|
\*-------------------------------------------------------------------------*/
{
	float ft = x * 3.1415927;
	float f = (1.0 - cos(ft)) * 0.5;
	return (a * (1.0 - f) + b * f);
}

float NoiseGenerator::NonCoherentNoise1D(float x)
/*-------------------------------------------------------------------------*\
|	Purpose:	A 1D pseudorandom (or hashing) function						|
|																			|
|	Parameters:	floating point number to hash								|
|																			|
|	Returns:	A pseudorandom number from -1.0 to 1.0						|
|																			|
\*-------------------------------------------------------------------------*/
{	 
	int n = (n<<13) ^ n;
	return float( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);	
}

float NoiseGenerator::NonCoherentNoise2D(float x, float y)
/*-------------------------------------------------------------------------*\
|	Purpose:	A 2D pseudorandom (or hashing) function						|
|																			|
|	Parameters:	two floating point numbers to hash together					|
|																			|
|	Returns:	A pseudorandom number from -1.0 to 1.0						|
|																			|
\*-------------------------------------------------------------------------*/
{
	x *= seed;
	y *= seed;
	int n = int(x)+int(y*57);
	n = (n<<13)^n;
	int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0-((float)nn/1073741824.0);
}

SIMD::Floats NoiseGenerator::NonCoherentNoise2D(SIMD::Floats& calcx, SIMD::Floats& calcy)
/*-------------------------------------------------------------------------*\
|	Purpose:	A 2D pseudorandom (or hashing) function	(SIMD)				|
|																			|
|	Parameters:	two sets of floating point numbers to hash together			|
|																			|
|	Returns:	A set of pseudorandom numbers from -1.0 to 1.0				|
|																			|
\*-------------------------------------------------------------------------*/
{
	using namespace SIMD;

	Floats x = calcx;
	Floats y = calcy;


	x *= seed;
	y *= seed;
	y *= 57;

	Integers n = x + y;

	n = (n<<13)^n;
	Integers nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;

	return 1.0-(Floats(nn)/1073741824.0);
}


float NoiseGenerator::Simplex(float x, float y)
/*-------------------------------------------------------------------------*\
|	Purpose:	Do a single Simplex coherent noise calculation				|
|																			|
|	Parameters:	the x and y position of the point to sample					|
|																			|
|	Returns:	the magnitude of the simplex function at that point			|
|																			|
\*-------------------------------------------------------------------------*/
{
	float root3 = 1.73205080757;
	float n1, n2, n3;		// Noise contributions from the three corners

	// Skew the input space to determine which simplex cell we're in
	float skewFactor = 0.5*(root3-1.0);
	float s = (x+y)*skewFactor;			// Hairy factor for 2D

	int i = int(x+s);
	int j = int(y+s);

	float unskewFactor = (3.0-root3)/6.0;
	float t = (i+j)*unskewFactor;

	float X0 = i-t;		// Unskew the cell origin back to (x,y) space
	float Y0 = j-t;
	float dx = x-X0;		// The x,y distances from the cell origin
	float dy = y-Y0;

	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.

	int i1, j1;		// Offsets for second (middle) corner of simplex in (i,j) coords

	int check = dy-dx;

	if(dx>dy) {		// lower triangle, XY order: (0,0)->(1,0)->(1,1)
		i1=1;
		j1=0;
	}
	else {			// upper triangle, YX order: (0,0)->(0,1)->(1,1)
		i1=0;
		j1=1;
	}				


	float x2 = dx - i1 + unskewFactor;				// Offsets for middle corner in (x,y) unskewed coords
	float y2 = dy - j1 + unskewFactor;
	float x3 = dx - 1.0 + 2.0 * unskewFactor;		// Offsets for last corner in (x,y) unskewed coords
	float y3 = dy - 1.0 + 2.0 * unskewFactor;

	// Work out the hashed gradient indices of the three simplex corners

	int ii = i & 255;
	int jj = j & 255;

	int grad1 = perm[ii+perm[jj]] % 12;
	int grad2 = perm[ii+i1+perm[jj+j1]] % 12;
	int grad3 = perm[ii+1+perm[jj+1]] % 12;

	float t1 = 0.5 - dx*dx-dy*dy;
	if (t1<0) {
		n1 = 0.0;
	} else {
		t1 *= t1;
		n1 = t1 * t1 * (grads[grad1][0] * dx + grads[grad1][1] * dy);		// (x,y) of grad3 used for 2D gradient
	}


	float t2 = 0.5 - x2*x2-y2*y2;
	if (t2<0) {
		n2 = 0.0;
	} else {
		t2 *= t2;
		n2 = t2 * t2 *(grads[grad2][0] * x2 + grads[grad2][1] * y2);
	}


	float t3 = 0.5 - x3*x3-y3*y3;
	if (t3<0) {
		n3 = 0.0;
	} else {
		t3 *= t3;
		n3 = t3 * t3 * (grads[grad3][0]* x3 + grads[grad3][1] * y3);
	}


	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1] (ish).
	return 70.0 * (n1 + n2 + n3);

}


float NoiseGenerator::FractalSimplex(float x, float y, NoiseObject n)
/*-------------------------------------------------------------------------*\
|	Purpose:	Sample the fractal Simplex function (defined by the			|
|				NoiseObject) at a point										|
|																			|
|	Parameters:	the x and y position of the point to sample	and a 			|
|				NoiseObject to describe the noise function					|
|																			|
|	Returns:	the magnitude of the fractal simplex function at that point	|
|																			|
\*-------------------------------------------------------------------------*/
{
	float noise = 0;
	float maxamp = 0;
	x = x + 15000;
	y = y + 15000;
	for(int i = 0; i < n.octaves; i++) {
		float frequency = pow(2.0f,i);//This increases the frequency with every loop of the octave.
		float amplitude = pow(n.persistance,i);//This decreases the amplitude with every loop of the octave.
		maxamp += amplitude;
		noise += Simplex(x*frequency/(n.zoom*2), y*frequency/(n.zoom*2))*amplitude;
	}
	noise /= maxamp;

	return noise*n.amplitude;	
}

Vector3 NoiseGenerator::FractalSimplexNormal(float x, float y, NoiseObject n, float step)
/*-------------------------------------------------------------------------*\
|	Purpose:	Sample the fractal Simplex function (defined by the			|
|				NoiseObject) at a point	to find the normal					|
|																			|
|	Parameters:	the x and y position of the point to sample	and a 			|
|				NoiseObject to describe the noise function as well as the	|
|				distance between samples									|
|																			|
|	Returns:	A vector3 that describes the normal of the fractal simplex	|
|				function at that point										|
|																			|
\*-------------------------------------------------------------------------*/
{
	float offs = step;
	float xtrioffs = offs * 0.86602540378;
	float ytrioffs = offs * 0.5;
	Vector4 A(x, FractalSimplex(x,y+xtrioffs,n), y+xtrioffs, 1.0f);
	Vector4 B(x-xtrioffs, FractalSimplex(x-xtrioffs,y-ytrioffs,n), y-ytrioffs, 1.0f);
	Vector4 C(x+xtrioffs, FractalSimplex(x+xtrioffs,y-ytrioffs,n), y-ytrioffs, 1.0f);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = Cross(AC, AB);
	Normal.NormaliseSelf();
	Vector3 output(Normal);
	return output;
}

Vector3 NoiseGenerator::SIMDCoherentNoiseNormal(float x, float y, NoiseObject n, float step)
/*-------------------------------------------------------------------------*\
|	Purpose:	Normal sampling function for perlin noise using SIMD to		|
|				increase speed, samples 3 points around point of interest	|
|				and interpolates normal based on this information			|
|																			|
|	Parameters:	the x and y position of the point to sample and the noise	|
|				parameters to use											|					
|																			|
|	Returns:	A normalised vectorthat describes the normal of the perlin	|
|				noise function at that point								|
|																			|
\*-------------------------------------------------------------------------*/
{

	float offs = step;
	float xtrioffs = offs * 0.86602540378;
	float ytrioffs = offs * 0.5;
	float maxamp = MaxAmplitude(n);
	Vector4 A(x, (SIMDCoherentFractalNoise(x,y+xtrioffs,n)*n.amplitude)/maxamp, y+xtrioffs, 1.0f);
	Vector4 B(x-xtrioffs, (SIMDCoherentFractalNoise(x-xtrioffs,y-ytrioffs,n)*n.amplitude)/maxamp, y-ytrioffs, 1.0f);
	Vector4 C(x+xtrioffs, (SIMDCoherentFractalNoise(x+xtrioffs,y-ytrioffs,n)*n.amplitude)/maxamp, y-ytrioffs, 1.0f);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = Cross(AC, AB);
	Normal.NormaliseSelf();
	Vector3 output(Normal);
	return output;
}



