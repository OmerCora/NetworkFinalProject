#include "MonopolyGlobal.h"
#include "cJailDistrict.h"

#include <conio.h>
#include <iostream>


cJailDistrict::cJailDistrict(int districtID)
	:cDistrict(districtID)
{
}


cJailDistrict::~cJailDistrict()
{
}

sProtocolMonopolyHeader::eDistrictType cJailDistrict::DistrictType() { return sProtocolMonopolyHeader::e_Jail; }
bool cJailDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	// implement content here


	int tempCount = player->getEnjailCount();
	player->SetEnjailCount(--tempCount);
	if (player->getEnjailCount() < 0)
	{
		player->SetEnjailCount(3);
		//player->setbChanceToThrowDice(false);
	}
	else if (player->getEnjailCount() <= 3 && player->getEnjailCount() >= 1)
	{
		//player->setbChanceToThrowDice(false);
	}
	else if (player->getEnjailCount() == 0)
	{
		//player->setbChanceToThrowDice(true);
	}


	std::cout << "\t cJailDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif
	return true;
}

bool cJailDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cJailDistrict::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;


	// TODO: send details to client
	{
		logic.PacketProcedure().SetHeader(sProtocolMonopolyHeader::e_ResponsePlayAction);
		sProtocolResponsePlayAction protocol;
		protocol.districtType = this->DistrictType();
		// TODO: add details into the protocol here
		logic.PacketProcedure().AppendProtocol(protocol);

		logic.PacketProcedure().SendData(logic.PlayerA().User()->SocketID());
		logic.PacketProcedure().SendData(logic.PlayerA().User()->SocketID());
	}
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif
	return true;
}
