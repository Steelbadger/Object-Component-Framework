#pragma once

#include "Component.h"


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
		if (update != nullptr) {
			delete update;
			update = nullptr;
		}
		update = new T(updateFunctor);
	}
	Controller& operator=(const Controller& rhs);
private:
	ControlFunctor* update;
};