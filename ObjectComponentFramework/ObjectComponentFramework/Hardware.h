#pragma once
#include "Mouse.h"
#include <D3DX10math.h>
#include <windows.h>
#include <time.h>

class HardwareState
{
public:

	static HardwareState& GetInstance();
	~HardwareState(void);

	void Message(UINT, WPARAM, LPARAM);

	void Update();

	D3DXVECTOR2 GetMousePosition() {return mouse.Position();}
	D3DXVECTOR2 GetMouseDeltaMovement() {return mouse.FrameDeltaMovement();}

	bool MouseButton(Mouse::Buttons b) {return mouse.Button(b);}
	bool MousePressed(Mouse::Buttons b) {return mouse.Pressed(b);}
	bool MouseReleased(Mouse::Buttons b) {return mouse.Released(b);}
	bool Button(char key) {return keys[key];}
	bool Pressed(char key) {return pressed[key];}
	bool Released(char key) {return released[key];}
	bool Button(int key) {return keys[key];}
	bool Pressed(int key) {return pressed[key];}
	bool Released(int key) {return released[key];}

	void SetMouseLocked(int x, int y){mouse.SetLocked(x, y);}
	void SetMouseUnlock(){mouse.Unlock();}

	bool CheckMouseWheel(){return mouse.CheckWheelMoved();}
	int GetMouseWheelDelta(){return mouse.GetWheelDelta();}

	double GetTimeForLastFrame() {return timeForLastFrame/CLOCKS_PER_SEC;}
	double GetTimeSinceLastFrame();
	double GetTimeForLastFrameHighResolution(){return highResTimer;}

	void EnableAveragedFrameRate(int averageLength = 60){enableFrameRate = true; loopMax = averageLength;}
	void DisableAveragedFrameRate(){enableFrameRate = false;}
	double Framerate(){return (1/GetTimeForLastFrame());}

	int GetScreenWidth(){return screenWidth;}
	int GetScreenHeight(){return screenHeight;}

private:

	HardwareState(void);

	bool keys[256];
	bool pressed[256];
	bool released[256];
	bool oldkey[256];

	double timeForLastFrame;
	double lastTime;
	double currentTime;
	bool enableFrameRate;
	int loopCount;
	int loopMax;

	long long int frameTimeOld;
	double highResTimer;

	Mouse mouse;

	void KeyDown(UINT);
	void KeyUp(UINT);

	static HardwareState* instance;

	int screenWidth;
	int screenHeight;
};