#include "MonopolyGlobal.h"
#include "cFreeParking.h"

#include <conio.h>
#include <iostream>


cFreeParking::cFreeParking(int districtID)
	:cDistrict(districtID)
{
}


cFreeParking::~cFreeParking()
{
}
sProtocolMonopolyHeader::eDistrictType cFreeParking::DistrictType() { return sProtocolMonopolyHeader::e_FreeParking; }
bool cFreeParking::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cFreeParking::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif
	return true;
}
bool cFreeParking::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cFreeParking::Response()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;

	// implement content here



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

