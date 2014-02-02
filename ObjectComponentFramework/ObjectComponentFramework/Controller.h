#pragma once

#include "Component.h"

#include <memory>


class ControlFunctor;

class Controller : public Component<Controller>
{
public:
	Controller();
	Controller(Controller&& orig);
	~Controller();

	void Update();
	template<class T>
	void SetControlFunction(T &updateFunctor) {
		update = std::unique_ptr<ControlFunctor>(new T(updateFunctor));
	}
	Controller& operator=(Controller&& orig);
private:
	std::unique_ptr<ControlFunctor> update;
};