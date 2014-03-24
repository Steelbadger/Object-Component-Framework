/*
	File:		HeightmapBuilder.h
	Version:	1.0
	Date:		20th March 2014
	Authors:	Ross Davies

	Namespace:	rabd
	Exposes:	HeightmapBuilder

	Description:
	The HeightmapBuilder class is a helper class for building DirectX heightmap textures.
*/
#pragma once

#include "noisegenerator.h"


class HeightmapBuilder
{
public:
	HeightmapBuilder();
	~HeightmapBuilder();

	void GenHeightsSIMD(float x, float y, NoiseObject n, float square);
	unsigned short* GenerateHeightField(float x, float y, NoiseObject n, float square);
	static unsigned short* GenerateHeights(float x, float y, NoiseObject n, float square);
	void GenerateHeightmapAsync(unsigned short** output, float x, float y, NoiseObject n, float square);

private:
	static unsigned short* GenerateHeights(float x, float y, NoiseObject n, float square);
	void write_tga(const char *filename, int size, unsigned char* base);
	void write_tga(const char *filename, int size, unsigned short* base);
	static unsigned __stdcall GenerateSection(void *data);
	static const int size = 1024;
};

