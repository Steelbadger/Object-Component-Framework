#pragma once

#include "Component.h"

#include <memory>


class ControlFunctor;

class Controller : public Component<Controller>
{
public:
	Controller();
	Controller(const Controller& copy);
	~Controller();

	void Update();
	template<class T>
	void SetControlFunction(const T &updateFunctor) {
		Controller* thing = this;
		update.reset((ControlFunctor*)updateFunctor.Copy());
	}
	Controller& operator=(const Controller& rhs);
private:
	std::unique_ptr<ControlFunctor> update;
};