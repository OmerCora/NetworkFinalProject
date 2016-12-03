#ifndef _cBuffer_HG_
#define _cBuffer_HG_

#include <stdint.h>
#include <string>
#include <vector>


#include "sProtocols.h"

#include "iBuffer.h"


class cBuffer : public iBuffer
{
public:
	static const unsigned short mc_DEFAULT_BUFFER_LENGTH = 512;

	cBuffer();
	~cBuffer();

	void clear();
	void CheckBufferSize(std::size_t size);
	void loadBuffer(const char* recvbuf, size_t size);
	void Append(cBuffer& buffer);
	void flushBuffer();
	const char* toCharArray();


	bool isEmpty();
	std::size_t getLength() const;
	std::size_t getReadIndex() const;
	std::size_t getWriteIndex() const;

	void setReadIndex(std::size_t index);
	void setWriteIndex(std::size_t index);

	void printInHex();
	
	void Serialize(const sProtocolHeader& data);
	void Serialize(const sProtocolRoom& data);
	void Serialize(const sProtocolResponseRoomList& data);
	void Serialize(const sProtocolRequestNewRoom& data);
	void Serialize(const sProtocolRequestJoinRoom& data);
	void Serialize(const sProtocolRequestLeaveRoom& data);
	void Serialize(const sProtocolUser& data);
	void Serialize(const sProtocolPassword& data);
	void Serialize(const sProtocolResponseRoomInfo& data);
	void Serialize(const sProtocolChatMessage& data);
	void Serialize(const sProtocolSendChatMessage& data);
	void Serialize(const sProtocolResponseChatHistory& data);
	void Serialize(const sProtocolLobbyUsers& data);
	void Serialize(const sProtocolResponseFailure& data);
	void Serialize(const sProtocolAccount& data);

	void Deserialize(sProtocolHeader& data);
	void Deserialize(sProtocolRoom& data);
	void Deserialize(sProtocolResponseRoomList& data);
	void Deserialize(sProtocolRequestNewRoom& data);
	void Deserialize(sProtocolRequestJoinRoom& data);
	void Deserialize(sProtocolRequestLeaveRoom& data);
	void Deserialize(sProtocolUser& data);
	void Deserialize(sProtocolPassword& data);
	void Deserialize(sProtocolResponseRoomInfo& data);
	void Deserialize(sProtocolChatMessage& data);
	void Deserialize(sProtocolSendChatMessage& data);
	void Deserialize(sProtocolResponseChatHistory& data);
	void Deserialize(sProtocolLobbyUsers& data);
	void Deserialize(sProtocolResponseFailure& data);
	void Deserialize(sProtocolAccount& data);

	/************************************************
	// Protocols for Game Monopoly
	************************************************/
	void Serialize(const sProtocolNameInfo& data);
	void Deserialize(sProtocolNameInfo& data);
	void Serialize(const sProtocolPlayerInfo& data);
	void Deserialize(sProtocolPlayerInfo& data);
	void Serialize(const sProtocolDistrictInfo& data);
	void Deserialize(sProtocolDistrictInfo& data);
	void Serialize(const sProtocolBoardInfo& data);
	void Deserialize(sProtocolBoardInfo& data);

	void Serialize(const sProtocolMonopolyHeader& data);
	void Deserialize(sProtocolMonopolyHeader& data);
	void Serialize(const sProtocolResponseGameStart& data);
	void Deserialize(sProtocolResponseGameStart& data);
	void Serialize(const sProtocolRequestPlayThrowDice& data);
	void Deserialize(sProtocolRequestPlayThrowDice& data);
	void Serialize(const sProtocolResponsePlayThrowDice& data);
	void Deserialize(sProtocolResponsePlayThrowDice& data);
	void Serialize(const sProtocolRequestPlayAction& data);
	void Deserialize(sProtocolRequestPlayAction& data);
	void Serialize(const sProtocolResponsePlayAction& data);
	void Deserialize(sProtocolResponsePlayAction& data);
	void Serialize(const sProtocolAskAssetAction& data);
	void Deserialize(sProtocolAskAssetAction& data);
	void Serialize(const sProtocolAnswerAssetAction& data);
	void Deserialize(sProtocolAnswerAssetAction& data);
	void Serialize(const sProtocolResponsePlayTurnChange& data);
	void Deserialize(sProtocolResponsePlayTurnChange& data);
	void Serialize(const sProtocolResponsePlayTurnKeep& data);
	void Deserialize(sProtocolResponsePlayTurnKeep& data);
	void Serialize(const sProtocolResponseGameFinish& data);
	void Deserialize(sProtocolResponseGameFinish& data);
	void Serialize(const sProtocolResponseGameOver& data);
	void Deserialize(sProtocolResponseGameOver& data);

	void writeChar(char value);
	char readChar();

	void writeStringBE(const std::string& value);
	void writeStringBE(std::size_t index, const std::string& value);
	std::string readStringBE(std::size_t stringLength);
	std::string readStringBE(std::size_t index,std::size_t stringLength);

	void writeUInt32BE(std::size_t index, uint32_t value);
	void writeUInt32BE(uint32_t value);
	void writeUInt32LE(std::size_t index, uint32_t value);
	void writeUInt32LE(uint32_t value);

	void writeInt32BE(std::size_t index, int32_t value);
	void writeInt32BE(int32_t value);
	void writeInt32LE(std::size_t index, int32_t value);
	void writeInt32LE(int32_t value);

	void writeUInt16BE(std::size_t index, uint16_t value);
	void writeUInt16BE(uint16_t value);
	void writeUInt16LE(std::size_t index, uint16_t value);
	void writeUInt16LE(uint16_t value);

	void writeInt16BE(std::size_t index, int16_t value);
	void writeInt16BE(int16_t value);
	void writeInt16LE(std::size_t index, int16_t value);
	void writeInt16LE(int16_t value);

	uint32_t readUInt32BE(std::size_t index);
	uint32_t readUInt32BE();
	uint32_t readUInt32LE(std::size_t index);
	uint32_t readUInt32LE();

	int32_t readInt32BE(std::size_t index);
	int32_t readInt32BE();
	int32_t readInt32LE(std::size_t index);
	int32_t readInt32LE();

	uint16_t readUInt16BE(std::size_t index);
	uint16_t readUInt16BE();
	uint16_t readUInt16LE(std::size_t index);
	uint16_t readUInt16LE();

	int16_t readInt16BE(std::size_t index);
	int16_t readInt16BE();
	int16_t readInt16LE(std::size_t index);
	int16_t readInt16LE();


private:

	//std::string _bufferString;
	std::vector<uint8_t> _buffer;
	int writeIndex;
	int readIndex;
};


#endif
