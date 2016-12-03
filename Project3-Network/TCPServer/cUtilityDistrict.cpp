#include "MonopolyGlobal.h"
#include "cUtilityDistrict.h"

#include <conio.h>
#include <iostream>

cUtilityDistrict::cUtilityDistrict(int districtID, unsigned int price)
	:cAssetDistrict(districtID, price)
{
	m_isRequiredAnswer = true;
}


cUtilityDistrict::~cUtilityDistrict()
{
}
sProtocolMonopolyHeader::eDistrictType cUtilityDistrict::DistrictType() { return sProtocolMonopolyHeader::e_Utility; }
bool cUtilityDistrict::Action(iPlayer* player, iLogicMonopolyMediator& logic)
{
	std::cout << "\t cUtilityDistrict::Action()" << std::endl;
	std::cout << "\t Press Any Key to Continue" << std::endl;

	//check ownership
	if (this->m_owner)
	{
		//owner is set so someone already bought it

		//pay up or DIE
		player->Withdraw(this->m_price);
	}
	else
	{
		//ask if the user wants to buy it
		logic.PacketProcedure().SetHeader(sProtocolMonopolyHeader::e_AskAssetAction);
		sProtocolAskAssetAction protocol;
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
		this->SetOwner(player->User()->SocketID);
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
	std::cout << "\t Press Any Key to Continue" << std::endl;

	{
		logic.PacketProcedure().SetHeader(sProtocolMonopolyHeader::e_ResponsePlayAction);
		sProtocolResponsePlayAction protocol;
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

