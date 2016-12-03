#include "MonopolyGlobal.h"
#include "cCardStorageCommunity.h"


#include <iostream>

cCardStorageCommunity::cCardStorageCommunity()
{
}


cCardStorageCommunity::~cCardStorageCommunity()
{
}

int cCardStorageCommunity::pickOneCard()
{
	int enumIndex = rand() % 16;
	eCommunityCards eCommCard = (eCommunityCards)enumIndex;

	if (eCommCard == e_Advance_to_Go_Collect_200) std::cout << "Advance to Go (Collect $200)" << std::endl;
	if (eCommCard == e_Bank_error_in_your_favor_Collect_75) std::cout << "Bank error in your favor - collect $75" << std::endl;
	if (eCommCard == e_Doctors_fees_Pay_50) std::cout << "Doctor's fees - Pay $50" << std::endl;
	if (eCommCard == e_Get_out_of_jail_free) std::cout << "Get out of jail free - this card may be kept until needed, or sold" << std::endl;
	if (eCommCard == e_Go_to_jail_directly) std::cout << "Go to jail - go directly to jail - Do not pass Go, do not collect $200" << std::endl;
	if (eCommCard == e_It_is_your_birthday_Collect_10_from_each_player) std::cout << "It is your birthday Collect $10 from each player" << std::endl;
	if (eCommCard == e_Grand_Opera_Night_Collect_50_from_every_player) std::cout << "Grand Opera Night - collect $50 from every player for opening night seats" << std::endl;
	if (eCommCard == e_Income_Tax_refund_Collect_20) std::cout << "Income Tax refund - collect $20" << std::endl;
	if (eCommCard == e_Life_Insurance_Matures_Collect_100) std::cout << "Life Insurance Matures - collect $100" << std::endl;
	if (eCommCard == e_Pay_Hospital_Fees_of_100) std::cout << "Pay Hospital Fees of $100" << std::endl;
	if (eCommCard == e_Pay_School_Fees_of_50) std::cout << "Pay School Fees of $50" << std::endl;
	if (eCommCard == e_Receive_25_Consultancy_Fee) std::cout << "Receive $25 Consultancy Fee" << std::endl;
	if (eCommCard == e_Street_repairs_40_per_house_115_per_hotel) std::cout << "You are assessed for street repairs - $40 per house, $115 per hotel" << std::endl;
	if (eCommCard == e_Prize_in_a_beauty_contest_Collect_10) std::cout << "You have won second prize in a beauty contest- collect $10" << std::endl;
	if (eCommCard == e_Inherit_100) std::cout << "You inherit $100" << std::endl;
	if (eCommCard == e_From_sale_of_stock_50) std::cout << "From sale of stock you get $50" << std::endl;
	if (eCommCard == e_Holiday_Fund_matures_Receive_100) std::cout << "Holiday Fund matures - Receive $100" << std::endl;


	return enumIndex;
}
