#include "MonopolyGlobal.h"

#include "cPacketProcedureMonopoly.h"
#include <sProtocolsMonopoly.h>


cPacketProcedureMonopoly::cPacketProcedureMonopoly(iLogicMonopolyMediator& logic)
	:m_logic(logic)
{
}

cPacketProcedureMonopoly::~cPacketProcedureMonopoly()
{
}

void cPacketProcedureMonopoly::SetHeader(sProtocolMonopolyHeader::ePacketID packetID)
{
	m_sendBuffer.clear();
	m_header.packet_id = packetID;
	m_sendBuffer.Serialize(m_header);
}

void cPacketProcedureMonopoly::AppendProtocol(iProtocol& protocol)
{
	m_header.SetProtocol(protocol);
	//m_sendBuffer.Serialize(protocol);
}

void cPacketProcedureMonopoly::SendData()
{
	m_sendBuffer.clear();
}

void cPacketProcedureMonopoly::ProcessReceiveData(cBuffer& receiveBuffer)
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