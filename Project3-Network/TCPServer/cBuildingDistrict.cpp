#include "MonopolyGlobal.h"
#include "cBuildingDistrict.h"


#include <conio.h>
#include <iostream>

cBuildingDistrict::cBuildingDistrict(int districtID)
	:cAssetDistrict(districtID)
{
	m_isRequiredAnswer = true;
}


cBuildingDistrict::~cBuildingDistrict()
{
}
sProtocolMonopolyHeader::eDistrictType cBuildingDistrict::DistrictType() { return sProtocolMonopolyHeader::e_Building; }
bool cBuildingDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cBuildingDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;

	// implement content here


	// TODO: ask to buy or pay it to client
	{
		logic.PacketProcedure().SetHeader(sProtocolMonopolyHeader::e_AskAssetAction);
		sProtocolResponsePlayAction protocol;
		protocol.districtType = this->DistrictType();
		// TODO: add details into the protocol here
		logic.PacketProcedure().AppendProtocol(protocol);

		logic.PacketProcedure().SendData(player->User()->SocketID());
	}

#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif

	return true;
}
bool cBuildingDistrict::ReceiveAnswer(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cBuildingDistrict::ReceiveAnswer() m_districtID: " << m_districtID << std::endl;



	return true;
}
bool cBuildingDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cBuildingDistrict::Response()" << std::endl;
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