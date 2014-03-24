#pragma once

#include "Component.h"
#include "Vector3.h"

namespace rabd
{
	class Listener : public Component<Listener>
	{
	public:
		Listener();
		~Listener();
		void SetAsListener();
	};
};