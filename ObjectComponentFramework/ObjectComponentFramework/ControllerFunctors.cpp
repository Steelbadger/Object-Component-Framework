#include "ControllerFunctors.h"
#include "Hardware.h"
#include "GameObject.h"
#include "Camera.h"
#include "Position.h"
#include "Orientation.h"
#include "ObjectManager.h"
#include "Emitter.h"

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
	rabd::Emitter* emitter = &manager->GetComponent<rabd::Emitter>(id);
	float mulp = sensitivity*input.GetTimeForLastFrameHighResolution();

	static const XMVECTOR x = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	static const XMVECTOR y = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	static const XMVECTOR z = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

	bool moving = false;

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
		XMVECTOR translation = orientation->GetTransformedZ() * mulp;
		position->Translate(translation);
		//  if we press to move forward then turn off autorun
		autorun = false;
		moving = true;
	}
	//  If we're autorunning move forward
	if (autorun) {
		XMVECTOR translation = orientation->GetTransformedZ() * mulp;
		position->Translate(translation);
		moving = true;
	}

	//  Move backwards
	if(input.Button('S')) {
		XMVECTOR translation = orientation->GetTransformedZ() * -mulp;
		position->Translate(translation);
		//  if we press to move backwards then turn off autorun
		autorun = false;
		moving = true;
	}

	//  Middle mouse to toggle autorun
	if (input.MousePressed(Mouse::MIDDLE)) {
		autorun = !autorun;
	}


	if (input.MouseButton(Mouse::RIGHT)) {
		D3DXVECTOR2 cursorMov = input.GetMouseDeltaMovement();
		orientation->Rotate(cursorMov.x/1000.0, y);
		orientation->RotateLocal(cursorMov.y/1000.0, x);
		if (input.Button('D')) {
			XMVECTOR translation = orientation->GetTransformedX() * mulp;
			position->Translate(translation);
			moving = true;
		}
		if (input.Button('A')) {
			XMVECTOR translation = orientation->GetTransformedX() * -mulp;
			position->Translate(translation);
			moving = true;
		}
	} else {
		if (input.Button('D')) {
			orientation->Rotate(mulp/5, y);
		}
		if (input.Button('A')) {
			orientation->Rotate(-mulp/5, y);
		}
	}

	if (!moving) {
		emitter->Pause();
	} else {
		emitter->Play();
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
	static const XMVECTOR x = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);
	static const XMVECTOR y = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	static const XMVECTOR z = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);

	float time = input.GetTimeForLastFrameHighResolution();

	Orientation* orientation = &manager->GetComponent<Orientation>(id);
	orientation->Rotate(spinSpeedx*time, x);
	orientation->Rotate(spinSpeedy*time, y);
	orientation->Rotate(spinSpeedz*time, z);
}

void SpinController::SetSpinSpeed(float x, float y, float z)
{
	spinSpeedx = x;
	spinSpeedy = y;
	spinSpeedz = z;
}
