#include "MonopolyGlobal.h"
#include "cStartDistrict.h"

#include <conio.h>
#include <iostream>

cStartDistrict::cStartDistrict(int districtID)
	:cDistrict(districtID)
{
}


cStartDistrict::~cStartDistrict()
{
}
sProtocolDistrictInfo::eDistrictType cStartDistrict::DistrictType() { return sProtocolDistrictInfo::e_Start; }
bool cStartDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	// implement content here

	player->Deposit(200);

	std::cout << "\t cStartDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif
	return true;
}
bool cStartDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cStartDistrict::Response()" << std::endl;
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
