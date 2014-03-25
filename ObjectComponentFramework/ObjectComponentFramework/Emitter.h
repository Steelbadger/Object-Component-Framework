#pragma once
#include "Component.h"
#include "AllanMilne\XASound.hpp"

#include <string>

#define AUDIODIR "Assets/Audio/"

namespace rabd
{
	class Emitter : public Component<Emitter>
	{
	public:
		Emitter();
		~Emitter();

		void Update();
		void LoadFile(std::string filename);
		void Play();
		void Stop();
		void Pause();
	private:
		AllanMilne::Audio::XASound* sound;
	};
};