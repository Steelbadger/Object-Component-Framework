#pragma once
#include <Windows.h>
#include <map>
#include <functional>
#include "MessagePasser.h"

class Application;

class Window
{
public:
	Window(Application* parent);
	void Create(LPSTR strWindowName, int width, int height, HINSTANCE hInstance, bool bFullScreen = false, DWORD dwStyle = (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP));
//	void CreateFullScreen(LPSTR strWindowName, HINSTANCE hInstance);
//	void CreateWindowed(LPSTR strWindowName, int width, int height, HINSTANCE hInstance);
	void CreateMessageWindow(LPSTR strWindowName, HINSTANCE hInstance);

	template<class T> void SetMessageHandler(T& t, std::function<void (T&, Window* wind, UINT Msg, WPARAM wParam, LPARAM lParam)> func) {
		messageHandler = new MessagePasser<T>(t, func);
	}

	~Window(void);

	void OnResize();
	void OnMove();

	int GetWidth(){return graphicsRect.right;}
	int GetHeight(){return graphicsRect.bottom;}
	int GetGraphicsYOffset(){return borderWidth+titleCaptionHeight;}
	int GetGraphicsXOffset(){return borderWidth;}
	int GetX(){return windowRect.left;};
	int GetY(){return windowRect.top;}

	void SetCursorToCentre();
	void SetMouseLockedCentreWindow();
	void SetMouseLockedCentreFullscreen();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static Window* GetWindowReference(HWND hWnd);

	HWND GetHandleToWindow(){return handleToWindow;}
private:

	HWND handleToWindow;
	WNDCLASS wcex;
	RECT graphicsRect;
	RECT windowRect;

	Application* parent;
	MessagePasserInterface* messageHandler;

	int gWidth, gHeight;
	int borderWidth;
	int titleCaptionHeight;
	static std::map<HWND,Window*>WindowMap;



	static const int COLOUR_DEPTH = 16;
};

