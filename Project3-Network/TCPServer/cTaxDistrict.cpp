#include "MonopolyGlobal.h"
#include "cTaxDistrict.h"

#include <conio.h>
#include <iostream>

cTaxDistrict::cTaxDistrict(int districtID, eTaxDistrictType taxtype)
	:cDistrict(districtID)
	, m_taxtype(taxtype)
{
}


cTaxDistrict::~cTaxDistrict()
{
}
sProtocolDistrictInfo::eDistrictType cTaxDistrict::DistrictType() { return sProtocolDistrictInfo::e_Tax; }
bool cTaxDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	// implement content here

	if (this->m_taxtype == e_Normal)
	{
		//ask the manager to take 100 from the player
		//logic.takeMoneyFrom(100, player);
	}
	else if (this->m_taxtype == e_Luxury)
	{
		//ask the manager to to take 200 from the player
		//logic.takeMoneyFrom(200, player);
	}


	std::cout << "\t cTaxDistrict::Action()" << std::endl;
	return true;
}
bool cTaxDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cTaxDistrict::Response()" << std::endl;


	// TODO: send details to client
	{
		logic.PacketProcedure().SetHeader(sProtocolMonopolyHeader::e_ResponsePlayAction);
		sProtocolResponsePlayAction protocol;
		player->GetPlayerInfo(protocol.player, player->PlayerID());
		this->GetDistrictInfo(protocol.district);
		protocol.districtType = this->DistrictType();
		// TODO: add details into the protocol here
		logic.PacketProcedure().AppendProtocol(protocol);

		logic.PacketProcedure().SendData(logic.PlayerA().User()->SocketID());
		logic.PacketProcedure().SendData(logic.PlayerB().User()->SocketID());
	}
	return true;
}
