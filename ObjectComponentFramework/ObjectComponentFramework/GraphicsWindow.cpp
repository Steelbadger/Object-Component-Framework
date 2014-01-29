#include "GraphicsWindow.h"
#include "Hardware.h"
#include "Application.h"

#include <mmsystem.h>
#include <stdio.h>

std::map<HWND, Window*> Window::WindowMap;

Window::Window(Application* p): parent(p)
{
}

Window::~Window(void)
{
	UnregisterClass(wcex.lpszClassName, wcex.hInstance);// Free the window class

	WindowMap.erase(handleToWindow);
	if (messageHandler) {
		delete messageHandler;
	}
	PostQuitMessage(0);		// Post a QUIT message to the window
}

void Window::SetCursorToCentre()
{
	SetCursorPos(gWidth/2+windowRect.left,gHeight/2+windowRect.top);
}

void Window::SetMouseLockedCentreWindow()
{
	HardwareState::GetInstance().SetMouseLocked((gWidth/2)-1,(gHeight/2)-23);
}

void Window::SetMouseLockedCentreFullscreen()
{
	HardwareState::GetInstance().SetMouseLocked((gWidth/2),(gHeight/2));
}

void Window::OnResize()
{
	GetClientRect(handleToWindow, &graphicsRect);
	GetWindowRect(handleToWindow, &windowRect);

	gHeight = graphicsRect.bottom;
	gWidth = graphicsRect.right;
}

void Window::OnMove()
{
	GetWindowRect(handleToWindow, &windowRect);
}


void Window::Create(LPSTR strWindowName, int width, int height, HINSTANCE hInstance, bool bFullScreen, DWORD dwStyle)
{

	DEVMODE dmScreenSettings;

	memset(&wcex, 0, sizeof(WNDCLASS));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;		
	wcex.lpfnWndProc	= &Window::WndProc;		
	wcex.hInstance		= hInstance;						
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);		
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= strWindowName;	

	
	RegisterClass(&wcex);// Register the class

	//Set the Client area of the window to be our resolution.
	RECT glwindow;
	glwindow.left		= 0;		
	glwindow.right		= width;	
	glwindow.top		= 0;		
	glwindow.bottom		= height;	

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.

	AdjustWindowRect( &glwindow, dwStyle, false);

	//Create the window
	handleToWindow = CreateWindowEx(NULL,
							strWindowName, 
							strWindowName, 
							dwStyle, 
							0, 
							0,
							glwindow.right  - glwindow.left,
							glwindow.bottom - glwindow.top, 
							NULL,
							NULL,
							hInstance,
							NULL
							);
	if(!handleToWindow) {
		MessageBox(NULL, "Could Not Get Handle To Window", "Error", MB_OK); // If we couldn't get a handle, return NULL
		PostQuitMessage (0);
	}
	WindowMap[handleToWindow] = this;

	ShowWindow(handleToWindow, SW_SHOWNORMAL);

	UpdateWindow(handleToWindow);					
	SetFocus(handleToWindow);

	OnResize();
	titleCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	borderWidth = GetSystemMetrics(SM_CXFIXEDFRAME);
}

void Window::CreateMessageWindow(LPSTR strWindowName, HINSTANCE hInstance)
{
	memset(&wcex, 0, sizeof(WNDCLASS));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;		
	wcex.lpfnWndProc	= &Window::WndProc;		
	wcex.hInstance		= hInstance;						
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);		
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= strWindowName;	

	
	RegisterClass(&wcex);// Register the class

	//Set the Client area of the window to be our resolution.

	// Create the window with the screen settings and get the handle to it.
	handleToWindow = CreateWindowEx(WS_EX_APPWINDOW,
									strWindowName,
									strWindowName, 
									0,
									CW_USEDEFAULT, CW_USEDEFAULT,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									HWND_MESSAGE,
									NULL,
									hInstance,
									NULL								);

	if(!handleToWindow) {
		MessageBox(NULL, "Could Not Get Handle To Window", "Error", MB_OK); // If we couldn't get a handle, return NULL
		PostQuitMessage (0);
	}
	WindowMap[handleToWindow] = this;
}

Window* Window::GetWindowReference(HWND hwnd)
{
	return WindowMap[hwnd];
}


LRESULT CALLBACK Window::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_CREATE)
    {
//		HardwareState::GetInstance().SetScreenSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }
    else
    {
		Window* pWindow = WindowMap[hwnd];
		if (pWindow != nullptr){		
			switch (message)
			{
				case WM_PAINT:
					break;
				case WM_COMMAND:
					break;
				case WM_SIZE:
				case WM_MOVE:
				case WM_LBUTTONUP:
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
				case WM_MOUSEWHEEL:
				case WM_MOUSEMOVE:
				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_DESTROY:
					(*pWindow->messageHandler)(pWindow, message, wParam, lParam);
					return 0;
			}
		}
    }

    return DefWindowProc (hwnd, message, wParam, lParam) ;
}
