#include "MonopolyGlobal.h"
#include "cJailDistrict.h"
#include "cPlayer.h"

#include <conio.h>
#include <iostream>


cJailDistrict::cJailDistrict(int districtID)
	:cDistrict(districtID)
{
}


cJailDistrict::~cJailDistrict()
{
}

sProtocolDistrictInfo::eDistrictType cJailDistrict::DistrictType() { return sProtocolDistrictInfo::e_Jail; }
bool cJailDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	if (dynamic_cast<cPlayer*>(player)->getbJustCameInJail())
	{
		player->SetEnjailCount(3);
		player->setbCanThrowDice(false);
	}

	else if (player->getEnjailCount() == 0)
	{
		player->setbCanThrowDice(true);
	}

	int tempCount = player->getEnjailCount();
	player->SetEnjailCount(--tempCount);

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
		player->GetPlayerInfo(protocol.player);
		this->GetDistrictInfo(protocol.district);
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
