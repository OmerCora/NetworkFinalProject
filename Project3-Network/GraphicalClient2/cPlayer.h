/*
* Program Name : INFO6017_AI_Project4 - cPlayer.h
* Purpose      : INFO-6017	Artificial Intelligence Poject3
* Description  : the Player class 
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/
#pragma once

#include "cEntity.h"

class cPlayer : public cEntity
{
public:
	cPlayer();
	virtual ~cPlayer();

	virtual void Update(float dt);

};