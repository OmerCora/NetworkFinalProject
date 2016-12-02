#include "cClientPacketProcedureMonopoly.h"

#include <iostream>

cClientPacketProcedureMonopoly::cClientPacketProcedureMonopoly()
{
}


cClientPacketProcedureMonopoly::~cClientPacketProcedureMonopoly()
{
}



void cClientPacketProcedureMonopoly::SetHeader()
{
}

void cClientPacketProcedureMonopoly::AppendProtocol()
{
}

void cClientPacketProcedureMonopoly::SendData()
{
}

void cClientPacketProcedureMonopoly::ProcessReceiveData(cBuffer& receiveBuffer)
{
	sProtocolMonopolyHeader header;
	receiveBuffer.Deserialize(header);

	switch (header.packet_id)
	{
	case sProtocolMonopolyHeader::ePacketID::e_ResponseGameStart:
	{
		std::cout << "e_ResponseGameStart" << std::endl;

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_RequestPlayDiceThrow:
	{
		std::cout << "e_RequestPlayDiceThrow" << std::endl;

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponsePlayDiceThrow:
	{
		std::cout << "e_ResponsePlayDiceThrow" << std::endl;

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_RequestPlayAction:
	{
		std::cout << "e_RequestPlayAction" << std::endl;

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponsePlayAction:
	{
		std::cout << "e_ResponsePlayAction" << std::endl;

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponsePlayTurnChange:
	{
		std::cout << "e_ResponsePlayTurnChange" << std::endl;

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponseGameFinish:
	{
		std::cout << "e_ResponseGameFinish" << std::endl;

		break;
	}
	default:
		break;
	}

}