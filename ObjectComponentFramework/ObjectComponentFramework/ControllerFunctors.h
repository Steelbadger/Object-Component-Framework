#pragma once
#include "LookupTable.h"
#include "GameObject.h"

class rabd::ObjectManager;

class ControlFunctor
{
public:
	ControlFunctor(){};
	virtual ~ControlFunctor(){};
	virtual void operator()(rabd::ObjectID id, rabd::ObjectManager* manager) = 0;
};


class FirstPersonController : public ControlFunctor
{
public:
	FirstPersonController();
	virtual ~FirstPersonController();
	virtual void operator()(rabd::ObjectID id, rabd::ObjectManager* manager);

	void SetSensitivity(float s);

private:
	float sensitivity;
	bool autorun;
};

class SpinController : public ControlFunctor
{
public:
	SpinController();
	virtual ~SpinController();
	virtual void operator()(rabd::ObjectID id, rabd::ObjectManager* manager);

	void SetSpinSpeed(float x, float y, float z);

private:
	float spinSpeedy;
	float spinSpeedz;
	float spinSpeedx;
};

