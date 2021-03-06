#include "MonopolyGlobal.h"
#include "cUtilityDistrict.h"

#include <conio.h>
#include <iostream>

cUtilityDistrict::cUtilityDistrict(int districtID, int price)
	:cAssetDistrict(districtID, price)
{
}


cUtilityDistrict::~cUtilityDistrict()
{
}
sProtocolDistrictInfo::eDistrictType cUtilityDistrict::DistrictType() { return sProtocolDistrictInfo::e_Utility; }
bool cUtilityDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cUtilityDistrict::Action()" << std::endl;

	//check ownership
	m_isRequiredAnswer = false;
	if (this->m_owner)
	{

		//owner is set so someone already bought it

		//pay up or DIE
		player->Withdraw(this->m_price);
		this->m_owner->Deposit(m_price);
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
bool cUtilityDistrict::ReceiveAnswer(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cUtilityDistrict::ReceiveAnswer() m_districtID: " << m_districtID << std::endl;

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
bool cUtilityDistrict::Response(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cUtilityDistrict::Response()" << std::endl;

	{
		logic.PacketProcedure().SetHeader(sProtocolMonopolyHeader::e_ResponsePlayAction);
		sProtocolResponsePlayAction protocol;
		player->GetPlayerInfo(protocol.player, player->PlayerID());
		this->GetDistrictInfo(protocol.district);
		protocol.districtType = this->DistrictType();
		logic.PacketProcedure().AppendProtocol(protocol);

		logic.PacketProcedure().SendData(logic.PlayerA().User()->SocketID());
		logic.PacketProcedure().SendData(logic.PlayerB().User()->SocketID());
	}
#ifdef _LOGIC_DEBUG_TEST
	char anyKey = _getch();
#endif
	return true;
}

