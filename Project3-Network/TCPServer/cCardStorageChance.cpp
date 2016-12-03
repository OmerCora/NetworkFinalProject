#include "MonopolyGlobal.h"
#include "cCardStorageChance.h"

#include <iostream>


cCardStorageChance::cCardStorageChance()
{
}


cCardStorageChance::~cCardStorageChance()
{
}

int cCardStorageChance::pickOneCard()
{
	int enumIndex = rand() % 15;
	eChanceCards eChanceCard = (eChanceCards)enumIndex;

	if (eChanceCard == e_Advance_to_Go_Collect_200) std::cout << "Advance to Go (Collect $200)" << std::endl;
	if (eChanceCard == e_Advance_to_Illinois_Ave) std::cout << "Advance to Illinois Ave." << std::endl;
	if (eChanceCard == e_Advance_to_nearest_Utility) std::cout << "Advance token to nearest Utility. If unowned, you may buy it from the Bank. If owned, throw dice and pay owner a total ten times the amount thrown." << std::endl;
	if (eChanceCard == e_Advance_to_nearest_Railroad_and_Pay_owner_twice_the_rental) std::cout << "Advance token to the nearest Railroad and pay owner twice the rental to which he/she is otherwise entitled. If Railroad is unowned, you may buy it from the Bank. (There are two of these.)" << std::endl;
	if (eChanceCard == e_Advance_to_St_Charles_Place_if_you_pass_Go_Collect_200) std::cout << "Advance to St. Charles Place - if you pass Go, collect $200" << std::endl;
	if (eChanceCard == e_Bank_Pays_you_dividend_of_50) std::cout << "Bank pays you dividend of $50" << std::endl;
	if (eChanceCard == e_Get_out_of_Jail_free) std::cout << "Get out of Jail free - this card may be kept until needed, or traded/sold" << std::endl;
	if (eChanceCard == e_Go_back_3_spaces) std::cout << "Go back 3 spaces" << std::endl;
	if (eChanceCard == e_Go_directly_to_Jail) std::cout << "Go directly to Jail - do not pass Go, do not collect $200" << std::endl;
	if (eChanceCard == e_General_repairs_on_all_your_property) std::cout << "Make general repairs on all your property - for each house pay $25 - for each hotel $100" << std::endl;
	if (eChanceCard == e_Pay_poor_tax_of_15) std::cout << "Pay poor tax of $15" << std::endl;
	if (eChanceCard == e_Take_a_trip_to_Reading_Railroad_if_you_pass_Go_Collect_200) std::cout << "Take a trip to Reading Railroad - if you pass Go collect $200" << std::endl;
	if (eChanceCard == e_Take_a_walk_on_the_Boardwalk_advance_to_Boardwalk) std::cout << "Take a walk on the Boardwalk - advance token to Boardwalk" << std::endl;
	if (eChanceCard == e_Been_elected_chairman_of_the_board_Pay_each_player_50) std::cout << "You have been elected chairman of the board - pay each player $50" << std::endl;
	if (eChanceCard == e_Building_loan_matures_Collect_150) std::cout << "Your building loan matures - collect $150" << std::endl;
	if (eChanceCard == e_Won_a_crossword_competition_Collect_100) std::cout << " You have won a crossword competition - collect $100" << std::endl;
	
	return enumIndex;
}
