#pragma once

#include "Component.h"
#include "AllanMilne\XACore.hpp"

namespace rabd
{
	class Listener : public Component<Listener>
	{
	public:
		Listener();
		~Listener();
		void Update();
		inline const X3DAUDIO_LISTENER& GetListenerStruct() const {return listenerData;}
	private:
		X3DAUDIO_LISTENER listenerData;
	};
};