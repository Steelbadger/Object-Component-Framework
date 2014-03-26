#include "Application.h"

#include "Orientation.h"
#include "GameObject.h"
#include "Camera.h"
#include "Controller.h"
#include "Material.h"
#include "ControllerFunctors.h"
#include "PointLight.h"
#include "Manager.h"

#include "HeightmapBuilder.h"
#include "UtilityFunctions.h"
#include "AllanMilne\XACore.hpp"
#include "Emitter.h"
#include <functional>

#include "simdMatrix.h"
#include "simdVector4.h"

Application::Application(): window(this)
{
	m_Input = 0;
	m_Graphics = 0;
	fullscreen = false;
	vSyncEnabled = false;
	deferred = true;
	wireframe = false;
}

Application::~Application()
{
}

void TESTFUNCTION()
{
	const int tests = 1000;

	//D3DXMATRIX in1[tests];
	//D3DXMATRIX in2 = D3DXMATRIX(rand(), rand(), rand(), rand(),
	//							rand(), rand(), rand(), rand(),
	//							rand(), rand(), rand(), rand(), 
	//							rand(), rand(), rand(), rand());
	//D3DXMATRIX out[tests];

	//for (int i = 0; i < tests; i++) {
	//	in1[i] = D3DXMATRIX(rand(), rand(), rand(), rand(),
	//							rand(), rand(), rand(), rand(),
	//							rand(), rand(), rand(), rand(), 
	//							rand(), rand(), rand(), rand());
	//}

	//std::cout << "Do DirectX Vector Multiplies..." << std::endl;
	//HardwareState::GetInstance().Update();
	//for (int i = 0; i < tests; i++) {
	//	D3DXMatrixMultiply(&out[i], &in1[i], &in2);
	//}
	//HardwareState::GetInstance().Update();
	//std::cout << "Time: " << HardwareState::GetInstance().GetTimeForLastFrameHighResolution() << std::endl;

	//SIMD::Matrix4x4 in3[tests];
	//SIMD::Matrix4x4 in4 (	rand(), rand(), rand(), rand(),
	//						rand(), rand(), rand(), rand(),
	//						rand(), rand(), rand(), rand(), 
	//						rand(), rand(), rand(), rand());
	//SIMD::Matrix4x4 out2[tests];

	//for (int i = 0; i < tests; i++) {
	//	in3[i] = SIMD::Matrix4x4 (	rand(), rand(), rand(), rand(),
	//						rand(), rand(), rand(), rand(),
	//						rand(), rand(), rand(), rand(), 
	//						rand(), rand(), rand(), rand());
	//}

	//std::cout << "Do SIMD Vector Multiplies..." << std::endl;
	//HardwareState::GetInstance().Update();
	//for (int i = 0; i < tests; i++) {
	//	out2[i] = in3[i] * in4;
	//}
	//HardwareState::GetInstance().Update();
	//std::cout << "Time: " << HardwareState::GetInstance().GetTimeForLastFrameHighResolution() << std::endl;


	D3DXVECTOR4 in1[tests];
	D3DXMATRIX in2 = D3DXMATRIX(rand(), rand(), rand(), rand(),
								rand(), rand(), rand(), rand(),
								rand(), rand(), rand(), rand(), 
								rand(), rand(), rand(), rand());
	D3DXVECTOR4 out[tests];

	for (int i = 0; i < tests; i++) {
		in1[i] = D3DXVECTOR4(rand(), rand(), rand(), rand());
	}

	std::cout << "Do DirectX Vector Multiplies..." << std::endl;
	HardwareState::GetInstance().Update();
	for (int i = 0; i < tests; i++) {
		D3DXVec4Transform(&out[i], &in1[i], &in2);
	}
	HardwareState::GetInstance().Update();
	std::cout << "Time: " << HardwareState::GetInstance().GetTimeForLastFrameHighResolution() << std::endl;

	SIMD::Vector4 in3[tests];
	SIMD::Matrix4x4 in4 (	rand(), rand(), rand(), rand(),
							rand(), rand(), rand(), rand(),
							rand(), rand(), rand(), rand(), 
							rand(), rand(), rand(), rand());
	SIMD::Vector4 out2[tests];

	for (int i = 0; i < tests; i++) {
		in3[i] = SIMD::Vector4 (	rand(), rand(), rand(), rand());
	}

	std::cout << "Do SIMD Vector Multiplies..." << std::endl;
	HardwareState::GetInstance().Update();
	for (int i = 0; i < tests; i++) {
		out2[i] = in3[i] * in4;
	}
	HardwareState::GetInstance().Update();
	std::cout << "Time: " << HardwareState::GetInstance().GetTimeForLastFrameHighResolution() << std::endl;


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
	world.PassObjectManager(&manager);
	world.PassTextureManager(&textureManager);
	//////////////////////////////IMPORTANT////////////////////////////////
	rabd::TextureBase::SetDevice(m_D3D.GetDevice());
	rabd::TextureBase::SetManager(&textureManager);
	AllanMilne::Audio::XACore::CreateInstance();
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

	TESTFUNCTION();

	manager.RegisterType<rabd::GameObject>();
	manager.RegisterType<Position>();
	manager.RegisterType<Orientation>();
	manager.RegisterType<Camera>();
	manager.RegisterType<Controller>();
	manager.RegisterType<PointLight>();
	manager.RegisterType<Transformation>();
	manager.RegisterType<rabd::Emitter>();

	world.CreateScene();

	rabd::ObjectID camera = manager.CreateObjectAndComponents<Position, Orientation, Camera, Controller,
																PointLight, Transformation, rabd::Emitter>();

	FirstPersonController cont;
	cont.SetSensitivity(50.0f);

	manager.GetComponent<Position>(camera).SetPosition(0,80,-100);
	manager.GetComponent<Camera>(camera).Initialise(true, 45, window.GetWidth(), window.GetHeight(), 0.1f, 1000.0f);
	manager.GetComponent<Controller>(camera).SetControlFunction(cont);
	manager.GetComponent<PointLight>(camera).SetColour(1.0f, 1.0f, 1.0f, 1.0f);
	manager.GetComponent<PointLight>(camera).SetSpecularPower(100.0f);
	manager.GetComponent<rabd::Emitter>(camera).LoadFile("Phyre1.wav");
	
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

	AllanMilne::Audio::XACore::DeleteInstance();

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
	std::list<rabd::ObjectID>::iterator it;
	for (it = world.GetUpdateList().begin(); it != world.GetUpdateList().end(); it++) {
		manager.GetComponent<Controller>(*it).Update();
	}


	if (m_Input->Pressed(VK_RETURN)) {
		std::cout << "FrameRate: " << (1/m_Input->GetTimeForLastFrameHighResolution()) << std::endl;
		std::cout << "HighResTimer: " << m_Input->GetTimeForLastFrameHighResolution() << std::endl;
	}

	if (m_Input->Pressed('L')) {
		std::cout << "Current Lights in Scene: " << world.GetLightList().size() << std::endl;
	}

	if (m_Input->Pressed('K')) {
		rabd::ObjectID light3 = manager.CreateObjectAndComponents<Position, Orientation, Transformation, PointLight>();
		manager.GetComponent<PointLight>(light3).SetColour(float(rand()%100)/100.0f, float(rand()%100)/100.0f, float(rand()%100)/100.0f, 1.0f);
		manager.GetComponent<PointLight>(light3).SetSpecularPower(100.0f);
		manager.GetComponent<Position>(light3).SetPosition(rand()%200-100,0.5,rand()%200-100);
		world.AddToScene(light3);
		std::cout << "Current Lights in Scene: " << world.GetLightList().size() << std::endl;
	}

	if (m_Input->Pressed('P')) {
		deferred = !deferred;
		if (deferred == true) {
			std::cout << "Now Rendering Using Deferred Shading" << std::endl;
		} else {
			std::cout << "Now Rendering Using Forward Shading" << std::endl;
		}
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