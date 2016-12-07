#include "MonopolyGlobal.h"
#include "cStationDistrict.h"

#include <conio.h>
#include <iostream>


cStationDistrict::cStationDistrict(int districtID, int price)
	:cAssetDistrict(districtID, price)
{
}


cStationDistrict::~cStationDistrict()
{
}
sProtocolDistrictInfo::eDistrictType cStationDistrict::DistrictType() { return sProtocolDistrictInfo::e_Station; }
bool cStationDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cStationDistrict::Action()" << std::endl;

	//check ownership
	m_isRequiredAnswer = false;
	if (this->m_owner)
	{

		//owner is set so someone already bought it

		//pay up or DIE
		player->Withdraw(this->m_price);
		this->m_owner->Deposit(m_price);

		//price may also be modified by a multiplier dependant on number of owned station;
	}
	else if (player->CanBuy(this->m_price))
	{
		m_isRequiredAnswer = true;

		//ask if the user wants to buy it
		logic.PacketProcedure().SetHeader(sProtocolMonopolyHeader::e_AskAssetAction);
		sProtocolAskAssetAction protocol;
		player->GetPlayerInfo(protocol.player, player->PlayerID());
		protocol.districtInfo.districtType = this->DistrictType();
		protocol.districtInfo.district_id = this->m_districtID;
		protocol.districtInfo.price = this->m_price;
		logic.PacketProcedure().AppendProtocol(protocol);

		logic.PacketProcedure().SendData(player->User()->SocketID());
	}
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif

	return true;
}
bool cStationDistrict::ReceiveAnswer(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cStationDistrict::ReceiveAnswer() m_districtID: " << m_districtID << std::endl;

	sProtocolAnswerAssetAction* answerAction = (sProtocolAnswerAssetAction*)logic.GetLastReceivedProtocol();

	if (answerAction->yesOrNo)
	{
		//player said yes
		this->SetOwner(player);
	}
	else
	{
		//player said no
		//keep playing
	}

	return true;
}
bool cStationDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cStationDistrict::Response()" << std::endl;

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
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif

	return true;
}
