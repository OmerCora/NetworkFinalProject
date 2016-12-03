#include "MonopolyGlobal.h"
#include "cCardDistrict.h"

#include <conio.h>
#include <iostream>


cCardDistrict::cCardDistrict(int districtID, eCardDistrictType type)
	:cDistrict(districtID)
	,m_type(type)
{

}

cCardDistrict::~cCardDistrict()
{
}
sProtocolMonopolyHeader::eDistrictType cCardDistrict::DistrictType() { return sProtocolMonopolyHeader::e_Card; }
bool cCardDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	if (this->m_type == e_Community)
	{
		//ask the manager to let the player pick one card from community chest cards pile
		logic.openCommunityCardTo(player);

		// TODO: This mention is as I just get an idea when I work packet procedure.(I'll discuss about it together)
		//		this method is specific now. it should be change as generic purpose
		//		for example, you could get a interface of card storage, then request a card.
		//		so that, you can send the details to client through network
	}
	else if (this->m_type == e_Chance)
	{
		//ask the manager to make the player pick one card from chance cards pile
		logic.openChanceCardTo(player);
	}

	
	std::cout << "\t cCardDistrict::Action()" << std::endl;

	std::cout << "\t Press Any Key to Continue" << std::endl;
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif


	return true;
}

bool cCardDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cCardDistrict::Response()" << std::endl;
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

