#pragma once

#include "iCardStorage.h"

class cCardStorageCommunity : public iCardStorage
{
public:
	enum eCommunityCards
	{
		e_Advance_to_Go_Collect_200,
		e_Bank_error_in_your_favor_Collect_75,
		e_Doctors_fees_Pay_50,
		e_Get_out_of_jail_free,
		e_Go_to_jail_directly,
		e_It_is_your_birthday_Collect_10_from_each_player,
		e_Grand_Opera_Night_Collect_50_from_every_player,
		e_Income_Tax_refund_Collect_20,
		e_Life_Insurance_Matures_Collect_100,
		e_Pay_Hospital_Fees_of_100,
		e_Pay_School_Fees_of_50,
		e_Receive_25_Consultancy_Fee,
		e_Street_repairs_40_per_house_115_per_hotel,
		e_Prize_in_a_beauty_contest_Collect_10,
		e_Inherit_100,
		e_From_sale_of_stock_50,
		e_Holiday_Fund_matures_Receive_100
	};

	cCardStorageCommunity();
	virtual ~cCardStorageCommunity();
};

