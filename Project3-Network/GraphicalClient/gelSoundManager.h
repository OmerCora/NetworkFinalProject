#ifndef _GELSOUNDMANAGER_HG_
#define _GELSOUNDMANAGER_HG_

#include "iManager.h"

#include <map>
#include <fmod.hpp>
#include "iGameObject.h"


class gelSoundManager : public iManager
{
public:
	//Singleton 
	static gelSoundManager & sharedInstance()
	{
		static gelSoundManager  instance;
		return instance;
	}
	void PlayObjectSound(iGameObject* gameObject);
	void PlaySoundWithName(std::string soundName);

	void CreateSampleSound(std::string alias, std::string path, float minDist, float maxDist);
	void CreateStreamSound(std::string alias, std::string path);
	void Setup();
	//void StartBGSound(void);
	virtual void Update(float deltaTime);
	void CleanUp();

	void IncreaseMasterVolume(void);
	void DecreaseMasterVolume(void);

	void IncreasePan(void);
	void DecreasePan(void);

	void IncreasePitch(void);
	void DecreasePitch(void);

private:
	gelSoundManager() {};
	gelSoundManager(gelSoundManager  const&); //dont implement - to build
	void operator=(gelSoundManager  const&); //dont implement - to build

	void UpdateChannel(iGameObject* gameObject);
	void UpdateListenerPosition(void);

	FMOD::System*      system;
	std::map<std::string, FMOD::Sound*> soundsStream;
	std::map<std::string, FMOD::Sound*> soundsSample;
	FMOD::Channel* channelBG;
	FMOD::Channel* chWithName[3];
	std::map<int, FMOD::Channel*> sampleChannels;
	FMOD::ChannelGroup *groupBG, *groupSample;
	FMOD::ChannelGroup *groupMaster;
	FMOD_RESULT result;
	unsigned int version;
	float masterVolume;
	float soundBalance;
	float pitch;
	void* extradriverdata = 0;
};

#endif // !_GELSOUNDMANAGER_HG_
