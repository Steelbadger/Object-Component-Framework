#include "ControllerFunctors.h"
#include "Hardware.h"
#include "GameObject.h"
#include "Camera.h"
#include "Position.h"
#include "Orientation.h"
#include "ObjectManager.h"

FirstPersonController::FirstPersonController():
	sensitivity(5.0f), autorun(false)
{
}

FirstPersonController::~FirstPersonController()
{

}

void FirstPersonController::operator()(rabd::ObjectID id, rabd::ObjectManager* manager)
{
	HardwareState input = HardwareState::GetInstance();
	Camera* camera = &manager->GetComponent<Camera>(id);
	Position* position = &manager->GetComponent<Position>(id);
	Orientation* orientation = &manager->GetComponent<Orientation>(id);
	float mulp = sensitivity*input.GetTimeForLastFrameHighResolution();

	//  Zoom with mouse wheel
	if (input.CheckMouseWheel()) {
		camera->Zoom(input.GetMouseWheelDelta());
	}

	//  Change frustum clip depths
	if (input.Button(VK_UP)) {
		camera->SetClipPlanes(camera->GetNearClipPlane(), camera->GetFarClipPlane()+10.0f);
	}
	if (input.Button(VK_DOWN)) {
		camera->SetClipPlanes(camera->GetNearClipPlane(), camera->GetFarClipPlane()-10.0f);
	}

	//  Move forward
	if (input.Button('W')) {
		D3DXVECTOR3 translation = orientation->GetTransformedZ() * mulp;
		position->Translate(translation);
		//  if we press to move forward then turn off autorun
		autorun = false;
	}
	//  If we're autorunning move forward
	if (autorun) {
		D3DXVECTOR3 translation = orientation->GetTransformedZ() * mulp;
		position->Translate(translation);
	}

	//  Move backwards
	if(input.Button('S')) {
		D3DXVECTOR3 translation = orientation->GetTransformedZ() * -mulp;
		position->Translate(translation);
		//  if we press to move backwards then turn off autorun
		autorun = false;
	}

	//  Middle mouse to toggle autorun
	if (input.MousePressed(Mouse::MIDDLE)) {
		autorun = !autorun;
	}


	if (input.MouseButton(Mouse::RIGHT)) {
		D3DXVECTOR2 cursorMov = input.GetMouseDeltaMovement();
		orientation->Rotate(cursorMov.x/1000.0, D3DXVECTOR3(0,1,0));
		orientation->RotateLocal(cursorMov.y/1000.0, D3DXVECTOR3(1,0,0));
		if (input.Button('D')) {
			D3DXVECTOR3 translation = orientation->GetTransformedX() * mulp;
			position->Translate(translation);
		}
		if (input.Button('A')) {
			D3DXVECTOR3 translation = orientation->GetTransformedX() * -mulp;
			position->Translate(translation);
		}
	} else {
		if (input.Button('D')) {
			orientation->Rotate(mulp/5, D3DXVECTOR3(0,1,0));
		}
		if (input.Button('A')) {
			orientation->Rotate(-mulp/5, D3DXVECTOR3(0,1,0));
		}
	}
}

void FirstPersonController::SetSensitivity(float s)
{
	sensitivity = s;
}

SpinController::SpinController()
{
}

SpinController::~SpinController()
{
}

void SpinController::operator()(rabd::ObjectID id, rabd::ObjectManager* manager) 
{
	HardwareState input = HardwareState::GetInstance();

	float time = input.GetTimeForLastFrameHighResolution();

	Orientation* orientation = &manager->GetComponent<Orientation>(id);
	orientation->Rotate(spinSpeedx*time, D3DXVECTOR3(1,0,0));
	orientation->Rotate(spinSpeedy*time, D3DXVECTOR3(0,1,0));
	orientation->Rotate(spinSpeedz*time, D3DXVECTOR3(0,0,1));
}

void SpinController::SetSpinSpeed(float x, float y, float z)
{
	spinSpeedx = x;
	spinSpeedy = y;
	spinSpeedz = z;
}
