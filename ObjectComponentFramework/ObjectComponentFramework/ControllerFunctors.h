#pragma once
#include "LookupTable.h"

class ControlFunctor
{
public:
	ControlFunctor(){};
	virtual ~ControlFunctor(){};
	virtual void operator()(ObjectID affectedObject) = 0;
	virtual ControlFunctor* Copy() const = 0;
};


class FirstPersonController : public ControlFunctor
{
public:
	FirstPersonController();
	virtual ~FirstPersonController();
	virtual void operator()(ObjectID affectedObject);
	virtual ControlFunctor* Copy() const;

	void SetSensitivity(float s);

private:
	float sensitivity;
	bool autorun;
};
