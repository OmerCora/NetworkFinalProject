#include "cClientPacketProcedureMonopoly.h"
#include <cErrorReport.h>
#include <iostream>

cClientPacketProcedureMonopoly::cClientPacketProcedureMonopoly(iTCPClient& client)
	:m_client(client)
{
}


cClientPacketProcedureMonopoly::~cClientPacketProcedureMonopoly()
{
}

void cClientPacketProcedureMonopoly::SetHeader(sProtocolMonopolyHeader::ePacketID packetID)
{
	m_temporaryBuffer.clear();

	m_rootHeader.packet_id = sProtocolHeader::e_PlayMonopoly;
	m_header.packet_id = packetID;
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolResponseGameStart& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolRequestPlayThrowDice& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayThrowDice& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolRequestPlayAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolAskAssetAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolAnswerAssetAction& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayTurnChange& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolResponsePlayTurnKeep& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolResponseGameFinish& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}
void cClientPacketProcedureMonopoly::AppendProtocol(sProtocolResponseGameOver& protocol)
{
	m_header.SetProtocol(protocol);
	m_temporaryBuffer.Serialize(protocol);
}

bool cClientPacketProcedureMonopoly::SendData(SOCKET client)
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

void cClientPacketProcedureMonopoly::ProcessReceiveData(cBuffer& receiveBuffer)
{
	sProtocolMonopolyHeader header;
	receiveBuffer.Deserialize(header);

	switch (header.packet_id)
	{
	case sProtocolMonopolyHeader::ePacketID::e_ResponseGameStart:
	{
		system("cls");

		std::cout << "e_ResponseGameStart" << std::endl;

		sProtocolResponseGameStart data;
		receiveBuffer.Deserialize(data);

		//m_client.Set

		m_client.SetState(iTCPClient::e_GM_Start);

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponsePlayThrowDice:
	{
		std::cout << "e_ResponsePlayThrowDice" << std::endl;

		sProtocolResponsePlayThrowDice data;
		receiveBuffer.Deserialize(data);

		m_client.SetNextLocation(data.nextLocation);
		m_client.SetState(iTCPClient::e_GM_ThrowDice);

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponsePlayAction:
	{
		std::cout << "e_ResponsePlayAction" << std::endl;

		sProtocolResponsePlayAction data;
		receiveBuffer.Deserialize(data);

		if (data.districtType == sProtocolMonopolyHeader::e_Card)
		{
			m_client.SetState(iTCPClient::e_GM_ActionDistrictCard);
		}
		else if (data.districtType == sProtocolMonopolyHeader::e_FreeParking)
		{
			m_client.SetState(iTCPClient::e_GM_ActionDistrictFreeParking);
		}
		else if (data.districtType == sProtocolMonopolyHeader::e_GotoJail)
		{
			m_client.SetState(iTCPClient::e_GM_ActionDistrictGotoJail);
		}
		else if (data.districtType == sProtocolMonopolyHeader::e_Jail)
		{
			m_client.SetState(iTCPClient::e_GM_ActionDistrictJail);
		}
		else if (data.districtType == sProtocolMonopolyHeader::e_Start)
		{
			m_client.SetState(iTCPClient::e_GM_ActionDistrictStart);
		}
		else if (data.districtType == sProtocolMonopolyHeader::e_Tax)
		{
			m_client.SetState(iTCPClient::e_GM_ActionDistrictTax);
		}
		else if (data.districtType == sProtocolMonopolyHeader::e_Building)
		{
			m_client.SetState(iTCPClient::e_GM_ActionDistrictBuilding);
		}
		else if (data.districtType == sProtocolMonopolyHeader::e_Station)
		{
			m_client.SetState(iTCPClient::e_GM_ActionDistrictStation);
		}
		else if (data.districtType == sProtocolMonopolyHeader::e_Utility)
		{
			m_client.SetState(iTCPClient::e_GM_ActionDistrictUtility);
		}

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_AskAssetAction:
	{
		std::cout << "e_AskAssetAction" << std::endl;

		sProtocolAskAssetAction data;
		receiveBuffer.Deserialize(data);

		if (data.districtInfo.districtType == sProtocolMonopolyHeader::e_Building)
		{
			m_client.SetState(iTCPClient::e_GM_AskDistrictBuilding);
		}
		else if (data.districtInfo.districtType == sProtocolMonopolyHeader::e_Station)
		{
			m_client.SetState(iTCPClient::e_GM_AskDistrictStation);
		}
		else if (data.districtInfo.districtType == sProtocolMonopolyHeader::e_Utility)
		{
			m_client.SetState(iTCPClient::e_GM_AskDistrictUtility);
		}

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponsePlayTurnChange:
	{
		std::cout << "e_ResponsePlayTurnChange" << std::endl;

		sProtocolResponsePlayTurnChange data;
		receiveBuffer.Deserialize(data);

		m_client.SetState(iTCPClient::e_GM_TurnChange);

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponsePlayTurnKeep:
	{
		std::cout << "e_ResponsePlayTurnKeep" << std::endl;

		sProtocolResponsePlayTurnKeep data;
		receiveBuffer.Deserialize(data);


		m_client.SetState(iTCPClient::e_GM_TurnKeep);

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponseGameFinish:
	{
		std::cout << "e_ResponseGameFinish" << std::endl;

		sProtocolResponseGameFinish data;
		receiveBuffer.Deserialize(data);

		break;
	}
	case sProtocolMonopolyHeader::ePacketID::e_ResponseGameOver:
	{
		std::cout << "e_ResponseGameFinish" << std::endl;

		sProtocolResponseGameOver data;
		receiveBuffer.Deserialize(data);

		break;
	}
	default:
	{
		std::cout << "Unkown packet id: " << header.packet_id << std::endl;
		break;
	}
	}

}