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
	m_temporaryBuffer.clear();

	m_rootHeader.packet_id = sProtocolHeader::e_PlayMonopoly;
	m_header.packet_id = packetID;
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponseGameStart& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolRequestPlayThrowDice& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayThrowDice& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolRequestPlayAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolAskAssetAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolAnswerAssetAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayTurnChange& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayTurnKeep& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponseGameFinish& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolResponseGameOver& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cPacketProcedureMonopoly::AppendProtocol(sProtocolBoardInfo& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
bool cPacketProcedureMonopoly::SendData(SOCKET client)
{
	m_sendBuffer.clear();

	m_rootHeader.SetProtocol(m_header);

	m_sendBuffer.Serialize(m_rootHeader);
	m_sendBuffer.Serialize(m_header);
	m_sendBuffer.Append(m_temporaryBuffer);

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
