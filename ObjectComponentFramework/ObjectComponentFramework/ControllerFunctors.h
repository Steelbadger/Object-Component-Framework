#pragma once
#include "LookupTable.h"

class ControlFunctor
{
public:
	ControlFunctor(){};
	virtual ~ControlFunctor(){};
	virtual void operator()(ObjectID affectedObject) = 0;
};


class FirstPersonController : public ControlFunctor
{
public:
	FirstPersonController();
	virtual ~FirstPersonController();
	virtual void operator()(ObjectID affectedObject);

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
	virtual void operator()(ObjectID affectedObject);

	void SetSpinSpeed(float x, float y, float z);

private:
	float spinSpeedy;
	float spinSpeedz;
	float spinSpeedx;
};

