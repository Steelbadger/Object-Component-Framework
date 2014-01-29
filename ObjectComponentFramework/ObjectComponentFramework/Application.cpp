#include "Application.h"

#include "Orientation.h"
#include "GameObject.h"
#include "Camera.h"
#include "FirstPersonController.h"
#include "Material.h"
#include "SpinController.h"
#include "PointLight.h"

#include <functional>

Application::Application(): window(this)
{
	m_Input = 0;
	m_Graphics = 0;
	fullscreen = false;
	vSyncEnabled = false;
	deferred = false;
	wireframe = false;
}

Application::~Application()
{
}

bool Application::Initialize()
{
	bool result;
	running = true;

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = &HardwareState::GetInstance();

	//  Before creating the Windows window, tell the container what Message Handler function to use.
	window.SetMessageHandler<Application>((*this), std::mem_fn(&Application::MessageHandler));



	window.Create("Engine", 720, 450, m_hinstance, fullscreen);


	// Initialize the Direct3D object.
	result = m_D3D.Initialize(window.GetWidth(), window.GetHeight(), vSyncEnabled, window.GetHandleToWindow(), fullscreen, 1000.0f, 0.1f);
	if(!result)
	{
		MessageBox(window.GetHandleToWindow(), "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}
	factory.SetDevice(m_D3D.GetDevice());
	world.PassMeshFactory(&factory);
	//////////////////////////////IMPORTANT////////////////////////////////
	TextureBase::SetDevice(m_D3D.GetDevice());
	///////////////////////////////////////////////////////////////////////

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(window.GetHandleToWindow(), &m_D3D);
	if(!result)
	{
		return false;
	}
	world.CreateScene();

	ObjectID camera = GameObject::New();
	GameObject::AddComponent<Position>(camera);
	GameObject::AddComponent<Orientation>(camera);
	GameObject::AddComponent<Camera>(camera);
	GameObject::AddComponent<FirstPersonController>(camera);
//	GameObject::AddComponent<PointLight>(camera);

	GameObject::GetComponent<Position>(camera).SetPosition(0,0,0);
	GameObject::GetComponent<Camera>(camera).Initialise(true, 45, window.GetWidth(), window.GetHeight(), 1.0f, 100.0f);
	GameObject::GetComponent<FirstPersonController>(camera).SetSensitivity(5.0f);
//	GameObject::GetComponent<PointLight>(camera).SetColour(1.0f, 0.0f, 0.0f, 1.0f);
//	GameObject::GetComponent<PointLight>(camera).SetSpecularPower(100);

	world.SetCameraObject(camera);
	return true;
}

void Application::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	return;
}

void Application::Run()
{
	MSG msg;
	bool result;
	m_Input->Update();


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	while(running)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			// Otherwise do the frame processing.
			if (m_Input->Pressed(VK_ESCAPE)) {
				running = false;
			} else {
				TestUpdate();
				result = Frame();
				if(!result)
				{
					running = false;
				}
			}
		}
		
	}

	return;

}

void Application::TestUpdate()
{
	m_Input->Update();

	float timestep = m_Input->GetTimeForLastFrameHighResolution();
	for (int i = 0; i < FirstPersonController::GetList().Size(); i++) {
		if (FirstPersonController::GetList().Exists(i)) {
			FirstPersonController::GetList().Get(i).Update();
		}
	}

	for (int i = 0; i < SpinController::GetList().Size(); i++) {
		if (SpinController::GetList().Exists(i)) {
			SpinController::GetList().Get(i).Update(timestep);
		}
	}

	if (m_Input->Pressed(VK_RETURN)) {
		std::cout << "FrameRate: " << (1/m_Input->GetTimeForLastFrameHighResolution()) << std::endl;
		std::cout << "HighResTimer: " << m_Input->GetTimeForLastFrameHighResolution() << std::endl;
	}

	if (m_Input->Pressed('P')) {
		deferred = !deferred;
		if (deferred == true) {
			std::cout << "Now Rendering Using Deferred Shading" << std::endl;
		} else {
			std::cout << "Now Rendering Using Forward Shading" << std::endl;
		}
	}

	if (m_Input->Pressed('N')) {
		world.TESTDELETE();
	}


	if (m_Input->Pressed('M')) {
		wireframe = !wireframe;
		if (wireframe) {
			m_D3D.EnableWireframe();
		} else {
			m_D3D.DisableWireframe();
		}
	}

	if (m_Input->MouseButton(Mouse::RIGHT)) {
		window.SetCursorToCentre();
		int fullscreen;
		m_D3D.GetSwapChain()->GetFullscreenState(&fullscreen, NULL);
		if (fullscreen)
			window.SetMouseLockedCentreFullscreen();
		else 
			window.SetMouseLockedCentreWindow();
	}

}

bool Application::Frame()
{
	bool result;

	result = m_Graphics->Frame(world, deferred);
	if(!result)
	{
		return false;
	}

	return true;
}

void Application::MessageHandler(Window* window, UINT message, WPARAM wParam, LPARAM lParam)
{
        switch (message)
        {
			case WM_SIZE:
				window->OnResize();
			case WM_MOVE:
				window->OnMove();
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
				HardwareState::GetInstance().Message(message, wParam, lParam);
				break;
            case WM_DESTROY:
				running = false;
                PostQuitMessage(0);
				break;
        }
}