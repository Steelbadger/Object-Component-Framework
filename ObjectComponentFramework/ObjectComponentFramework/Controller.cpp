#include "Controller.h"
#include "ControllerFunctors.h"
#include "ObjectManager.h"


Controller::Controller(): update(nullptr)
{
}

Controller::~Controller()
{
}


void Controller::Update()
{
	if (update) {
		(*update)(GetParentID(), manager);
	}
}