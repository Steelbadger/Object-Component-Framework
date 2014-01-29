#pragma once


// PRE-PROCESSING DIRECTIVES //
#define WIN32_LEAN_AND_MEAN

// INCLUDES //
#include <windows.h>
// MY CLASS INCLUDES //
#include "Hardware.h"
#include "GraphicsWindow.h"
#include "graphicsclass.h"
#include "Position.h"
#include "World.h"
#include "MeshFactory.h"
#include "d3dclass.h"

// Class name: Application
class Application
{
public:
	Application();
	~Application();

	bool Initialize();
	void Shutdown();
	void Run();

	void MessageHandler(Window* window, UINT message, WPARAM wParam, LPARAM lParam);
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	void TestUpdate();

private:
	LPSTR m_applicationName;
	HINSTANCE m_hinstance;

	HardwareState* m_Input;
	GraphicsClass* m_Graphics;
	D3DClass m_D3D;
	Window window;
	World world;
	MeshFactory factory;

	bool fullscreen, vSyncEnabled;
	bool deferred;
	bool wireframe;

	bool running;
};