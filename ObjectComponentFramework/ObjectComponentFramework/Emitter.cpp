#include "Emitter.h"
#include "ObjectManager.h"
#include "Listener.h"
#include "Position.h"


namespace rabd
{
	Emitter::Emitter() : sound(nullptr)
	{
		ZeroMemory(&emitterData, sizeof(emitterData));	
	}

	Emitter::~Emitter()
	{
	}

	void Emitter::Update(ObjectID listener)
	{

		if (IsPlaying()) {
			ZeroMemory(&emitterData, sizeof(emitterData));			
			emitterData.Position = manager->GetComponent<Position>(GetParentID()).GetPosition();
	//		X3DAUDIO_EMITTER test = {0};
			X3DAUDIO_LISTENER listenerData = {0};

			//// Set up the emitter struct for start position.
			//XAUDIO2_VOICE_DETAILS details;
			//sound->GetSourceVoice()->GetVoiceDetails(&details);
			//test.ChannelCount = details.InputChannels;

	//		test.Position.x = 0;
	//		test.Position.y = 0;
	//		test.Position.z = 0;

	//		test.Position = manager->GetComponent<Position>(GetParentID()).GetPosition();

	//		listenerData.Position.x = 0;
	//		listenerData.Position.y = 0;
	//		listenerData.Position.z = 0;

			listenerData = manager->GetComponent<rabd::Listener>(listener).GetListenerStruct();


			AllanMilne::Audio::XACore::GetInstance()->Apply3D(sound->GetSourceVoice(), &emitterData, &listenerData, X3DAUDIO_CALCULATE_MATRIX);
		}
	}

	void Emitter::LoadFile(std::string file)
	{
		std::string longName = AUDIODIR + file;
		if (sound != nullptr) {
			delete sound;
		}
		sound = new AllanMilne::Audio::XASound(longName, true);
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
