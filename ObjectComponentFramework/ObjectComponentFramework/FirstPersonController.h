#pragma once


#include "Component.h"

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