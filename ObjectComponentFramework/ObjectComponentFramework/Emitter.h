#pragma once
#include "Component.h"

namespace rabd
{
	class Emitter : public Component<Emitter>
	{
	public:
		Emitter();
		~Emitter();

		void Update();
	private:
	};
};