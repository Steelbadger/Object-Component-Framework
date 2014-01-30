#include "Controller.h"
#include "ControllerFunctors.h"



Controller::Controller(): update(nullptr)
{
}

Controller::Controller(const Controller& copy)
{
	if (copy.update) {
		update.reset((ControlFunctor*)copy.update->Copy());
	}
}

Controller::~Controller()
{
}


void Controller::Update()
{
	if (update) {
		(*update)(GetParentID());
	}
}

Controller& Controller::operator=(const Controller& rhs)
{
	if (rhs.update) {
		update.reset((ControlFunctor*)rhs.update->Copy());
	}
	return (*this);
}


