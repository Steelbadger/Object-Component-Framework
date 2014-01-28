#include "Controller.h"
#include "ControllerFunctors.h"



Controller::Controller(): update(nullptr)
{
}

Controller::Controller(const Controller& copy)
{
	update = nullptr;
	if (copy.update != nullptr) {
		update = copy.update->Copy();
	}
}

Controller::~Controller()
{
	if (update != nullptr) {
		delete update;
	}
}


void Controller::Update()
{
	(*update)(GetParentID());
}

Controller& Controller::operator=(const Controller& rhs)
{
	if (update != nullptr) {
//		delete update;
	}
	if (rhs.update != nullptr) {
		update = rhs.update->Copy();
	}
	return (*this);
}


