#include "Application.h"

#include "Orientation.h"
#include "GameObject.h"
#include "Camera.h"
#include "Controller.h"
#include "Material.h"
#include "ControllerFunctors.h"
#include "PointLight.h"

#include <functional>


void TESTFUNCTION()
{
	D3DXVECTOR3 point(300, -12.3, 400);

	D3DXVECTOR3 light(1,0,0);

	D3DXVECTOR3 eye(0,0,0);
	D3DXVECTOR3 up(0,1,0);
	D3DXVECTOR3 look(0,0,1);

	D3DXVECTOR3 topLeft(-1, 1, 1);
	D3DXVECTOR3 topRight(1, 1, 1);
	D3DXVECTOR3 bottomLeft(-1, -1, 1);
	D3DXVECTOR3 bottomRight(1, -1, 1);

	D3DXVECTOR4 result;
	D3DXMATRIX persp;
	D3DXMATRIX invPersp;
	D3DXMATRIX view;
	D3DXMATRIX invView;

	float fovy = (45 * (float)D3DX_PI)/180.0f;
	float maxDepth = 1000.0f;
	float det;
	D3DXMatrixPerspectiveFovLH(&persp, fovy, 720.0f/450.0f, 0.01, maxDepth);
	D3DXMatrixLookAtLH(&view, &eye, &look, &up);
	D3DXMatrixInverse(&invView, &det, &view);
	D3DXMatrixInverse(&invPersp, &det, &persp);
	D3DXVec3Transform(&result, &point, &view);
	result /= result.w;
	D3DXVECTOR3 jeeze = D3DXVECTOR3(result);
	float depth = result.z / maxDepth;
	D3DXVec3Transform(&result, &jeeze, &persp);

	D3DXVECTOR4 resLight;
	D3DXVec3Transform(&resLight, &light, &view);

	result /= result.w;
	result.z = depth;

	//  Into the reconstruction phase

	float u, v;
	u = (result.x+1)/2.0f;
	v = (result.y+1)/2.0f;
	
	D3DXVECTOR4 tL;
	D3DXVECTOR4 tR;
	D3DXVECTOR4 bL;
	D3DXVECTOR4 bR;

	D3DXVec3Transform(&tL, &topLeft, &invPersp);
	D3DXVec3Transform(&tR, &topRight, &invPersp);
	D3DXVec3Transform(&bL, &bottomLeft, &invPersp);
	D3DXVec3Transform(&bR, &bottomRight, &invPersp);

	tL /= tL.w;
	tR /= tR.w;
	bL /= bL.w;
	bR /= bR.w;

	tL /= tL.z;
	tR /= tR.z;
	bL /= bL.z;
	bR /= bR.z;

	topLeft = D3DXVECTOR3(tL);
	topRight = D3DXVECTOR3(tR);
	bottomLeft = D3DXVECTOR3(bL);
	bottomRight = D3DXVECTOR3(bR);

//	D3DXVec3Normalize(&topLeft, &topLeft);
//	D3DXVec3Normalize(&topRight, &topRight);
//	D3DXVec3Normalize(&bottomLeft, &bottomLeft);
//	D3DXVec3Normalize(&bottomRight, &bottomRight);

	D3DXVECTOR3 u1;
	D3DXVECTOR3 u2;
	D3DXVECTOR3 dir;

	D3DXVec3Lerp(&u1, &topLeft, &topRight, u);
	D3DXVec3Lerp(&u2, &bottomLeft, &bottomRight, u);
	D3DXVec3Lerp(&dir, &u2, &u1, v);
	
//	D3DXVec3Normalize(&dir, &dir);

	float distance = result.z * maxDepth;

	D3DXVECTOR3 final = dir * distance;

	D3DXVECTOR4 back;
	D3DXVec3Transform(&back, &final, &invView);
}

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

bool Application::Initialize()
{
	Mesh m;
	Controller c;
	Material ma;



	bool result;
	running = true;

//	TESTFUNCTION();

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
	GameObject::AddComponent<Controller>(camera);

	FirstPersonController cont;

	GameObject::GetComponent<Position>(camera).SetPosition(0,0,0);
	GameObject::GetComponent<Camera>(camera).Initialise(true, 45, window.GetWidth(), window.GetHeight(), 0.1f, 1000.0f);
	GameObject::GetComponent<Controller>(camera).SetControlFunction(cont);



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
	std::list<ObjectID>::iterator it;
	for (it = world.GetUpdateList().begin(); it != world.GetUpdateList().end(); it++) {
		GameObject::GetComponent<Controller>(*it).Update();

	}


	if (m_Input->Pressed(VK_RETURN)) {
		std::cout << "FrameRate: " << (1/m_Input->GetTimeForLastFrameHighResolution()) << std::endl;
		std::cout << "HighResTimer: " << m_Input->GetTimeForLastFrameHighResolution() << std::endl;
	}

	if (m_Input->Pressed('L')) {
		std::cout << "Current Lights in Scene: " << world.GetLightList().size() << std::endl;
	}

	if (m_Input->Pressed('K')) {
		ObjectID light3 = GameObject::New();
		GameObject::AddComponent<Position>(light3);
		GameObject::AddComponent<Orientation>(light3);
		GameObject::AddComponent<PointLight>(light3);
		GameObject::GetComponent<PointLight>(light3).SetColour(float(rand()%100)/100.0f, float(rand()%100)/100.0f, float(rand()%100)/100.0f, 1.0f);
		GameObject::GetComponent<PointLight>(light3).SetSpecularPower(100.0f);
		GameObject::GetComponent<Position>(light3).SetPosition(rand()%200-100,0.5,rand()%200-100);
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