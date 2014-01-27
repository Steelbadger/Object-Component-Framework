#pragma once


#include "Component.h"
#include "ControllerUpdateInterface.h"

class FirstPersonController : public Component<FirstPersonController>
{
public:
	FirstPersonController();
	~FirstPersonController();

	void Update();

	void SetSensitivity(float sensitivity);

private:
	float sensitivity;
	bool autorun;
};