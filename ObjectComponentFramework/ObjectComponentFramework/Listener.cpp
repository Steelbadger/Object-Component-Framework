#include "Listener.h"
#include "ObjectManager.h"
#include "Transformation.h"
#include "Orientation.h"
#include "Position.h"
#include "AllanMilne\XACore.hpp"

namespace rabd
{
	Listener::Listener()
	{
		ZeroMemory(&listenerData, sizeof(listenerData));
	}

	Listener::~Listener()
	{}

	void Listener::Update()
	{
		ZeroMemory(&listenerData, sizeof(listenerData));
		listenerData.Position = manager->GetComponent<Position>(GetParentID()).GetPosition();
		listenerData.OrientFront = manager->GetComponent<Orientation>(GetParentID()).GetTransformedZ();
		listenerData.OrientTop = manager->GetComponent<Orientation>(GetParentID()).GetTransformedY();
	}
};