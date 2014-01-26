#include "Hardware.h"
#include "mouse.h"
#include <windows.h>


HardwareState& HardwareState::GetInstance()
{
	if (instance == NULL) {
		instance = new HardwareState();
	}
	return (*instance);
}

HardwareState* HardwareState::instance;

HardwareState::HardwareState(void)
{
	for (int i = 255; i >= 0; i--)
	{
		oldkey[i] = false;
		keys[i] = false;
		pressed[i] = false;
		released[i] = false;
	}
	timeForLastFrame = 0;
	lastTime = clock();
	currentTime = clock();
	screenWidth =  GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	loopCount = 0;
}


HardwareState::~HardwareState(void)
{
}

void HardwareState::Message(UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_KEYDOWN:
			KeyDown(wParam);
			break;
		case WM_KEYUP:
			KeyUp(wParam);
			break;
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			mouse.Message(message, wParam, lParam);
			break;
	}

}

void HardwareState::KeyDown(UINT wParam)
{
	oldkey[wParam] = keys[wParam];
	keys[wParam]=true;
}

void HardwareState::KeyUp(UINT wParam)
{
	oldkey[wParam] = keys[wParam];
	keys[wParam]=false;	
}

void HardwareState::Update()
{

	if (enableFrameRate) {
		loopCount++;
		if (loopCount > loopMax) {
			lastTime = currentTime;
			currentTime = clock();
			timeForLastFrame = currentTime - lastTime;
			timeForLastFrame /= loopCount;
			loopCount = 0;
		}
	} else {
		lastTime = currentTime;
		currentTime = clock();
		timeForLastFrame = currentTime - lastTime;
	}

	LARGE_INTEGER currentTime, frequency;
	long long int tickCount, countsPerSecond;
	QueryPerformanceCounter(&currentTime);
	QueryPerformanceFrequency(&frequency);
	tickCount = currentTime.QuadPart-frameTimeOld;
	frameTimeOld = currentTime.QuadPart;
	countsPerSecond = frequency.QuadPart;

	if(tickCount < 0.0f) {
		tickCount = 0.0f;
	}


	highResTimer = double(tickCount)/countsPerSecond;

	for (int i = 256; i > 0; i--)
	{
		if (keys[i-1] == true && oldkey[i-1] == false)
			pressed[i-1] = true;
		else
			pressed[i-1] = false;

		if (keys[i-1] == false && oldkey[i-1] == true)
			released[i-1] = true;
		else
			released[i-1] = false;
	}
	memcpy(oldkey, keys, sizeof(keys));
	mouse.Update();
}

double HardwareState::GetTimeSinceLastFrame()
{
	return clock()-currentTime;
}