#include "gelSoundManager.h"

#include "managers.h"

#include <time.h>
#include <vector>

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
	if (result != FMOD_OK)
	{
		//error
	}
}

float getRandFloat3(float LO, float HI)
{
	float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return r3;
}
//
//void gelSoundManager::Checkcooldowns(float deltaTime)
//{
//	
//	cooldown_1 -= deltaTime;
//	cooldown_2 -= deltaTime;
//	cooldown_3 -= deltaTime;
//
//	if (cooldown_1 <= 0.0f)
//	{
//		int randomVal = rand() % sampleChannels.size();
//		std::map<int, FMOD::Channel*>::iterator item = sampleChannels.begin();
//		std::advance(item, randomVal);
//		int objectID = item->first;
//		iGameObject* gameObject = gelObjectManager::sharedInstance().FindGameObjectWithID(objectID);
//		//fire a sound
//		this->PlayObjectSound(gameObject);
//		cooldown_1 = soundCooldown_1;
//	}
//
//	if (cooldown_2 <= 0.0f)
//	{
//		int randomVal = rand() % sampleChannels.size();
//		std::map<int, FMOD::Channel*>::iterator item = sampleChannels.begin();
//		std::advance(item, randomVal);
//		int objectID = item->first;
//		iGameObject* gameObject = gelObjectManager::sharedInstance().FindGameObjectWithID(objectID);
//		//fire a sound
//		this->PlayObjectSound(gameObject);
//		cooldown_2 = soundCooldown_2;
//	}
//	if (cooldown_3 <= 0.0f)
//	{
//		int randomVal = rand() % sampleChannels.size();
//		std::map<int, FMOD::Channel*>::iterator item = sampleChannels.begin();
//		std::advance(item, randomVal);
//		int objectID = item->first;
//		iGameObject* gameObject = gelObjectManager::sharedInstance().FindGameObjectWithID(objectID);
//		//fire a sound
//		this->PlayObjectSound(gameObject);
//		cooldown_3 = soundCooldown_3;
//	}
//}

void gelSoundManager::UpdateChannel(iGameObject* gameObject)
{
	std::string alias = gameObject->GetMesh().name;
	std::map< std::string, FMOD::Sound* >::iterator itSound
		= this->soundsSample.find(alias);

	// If the iterator ISN'T set to end(), then it found it
	if (itSound == this->soundsSample.end())
	{	// On no! Didn't find it
		//this cant have a channel
		return;
	}

	FMOD::Channel *ch;
	int objectID = gameObject->getObjectID();
	//look for the channel
	std::map< int, FMOD::Channel* >::iterator itChannel
		= this->sampleChannels.find(objectID);
	
	if (itChannel == this->sampleChannels.end())
	{
		//create new channel

		FMOD::Sound *sound = itSound->second;
		result = system->playSound(sound, 0, true, &ch);
		ERRCHECK(result);

		ch->setChannelGroup(groupSample);
		this->sampleChannels[objectID] = ch;
	}
	else
	{
		//use the already created channel and only update position
		FMOD::Channel* channel = itChannel->second; //same as this->sampleChannels[objectID]
		ch = channel;
	}

	GelPosition pos = gameObject->GetRender().position;
	GelPosition vel = gameObject->GetPhysics().velocity;
	FMOD_VECTOR position = { pos.x, pos.y, pos.z };
	FMOD_VECTOR velocity = { vel.x, vel.y, vel.z };

	result = ch->set3DAttributes(&position, &velocity);
	ERRCHECK(result);
}

void gelSoundManager::UpdateListenerPosition(void)
{
	iGameObject* mainPlayer = gelObjectManager::sharedInstance().GetMainPlayer();
	GelPosition mainPlayerPosition = mainPlayer->GetRender().position;

	FMOD_VECTOR position = { mainPlayerPosition.x, mainPlayerPosition.y, mainPlayerPosition.z };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 1.0f, 0.0f };

	result = system->set3DListenerAttributes(0, &position, &vel, &forward, &up);
	ERRCHECK(result);
}

//void gelSoundManager::StartBGSound(void)
//{
//	for (std::map<std::string, FMOD::Sound*>::iterator iter = soundsStream.begin(); iter != soundsStream.end(); ++iter)
//	{
//
//		//get the channel for the bgSound
//		FMOD::Sound *sound = iter->second;
//		result = system->playSound(sound, 0, false, &channelBG);
//
//		channelBG->setVolume(0.6f);
//
//		ERRCHECK(result);
//	}
//}

void gelSoundManager::PlayObjectSound(iGameObject * gameObject)
{
	std::string alias = gameObject->GetMesh().name;
	std::map< std::string, FMOD::Sound* >::iterator itSound
		= this->soundsSample.find(alias);

	// If the iterator ISN'T set to end(), then it found it
	if (itSound == this->soundsSample.end())
	{	// On no! Didn't find it
		//this cant have a channel
		return;
	}

	FMOD::Channel *ch;
	int objectID = gameObject->getObjectID();
	//look for the channel
	std::map< int, FMOD::Channel* >::iterator itChannel
		= this->sampleChannels.find(objectID);

	if (itChannel == this->sampleChannels.end())
	{
		//create new channel

		FMOD::Sound *sound = itSound->second;
		result = system->playSound(sound, 0, false, &ch);
		ERRCHECK(result);

		ch->setChannelGroup(groupSample);
		this->sampleChannels[objectID] = ch;
	}
	else
	{
		//use the already created channel and only update position
		FMOD::Channel* channel = itChannel->second; //same as this->sampleChannels[objectID]
		ch = channel;

		FMOD::Sound *sound = itSound->second;
		result = system->playSound(sound, 0, false, &ch);
		ERRCHECK(result);
	}

	GelPosition pos = gameObject->GetRender().position;
	GelVelocity vel = gameObject->GetPhysics().velocity;
	FMOD_VECTOR position = { pos.x, pos.y, pos.z };
	FMOD_VECTOR velocity = { vel.x, vel.y, vel.z };

	result = ch->set3DAttributes(&position, &velocity);
	ERRCHECK(result);
}

void gelSoundManager::PlaySoundWithName(std::string soundName)
{
	
	std::map< std::string, FMOD::Sound* >::iterator itSound
		= this->soundsSample.find(soundName);

	// If the iterator ISN'T set to end(), then it found it
	if (itSound == this->soundsSample.end())
	{	// On no! Didn't find it
		//this cant have a channel
		return;
	}

	FMOD::Channel* ch;
	FMOD::Sound *sound = itSound->second;
	result = system->playSound(sound, 0, false, &ch);
	ERRCHECK(result);
}

void gelSoundManager::CreateSampleSound(std::string alias, std::string path, float minDist, float maxDist)
{
	FMOD::Sound* sound;
	result = system->createSound((const char*)path.c_str(), FMOD_CREATESAMPLE | FMOD_3D, 0, &sound);
	ERRCHECK(result);

	result = sound->set3DMinMaxDistance(minDist, maxDist);
	ERRCHECK(result);

	this->soundsSample[alias] = sound;
}

void gelSoundManager::CreateStreamSound(std::string alias, std::string path)
{
	FMOD::Sound* sound;
	result = system->createSound((const char*)path.c_str(), FMOD_LOOP_NORMAL | FMOD_CREATESTREAM | FMOD_2D, 0, &sound);
	ERRCHECK(result);

	this->soundsStream[alias] = sound;
}

void gelSoundManager::Setup()
{
	soundBalance = 0.0f;
	pitch = 1.0f;

	//Create system object 
	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	//Get version
	result = system->getVersion(&version);
	ERRCHECK(result);

	//Validate version
	if (version < FMOD_VERSION)
	{
		//version error
	}

	//Initialize
	result = system->init(1024, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	//create channel
	result = system->createChannelGroup("BG (stream) Sounds", &groupBG);
	ERRCHECK(result);
	//create channel
	result = system->createChannelGroup("Sample Sounds", &groupSample);
	ERRCHECK(result);

	//get master group and the previous 2
	result = system->getMasterChannelGroup(&groupMaster);
	ERRCHECK(result);
	result = groupMaster->addGroup(groupBG);
	ERRCHECK(result);
	result = groupMaster->addGroup(groupSample);
	ERRCHECK(result);

}

void gelSoundManager::Update(float deltaTime)
{
	std::vector<iGameObject*> gameObjects = gelObjectManager::sharedInstance().GetGameObjects();
	
	this->UpdateListenerPosition();

	//not used for now
	//this->Checkcooldowns(deltaTime);

	//update sound locations of objects
	for (int index = 0; index != gameObjects.size(); index++)
	{
		this->UpdateChannel(gameObjects[index]);
	}

}

void gelSoundManager::CleanUp()
{
	for (std::map<std::string, FMOD::Sound*>::iterator iter = soundsSample.begin(); iter != soundsSample.end(); ++iter)
	{
		FMOD::Sound *sound = iter->second;
		result = sound->release();
		ERRCHECK(result);
	}

	for (std::map<std::string, FMOD::Sound*>::iterator iter = soundsStream.begin(); iter != soundsStream.end(); ++iter)
	{
		FMOD::Sound *sound = iter->second;
		result = sound->release();
		ERRCHECK(result);
	}
	
	result = groupBG->release();
	ERRCHECK(result);
	result = groupSample->release();
	ERRCHECK(result);
	result = groupMaster->release();
	ERRCHECK(result);

	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);
}

void gelSoundManager::IncreaseMasterVolume(void)
{
	masterVolume = (masterVolume >= 1.0f) ? 1.0f : (masterVolume += 0.05f);
	groupMaster->setVolume(masterVolume);
	ERRCHECK(result);
}
void gelSoundManager::DecreaseMasterVolume(void)
{
	masterVolume = (masterVolume <= 0.05f) ? 0.0f : (masterVolume -= 0.05f);
	groupMaster->setVolume(masterVolume);
	ERRCHECK(result);
}

void gelSoundManager::IncreasePan(void)
{
	soundBalance = (soundBalance >= 1.0f) ? 1.0f : (soundBalance += 0.05f);
	groupMaster->setVolume(soundBalance);
	ERRCHECK(result);
}

void gelSoundManager::DecreasePan(void)
{
	soundBalance = (soundBalance <= -0.95f) ? -1.0f : (soundBalance -= 0.05f);
	groupMaster->setPan(soundBalance);
	ERRCHECK(result);
}

void gelSoundManager::IncreasePitch(void)
{
	pitch = (pitch >= 2.0f) ? 2.0f : (pitch += 0.05f);
	groupMaster->setPitch(pitch);
	ERRCHECK(result);
}

void gelSoundManager::DecreasePitch(void)
{
	pitch = (pitch <= 0.55f) ? 0.5f : (pitch -= 0.05f);
	groupMaster->setPitch(pitch);
	ERRCHECK(result);
}
