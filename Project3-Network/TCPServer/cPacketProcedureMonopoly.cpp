#include "MonopolyGlobal.h"

#include "cPacketProcedureMonopoly.h"
#include <sProtocolsMonopoly.h>
#include <cErrorReport.h>
#include "iLogicMonopoly.h"

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
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponseGameStart& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolRequestPlayThrowDice& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayThrowDice& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolRequestPlayAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolAskAssetAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolAnswerAssetAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayTurnChange& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayTurnKeep& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponseGameFinish& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponseGameOver& protocol)
{
	m_header.SetProtocol(protocol);
	m_sendBuffer.Serialize(protocol);
}

bool cPacketProcedureMonopoly::SendData(SOCKET client)
{
	const char* data = m_sendBuffer.toCharArray();
	int length = (int)m_sendBuffer.getLength();
	int result = send(client, data, length, 0);
	if (result == SOCKET_ERROR)
	{
		cErrorReport::GetInstance().PrintWSAGetLastMessage("\tSend failed: ");
		return false;
	}

	std::cout << "\t Socket: " << client << " Bytes sent: " << result << std::endl;

	return true;
}

void cPacketProcedureMonopoly::ProcessReceiveData(cBuffer& receiveBuffer)
{
	sProtocolMonopolyHeader header;
	receiveBuffer.Deserialize(header);

	switch (header.packet_id)
	{
	case sProtocolMonopolyHeader::ePacketID::e_RequestPlayThrowDice:
	{
		std::cout << "e_RequestPlayDiceThrow" << std::endl;

		sProtocolRequestPlayThrowDice data;
		receiveBuffer.Deserialize(data);

		// change state
		iLogicMonopoly& logic = dynamic_cast<iLogicMonopoly&>(m_logic);
		logic.SetState(iLogicMonopoly::e_ThrowDice);

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_RequestPlayAction:
	{
		std::cout << "e_RequestPlayAction" << std::endl;

		sProtocolRequestPlayAction data;
		receiveBuffer.Deserialize(data);

		iLogicMonopoly& logic = dynamic_cast<iLogicMonopoly&>(m_logic);
		logic.SetState(iLogicMonopoly::e_Action);

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_AnswerAssetAction:
	{
		std::cout << "e_AnswerAssetAction" << std::endl;

		sProtocolAnswerAssetAction data;
		receiveBuffer.Deserialize(data);

		iLogicMonopoly& logic = dynamic_cast<iLogicMonopoly&>(m_logic);
		logic.SetState(iLogicMonopoly::e_ReceiveAnswer);

		break;
	}
	default:
		break;
	}

}