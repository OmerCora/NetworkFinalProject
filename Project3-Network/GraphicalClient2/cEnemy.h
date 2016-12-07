/*
* Program Name : INFO6017_AI_Project4 - cEnemy.cpp
* Purpose      : INFO-6017	Artificial Intelligence Poject3
* Description  : cEnemy class that does Evade, Arrive, Pursue and Wander behaviors
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/
#pragma once
#include "cEntity.h"

class cEnemy : public cEntity
{
public:
	enum eEnemyType
	{
		TYPE1,
		TYPE2
	}mType;


	cEnemy();
	virtual ~cEnemy();

	virtual void Update(float dt);	

	////member variables to be used for wander state behaviour
	//int countWnader;
	//float yAxisWander;
	//float rotAngleWander;
};