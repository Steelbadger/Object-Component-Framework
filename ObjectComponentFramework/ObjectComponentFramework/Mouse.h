#pragma once
#include <D3D11.h>
#include <D3DX10math.h>

#include <windows.h>

class Mouse
{
public:
	enum Buttons {LEFT = 0, RIGHT = 1, MIDDLE = 2};

	Mouse(void);
	~Mouse(void);

	bool Button(Buttons button){return buttons[button];}
	bool Pressed(Buttons button){return pressed[button];}
	bool Released(Buttons button){return released[button];}

	void Message(UINT, WPARAM, LPARAM);
	void Update();

	D3DXVECTOR2 FrameDeltaMovement(){return D3DXVECTOR2(frameMoveX, frameMoveY);}
	D3DXVECTOR2 Position(){return D3DXVECTOR2(x,y);}

	void SetLocked(int xl, int yl){oldx = xl; oldy = yl;}
	void Unlock(){locked = false;}
	int GetWheelDelta(){return wheelDelta;}
	bool CheckWheelMoved(){return wheelWasMoved;}

private:
	int x, y;
	int oldx, oldy;
	int frameMoveX, frameMoveY;
	int Lockedx, Lockedy;
	bool locked;
	bool buttons[3];
	bool lastFrameButtons[3];
	bool pressed[3];
	bool released[3];

	bool wheelMoved;
	bool wheelWasMoved;
	int wheelDelta;


	void UpdatePosition(LPARAM);
};