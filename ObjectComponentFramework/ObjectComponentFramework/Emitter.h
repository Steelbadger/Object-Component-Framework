#pragma once
#include "Component.h"
#include "GameObject.h"
#include "AllanMilne\XASound.hpp"

#include <string>
#include "AllanMilne\XACore.hpp"

#define AUDIODIR "Assets/Audio/"

namespace rabd
{
	class Emitter : public Component<Emitter>
	{
	public:
		Emitter();
		~Emitter();

		void Update(ObjectID listener);
		void LoadFile(std::string filename);
		void Play();
		void Stop();
		void Pause();
		inline const X3DAUDIO_EMITTER& GetEmitterStruct() const {return emitterData;}
	private:
		AllanMilne::Audio::XASound* sound;
		X3DAUDIO_EMITTER emitterData;
	};
};