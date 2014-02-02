#include "Controller.h"
#include "ControllerFunctors.h"



Controller::Controller(): update(nullptr)
{
}

Controller::Controller(Controller&& orig)
{
	update = std::move(orig.update);
	SetParentID(orig.GetParentID());
	SetLookup(orig.GetID());
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

Controller& Controller::operator=(Controller&& orig)
{
	update = std::unique_ptr<ControlFunctor>(std::move(orig.update));
	SetParentID(orig.GetParentID());
	SetLookup(orig.GetID());
		return (*this);
}
