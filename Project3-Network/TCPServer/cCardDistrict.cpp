#include "MonopolyGlobal.h"
#include "cCardDistrict.h"
#include "cCardStorageCommunity.h"
#include "cCardStorageChance.h"

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
		// TODO: This mention is as I just get an idea when I work packet procedure.(I'll discuss about it together)
		//		this method is specific now. it should be change as generic purpose
		//		for example, you could get a interface of card storage, then request a card.
		//		so that, you can send the details to client through network

		cCardStorageCommunity communityStorage;
		cCardStorageCommunity::eCommunityCards eCommCard = (cCardStorageCommunity::eCommunityCards)communityStorage.pickOneCard();
		//int eCommCardIndex = commCardStorage.pickOneCard();

		switch (eCommCard)
		{
		case cCardStorageCommunity::e_Advance_to_Go_Collect_200:  
			
			break;
		case cCardStorageCommunity::e_Bank_error_in_your_favor_Collect_75:  
			player->Deposit(75);
			break;
		case cCardStorageCommunity::e_Doctors_fees_Pay_50:  
			player->Withdraw(50);
			break;
		case cCardStorageCommunity::e_Get_out_of_jail_free:  
			
			break;
		case cCardStorageCommunity::e_Go_to_jail_directly:  
			
			break;
		case cCardStorageCommunity::e_It_is_your_birthday_Collect_10_from_each_player:  
			
			break;
		case cCardStorageCommunity::e_Grand_Opera_Night_Collect_50_from_every_player:  
			
			break;
		case cCardStorageCommunity::e_Income_Tax_refund_Collect_20:  
			player->Deposit(20);
			break;
		case cCardStorageCommunity::e_Life_Insurance_Matures_Collect_100:  
			player->Deposit(100);
			break;
		case cCardStorageCommunity::e_Pay_Hospital_Fees_of_100:  
			player->Withdraw(100);
			break;
		case cCardStorageCommunity::e_Pay_School_Fees_of_50:  
			
			break;
		case cCardStorageCommunity::e_Receive_25_Consultancy_Fee:  
			player->Deposit(100);
			break;
		case cCardStorageCommunity::e_Street_repairs_40_per_house_115_per_hotel:  
			
			break;
		case cCardStorageCommunity::e_Prize_in_a_beauty_contest_Collect_10:  
			player->Deposit(10);
			break;
		case cCardStorageCommunity::e_Inherit_100:  
			player->Deposit(100);
			break;
		case cCardStorageCommunity::e_From_sale_of_stock_50:  
			player->Deposit(50);
			break;
		case cCardStorageCommunity::e_Holiday_Fund_matures_Receive_100:  
			player->Deposit(100);
			break;
		default:
			break;

		}
	}
	else if (this->m_type == e_Chance)
	{
		cCardStorageChance chanceCard;
		cCardStorageChance::eChanceCards eChanceCard = (cCardStorageChance::eChanceCards)chanceCard.pickOneCard();
		switch (eChanceCard)
		{
		case cCardStorageChance::e_Advance_to_Go_Collect_200:    
			
			break;
		case cCardStorageChance::e_Advance_to_Illinois_Ave:    
			
			break;
		case cCardStorageChance::e_Advance_to_nearest_Utility:    
			
			break;
		case cCardStorageChance::e_Advance_to_nearest_Railroad_and_Pay_owner_twice_the_rental:    
			
			break;
		case cCardStorageChance::e_Advance_to_St_Charles_Place_if_you_pass_Go_Collect_200:    
			
			break;
		case cCardStorageChance::e_Bank_Pays_you_dividend_of_50:    
			player->Deposit(50);
			break;
		case cCardStorageChance::e_Get_out_of_Jail_free:    
			
			break;
		case cCardStorageChance::e_Go_back_3_spaces:    
			
			break;
		case cCardStorageChance::e_Go_directly_to_Jail:    
			
			break;
		case cCardStorageChance::e_General_repairs_on_all_your_property:    
			
			break;
		case cCardStorageChance::e_Pay_poor_tax_of_15:    
			player->Withdraw(15);
			break;
		case cCardStorageChance::e_Take_a_trip_to_Reading_Railroad_if_you_pass_Go_Collect_200:    
			
			break;
		case cCardStorageChance::e_Take_a_walk_on_the_Boardwalk_advance_to_Boardwalk:    
			
			break;
		case cCardStorageChance::e_Been_elected_chairman_of_the_board_Pay_each_player_50:    
			
			break;
		case cCardStorageChance::e_Building_loan_matures_Collect_150:    
			player->Deposit(150);
			break;
		case cCardStorageChance::e_Won_a_crossword_competition_Collect_100:    
			player->Deposit(100);
			break;

		default:
			break;

		}

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

