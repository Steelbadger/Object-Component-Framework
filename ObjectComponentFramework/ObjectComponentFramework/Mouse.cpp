#include "mouse.h"

#include <WindowsX.h>

#include <stdio.h>
#include <iostream>

Mouse::Mouse(void)
{
	x = 0;
	y = 0;
	oldx = 0;
	oldy = 0;
	for (int i = 0; i < 3; i++) {
		buttons[i] = false;
	}
}


Mouse::~Mouse(void)
{
}

void Mouse::UpdatePosition(LPARAM lParam)
{
	x = GET_X_LPARAM(lParam); 
	y = GET_Y_LPARAM(lParam); 
}

void Mouse::Message(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MOUSEMOVE:
			UpdatePosition(lParam);
			break;
		case WM_MOUSEWHEEL:
			wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			wheelMoved = true;
			break;
		case WM_MBUTTONDOWN:
			buttons[MIDDLE] = true;
			break;
		case WM_MBUTTONUP:
			buttons[MIDDLE] = false;
			break;
		case WM_LBUTTONDOWN:
			buttons[LEFT] = true;
			break;	
		case WM_LBUTTONUP:
			buttons[LEFT] = false;
			break;
		case WM_RBUTTONDOWN:
			buttons[RIGHT] = true;
			break;
		case WM_RBUTTONUP:
			buttons[RIGHT] = false;
			break;
	}
}

void Mouse::Update()
{

	for (int i = 0; i < 3; i++) {
		pressed[i] = false;
		released[i] = false;
	}

	frameMoveX = x - oldx;
	frameMoveY = y - oldy;
	oldx = x;
	oldy = y;


	wheelWasMoved = wheelMoved;
	if (wheelMoved == false) {
		wheelDelta = 0;
	}
	wheelMoved = false;

	for (int i = 0; i < 3; i++) {
		if (buttons[i] == true && lastFrameButtons[i] == false) {
			pressed[i] = true;
		}
		if (buttons[i] == false && lastFrameButtons[i] == true) {
			released[i] = true;
		}
		lastFrameButtons[i] = buttons[i];
	}
}
