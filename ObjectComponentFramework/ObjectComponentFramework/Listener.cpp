#include "Listener.h"
#include "ObjectManager.h"
#include "Position.h"

namespace rabd
{
	Listener::Listener()
	{}

	Listener::~Listener()
	{}

	void Listener::SetAsListener()
	{
		if (manager->HasComponent<Position>(GetParentID())) {
			//  Pass position into Audiomanager as vector3
		}
	}

};