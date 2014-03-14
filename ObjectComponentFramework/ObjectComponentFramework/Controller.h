#pragma once

#include "Component.h"

#include <memory>


class ControlFunctor;

class Controller : public rabd::Component<Controller>
{
public:
	Controller();
	~Controller();

	void Update();
	template<class T>
	void SetControlFunction(T &updateFunctor) {
		update = new T(updateFunctor);
	}

private:
//	std::unique_ptr<ControlFunctor> update;
	ControlFunctor* update;
};