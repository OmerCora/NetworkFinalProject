#pragma once


class iTCPClient
{
public:
	virtual ~iTCPClient() {}

	enum eGameMonopolyState
	{
		e_GM_Wait = 0,
		e_GM_Start,
		e_GM_ThrowDice,
		e_GM_AnimationThrowDice,
		e_GM_MovePiece,
		e_GM_AnimationMovePiece,

		e_GM_ActionDistrictStart,
		e_GM_ActionDistrictTax,
		e_GM_ActionDistrictCard,
		e_GM_ActionDistrictFreeParking,
		e_GM_ActionDistrictGotoJail,
		e_GM_ActionDistrictJail,
		e_GM_ActionDistrictBuilding,
		e_GM_ActionDistrictStation,
		e_GM_ActionDistrictUtility,
		e_GM_AskDistrictBuilding,
		e_GM_AskDistrictStation,
		e_GM_AskDistrictUtility,

		e_GM_TurnChange,
		e_GM_TurnKeep,

		e_GM_Finish,
	};

	virtual SOCKET GetSocketID() = 0;
	virtual void SetState(eGameMonopolyState state) = 0;
	//virtual void SetNextLocation(short nextLocation) = 0;
	virtual void SetGameStarted(bool flag) = 0;
	virtual void PrintPlayerInfo(sProtocolPlayerInfo& info) = 0;

};

