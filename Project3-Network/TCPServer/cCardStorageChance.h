#pragma once

#include "iCardStorage.h"

class cCardStorageChance : public iCardStorage
{
public:
	enum eChanceCards
	{
		e_Advance_to_Go_Collect_200,
		e_Advance_to_Illinois_Ave,
		e_Advance_to_nearest_Utility,
		e_Advance_to_nearest_Railroad_and_Pay_owner_twice_the_rental,
		e_Advance_to_St_Charles_Place_if_you_pass_Go_Collect_200,
		e_Bank_Pays_you_dividend_of_50,
		e_Get_out_of_Jail_free,
		e_Go_back_3_spaces,
		e_Go_directly_to_Jail,
		e_General_repairs_on_all_your_property,
		e_Pay_poor_tax_of_15,
		e_Take_a_trip_to_Reading_Railroad_if_you_pass_Go_Collect_200,
		e_Take_a_walk_on_the_Boardwalk_advance_to_Boardwalk,
		e_Been_elected_chairman_of_the_board_Pay_each_player_50,
		e_Building_loan_matures_Collect_150,
		e_Won_a_crossword_competition_Collect_100
	};

	cCardStorageChance();
	virtual ~cCardStorageChance();

	virtual int pickOneCard();
};

