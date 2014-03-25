#include "Emitter.h"

namespace rabd
{
	Emitter::Emitter() : sound(nullptr)
	{}

	Emitter::~Emitter()
	{}

	void Emitter::Update()
	{
	}

	void Emitter::LoadFile(std::string file)
	{
		std::string longName = AUDIODIR + file;
		if (sound != nullptr) {
			delete sound;
		}
		sound = new AllanMilne::Audio::XASound(longName);
	}

	void Emitter::Play()
	{
		if (sound) {
			sound->Play();
		}
	}

	void Emitter::Stop()
	{
		if (sound) {
			sound->Stop();
		}
	}

	void Emitter::Pause()
	{
		if (sound) {
			sound->Pause();
		}
	}
}
