#include "cBuffer.h"
#include <stdio.h>


cBuffer::cBuffer()
{
	_buffer.resize(cBuffer::mc_DEFAULT_BUFFER_LENGTH);
	writeIndex = 0;
	readIndex = 0;
}

cBuffer::~cBuffer()
{
}


//Reset read and write index and clear buffer
void cBuffer::clear()
{
	writeIndex = 0;
	readIndex = 0;
}
void cBuffer::CheckBufferSize(std::size_t size)
{
	//Grow buffer if new value will pass current size
	if (size + writeIndex > _buffer.size())
		_buffer.resize(_buffer.size()+ size);
}
//Load this buffer from the static buffer used in recieve function
void cBuffer::loadBuffer(const char* recvbuf, size_t size)
{
	//Grow buffer if new value will pass current size
	this->CheckBufferSize(size);

	//Add each byte from the static buffer to class memeber buffer
	for (size_t i = 0; i < size; ++i)
	{
		_buffer.at(writeIndex) = recvbuf[i];
		writeIndex++;
	}
}
void cBuffer::Append(cBuffer& buffer)
{
	this->loadBuffer(buffer.toCharArray(), buffer.getLength());
}

void cBuffer::flushBuffer()
{
	if (this->isEmpty())
	{
		writeIndex = 0;
		readIndex = 0;
	}
	else
	{
		// shift 
		for (int i = 0; i < writeIndex - readIndex; ++i)
		{
			_buffer.at(i) = _buffer[i + readIndex];
		}

		writeIndex -= readIndex;
		readIndex = 0;
	}
}

//Translate buffer to const char* for send function
const char* cBuffer::toCharArray()
{
	return (const char*)&_buffer[readIndex];
}


bool cBuffer::isEmpty()
{
	return (this->getLength() == 0);
}
std::size_t cBuffer::getLength() const
{
	return writeIndex - readIndex;
}

std::size_t cBuffer::getReadIndex() const
{
	return readIndex;
}

std::size_t cBuffer::getWriteIndex() const
{
	return writeIndex;
}

void cBuffer::setReadIndex(std::size_t index)
{
	readIndex = index;
}

void cBuffer::setWriteIndex(std::size_t index)
{
	writeIndex = index;
}

void cBuffer::printInHex()
{
	for (std::vector<uint8_t>::iterator it = _buffer.begin(); it != _buffer.end(); ++it)
	{
		printf("%02x ", *it);
	}
}

///////////////////////////////////////////////
// begin serialize
void cBuffer::Serialize(const sProtocolHeader& data)
{
	this->writeInt32BE(data.packet_length);
	this->writeInt32BE(data.packet_id);
}
void cBuffer::Serialize(const sProtocolRoom& data)
{
	this->writeInt16BE(data.roomname_length);
	this->writeStringBE(data.roomname);
}
void cBuffer::Serialize(const sProtocolResponseRoomList& data)
{
	this->writeInt16BE(data.numberOfRooms);
	for (int i = 0; i < (int)data.rooms.size();++i)
	{
		this->Serialize(data.rooms[i]);
		this->writeInt16BE(data.user_counts[i]);
	}
}
void cBuffer::Serialize(const sProtocolRequestNewRoom& data)
{
	this->Serialize((const sProtocolRoom&)data);
	this->Serialize((const sProtocolUser&)data);
}
void cBuffer::Serialize(const sProtocolRequestJoinRoom& data)
{
	this->Serialize((const sProtocolRoom&)data);
	this->Serialize((const sProtocolUser&)data);
}
void cBuffer::Serialize(const sProtocolRequestLeaveRoom& data)
{
	this->Serialize((const sProtocolRoom&)data);
	this->Serialize((const sProtocolUser&)data);
}
void cBuffer::Serialize(const sProtocolUser& data)
{
	this->writeInt16BE(data.username_length);
	this->writeStringBE(data.username);
}
void cBuffer::Serialize(const sProtocolPassword& data)
{
	this->writeInt16BE(data.password_length);
	this->writeStringBE(data.password);
}
void cBuffer::Serialize(const sProtocolResponseRoomInfo& data)
{
	this->Serialize((const sProtocolRoom&)data);
	
	this->writeInt16BE(data.numberOfUsers);
	for (int i = 0; i < (int)data.users.size(); ++i)
	{
		this->Serialize(data.users[i]);
	}
}
void cBuffer::Serialize(const sProtocolChatMessage& data)
{
	this->writeInt32BE(data.message_length);
	this->writeStringBE(data.message);
}
void cBuffer::Serialize(const sProtocolSendChatMessage& data)
{
	this->Serialize((const sProtocolUser&)data);

	this->writeInt16BE(data.roomname_length);
	this->writeStringBE(data.roomname);

	this->Serialize((const sProtocolChatMessage&)data);
}
void cBuffer::Serialize(const sProtocolResponseChatHistory& data)
{
	this->writeInt32BE(data.numberOfMessages);
	for (int i = 0; i < (int)data.chatHistory.size(); ++i)
	{
		this->Serialize(data.chatHistory[i]);
	}
}
void cBuffer::Serialize(const sProtocolLobbyUsers& data)
{
	this->writeInt16BE(data.numberOfUsers);
	for (int i = 0; i < (int)data.users.size(); ++i)
	{
		this->Serialize(data.users[i]);
	}
}
void cBuffer::Serialize(const sProtocolResponseFailure& data)
{
	this->writeInt16BE(data.message_length);
	this->writeStringBE(data.errormessage);
	this->writeInt32BE(data.failureCode);
}
void cBuffer::Serialize(const sProtocolAccount& data)
{
	this->Serialize((const sProtocolUser&)data);
	this->Serialize((const sProtocolPassword&)data);
}

///////////////////////////////////////////////
// begin deserialize
void cBuffer::Deserialize(sProtocolHeader& data)
{
	data.packet_length = this->readInt32BE();
	data.packet_id = (sProtocolHeader::ePacketID)this->readInt32BE();
	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolRoom& data)
{
	data.roomname_length = this->readInt16BE();
	data.roomname = this->readStringBE(data.roomname_length);
	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolResponseRoomList& data)
{
	data.numberOfRooms = this->readInt16BE();
	data.rooms.resize(data.numberOfRooms);
	data.user_counts.resize(data.numberOfRooms);
	for (int i = 0; i < data.numberOfRooms; ++i)
	{
		this->Deserialize(data.rooms[i]);
		data.user_counts[i] = this->readInt16BE();
	}
	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolRequestNewRoom& data)
{
	this->Deserialize((sProtocolRoom&)data);
	this->Deserialize((sProtocolUser&)data);

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolRequestJoinRoom& data)
{
	this->Deserialize((sProtocolRoom&)data);
	this->Deserialize((sProtocolUser&)data);

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolRequestLeaveRoom& data)
{
	this->Deserialize((sProtocolRoom&)data);
	this->Deserialize((sProtocolUser&)data);

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolUser& data)
{
	data.username_length = this->readInt16BE();
	data.username = this->readStringBE(data.username_length);

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolPassword& data)
{
	data.password_length = this->readInt16BE();
	data.password = this->readStringBE(data.password_length);

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolResponseRoomInfo& data)
{
	this->Deserialize((sProtocolRoom&)data);

	data.numberOfUsers = this->readInt16BE();
	data.users.resize(data.numberOfUsers);
	for (int i = 0; i < data.numberOfUsers; ++i)
	{
		this->Deserialize(data.users[i]);
	}
	this->flushBuffer();
}

void cBuffer::Deserialize(sProtocolChatMessage& data)
{
	data.message_length = this->readInt32BE();
	data.message = this->readStringBE(data.message_length);

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolSendChatMessage& data)
{
	this->Deserialize((sProtocolUser&)data);
	this->Deserialize((sProtocolRoom&)data);
	this->Deserialize((sProtocolChatMessage&)data);

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolResponseChatHistory& data)
{
	data.numberOfMessages = this->readInt32BE();
	data.chatHistory.resize(data.numberOfMessages);
	for (int i = 0; i < data.numberOfMessages; ++i)
	{
		this->Deserialize(data.chatHistory[i]);
	}

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolLobbyUsers& data)
{
	data.numberOfUsers = this->readInt16BE();
	data.users.resize(data.numberOfUsers);
	for (int i = 0; i < data.numberOfUsers; ++i)
	{
		this->Deserialize(data.users[i]);
	}

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolResponseFailure& data)
{
	data.message_length = this->readInt16BE();
	data.errormessage = this->readStringBE(data.message_length);
	data.failureCode = (sProtocolResponseFailure::eFailureCode)this->readInt32BE();

	this->flushBuffer();
}
void cBuffer::Deserialize(sProtocolAccount& data)
{
	this->Deserialize((sProtocolUser&)data);
	this->Deserialize((sProtocolPassword&)data);

	this->flushBuffer();
}


/************************************************
// Protocols for Game Monopoly
************************************************/
void cBuffer::Serialize(const sProtocolMonopolyHeader& data)
{
	this->writeInt32BE(data.packet_length);
	this->writeInt32BE(data.packet_id);
}
void cBuffer::Deserialize(sProtocolMonopolyHeader& data)
{
	data.packet_length = this->readInt32BE();
	data.packet_id = (sProtocolMonopolyHeader::ePacketID)this->readInt32BE();
	this->flushBuffer();
}
void cBuffer::Serialize(const sProtocolResponseGameStart& data)
{
	this->writeChar(data.isMyTurn);
}
void cBuffer::Deserialize(sProtocolResponseGameStart& data)
{
	data.isMyTurn = this->readChar();
}
void cBuffer::Serialize(const sProtocolRequestPlayThrowDice& data)
{
}
void cBuffer::Deserialize(sProtocolRequestPlayThrowDice& data)
{
}
void cBuffer::Serialize(const sProtocolResponsePlayThrowDice& data)
{
	this->writeInt16BE(data.nextLocation);
}
void cBuffer::Deserialize(sProtocolResponsePlayThrowDice& data)
{
	data.nextLocation = this->readInt16BE();
}
void cBuffer::Serialize(const sProtocolRequestPlayAction& data)
{
}
void cBuffer::Deserialize(sProtocolRequestPlayAction& data)
{
}
void cBuffer::Serialize(const sProtocolResponsePlayAction& data)
{
	this->writeInt16BE(data.districtType);
}
void cBuffer::Deserialize(sProtocolResponsePlayAction& data)
{
	data.districtType = this->readInt16BE();
}
void cBuffer::Serialize(const sProtocolAskAssetAction& data)
{
	this->writeInt16BE(data.districtType);
}
void cBuffer::Deserialize(sProtocolAskAssetAction& data)
{
	data.districtType = this->readInt16BE();
}
void cBuffer::Serialize(const sProtocolAnswerAssetAction& data)
{
	this->writeChar(data.yesOrNo);
}
void cBuffer::Deserialize(sProtocolAnswerAssetAction& data)
{
	data.yesOrNo = this->readChar();
}
void cBuffer::Serialize(const sProtocolResponsePlayTurnChange& data)
{
}
void cBuffer::Deserialize(sProtocolResponsePlayTurnChange& data)
{
}
void cBuffer::Serialize(const sProtocolResponsePlayTurnKeep& data)
{
}
void cBuffer::Deserialize(sProtocolResponsePlayTurnKeep& data)
{
}
void cBuffer::Serialize(const sProtocolResponseGameFinish& data)
{
}
void cBuffer::Deserialize(sProtocolResponseGameFinish& data)
{
}
void cBuffer::Serialize(const sProtocolResponseGameOver& data)
{
}
void cBuffer::Deserialize(sProtocolResponseGameOver& data)
{
}

//////////////////////////////////////////////////
// functions
void cBuffer::writeChar(char value)
{
	_buffer[writeIndex] = value;
	writeIndex++;
}
char cBuffer::readChar()
{
	return _buffer[readIndex];
	readIndex++;
}

void cBuffer::writeStringBE(const std::string& value)
{
	writeStringBE(writeIndex, value);
	writeIndex += value.size();
}

void cBuffer::writeStringBE(std::size_t index, const std::string& value)
{
	for (size_t i = 0; i < value.size(); i++)
	{
		_buffer.at(index++) = value.at(i);
	}
}

std::string cBuffer::readStringBE(std::size_t stringLength)
{
	std::string  value = readStringBE(readIndex, stringLength);
	readIndex += value.size();
	return value;
}

std::string cBuffer::readStringBE(std::size_t index, std::size_t stringLength) 
{
	std::string value;

	for (size_t i = 0; i < stringLength; i++)
	{
		char temp = _buffer[index++];
		value.append(1,temp);
	}

	return value;
}




void cBuffer::writeUInt32BE(std::size_t index, uint32_t value)
{
	_buffer[index++] = value >> 24;
	_buffer[index++] = value >> 16;
	_buffer[index++] = value >> 8;
	_buffer[index++] = value;
}
void cBuffer::writeUInt32BE(uint32_t value)
{
	writeUInt32BE(writeIndex, value);
	writeIndex += 4;
}
void cBuffer::writeUInt32LE(std::size_t index, uint32_t value)
{
	_buffer[index++] = value;
	_buffer[index++] = value >> 8;
	_buffer[index++] = value >> 16;
	_buffer[index++] = value >> 24;
}
void cBuffer::writeUInt32LE(uint32_t value)
{
	writeUInt32LE(writeIndex, value);
	writeIndex += 4;
}

void cBuffer::writeInt32BE(std::size_t index, int32_t value)
{
	_buffer[index++] = value >> 24;
	_buffer[index++] = value >> 16;
	_buffer[index++] = value >> 8;
	_buffer[index++] = value;
}
void cBuffer::writeInt32BE(int32_t value)
{
	writeInt32BE(writeIndex, value);
	writeIndex += 4;
}
void cBuffer::writeInt32LE(std::size_t index, int32_t value)
{
	_buffer[index++] = value;
	_buffer[index++] = value >> 8;
	_buffer[index++] = value >> 16;
	_buffer[index++] = value >> 24;
}
void cBuffer::writeInt32LE(int32_t value)
{
	writeInt32LE(writeIndex, value);
	writeIndex += 4;
}

void cBuffer::writeUInt16BE(std::size_t index, uint16_t value)
{
	_buffer[index++] = (uint8_t)(value >> 8);
	_buffer[index++] = (uint8_t)(value);
}
void cBuffer::writeUInt16BE(uint16_t value)
{
	writeUInt16BE(writeIndex, value);
	writeIndex += 2;
}
void cBuffer::writeUInt16LE(std::size_t index, uint16_t value)
{
	_buffer[index++] = (uint8_t)(value);
	_buffer[index++] = (uint8_t)(value >> 8);
}
void cBuffer::writeUInt16LE(uint16_t value)
{
	writeUInt16LE(writeIndex, value);
	writeIndex += 2;
}

void cBuffer::writeInt16BE(std::size_t index, int16_t value)
{
	_buffer[index++] = (uint8_t)(value >> 8);
	_buffer[index++] = (uint8_t)(value);
}
void cBuffer::writeInt16BE(int16_t value)
{
	writeInt16BE(writeIndex, value);
	writeIndex += 2;
}
void cBuffer::writeInt16LE(std::size_t index, int16_t value)
{
	_buffer[index++] = (uint8_t)(value);
	_buffer[index++] = (uint8_t)(value >> 8);
}
void cBuffer::writeInt16LE(int16_t value)
{
	writeInt16LE(writeIndex, value);
	writeIndex += 2;
}

uint32_t cBuffer::readUInt32BE(std::size_t index)
{
	uint32_t value = 0;
	value |= _buffer[index++] << 24;
	value |= _buffer[index++] << 16;
	value |= _buffer[index++] << 8;
	value |= _buffer[index++];
	return value;
}
uint32_t cBuffer::readUInt32BE()
{
	uint32_t value = readUInt32BE(readIndex);
	readIndex += 4;
	return value;
}
uint32_t cBuffer::readUInt32LE(std::size_t index)
{
	uint32_t value = 0;
	value |= _buffer[index++];
	value |= _buffer[index++] << 8;
	value |= _buffer[index++] << 16;
	value |= _buffer[index++] << 24;
	return value;
}
uint32_t cBuffer::readUInt32LE()
{
	uint32_t value = readUInt32LE(readIndex);
	readIndex += 4;
	return value;
}

int32_t cBuffer::readInt32BE(std::size_t index)
{
	int32_t value = 0;
	value |= _buffer[index++] << 24;
	value |= _buffer[index++] << 16;
	value |= _buffer[index++] << 8;
	value |= _buffer[index++];
	return value;
}
int32_t cBuffer::readInt32BE()
{
	int32_t value = readInt32BE(readIndex);
	readIndex += 4;
	return value;
}
int32_t cBuffer::readInt32LE(std::size_t index)
{
	int32_t value = 0;
	value |= _buffer[index++];
	value |= _buffer[index++] << 8;
	value |= _buffer[index++] << 16;
	value |= _buffer[index++] << 24;
	return value;
}
int32_t cBuffer::readInt32LE()
{
	int32_t value = readInt32LE(readIndex);
	readIndex += 4;
	return value;
}

uint16_t cBuffer::readUInt16BE(std::size_t index)
{
	uint16_t value = 0;
	value |= _buffer[index++] << 8;
	value |= _buffer[index++];
	return value;
}
uint16_t cBuffer::readUInt16BE()
{
	uint16_t value = readUInt16BE(readIndex);
	readIndex += 2;
	return value;
}
uint16_t cBuffer::readUInt16LE(std::size_t index)
{
	uint16_t value = 0;
	value |= _buffer[index++];
	value |= _buffer[index++] << 8;
	return value;
}
uint16_t cBuffer::readUInt16LE()
{
	uint16_t value = readUInt16LE(readIndex);
	readIndex += 2;
	return value;
}

int16_t cBuffer::readInt16BE(std::size_t index)
{
	int16_t value = 0;
	value |= _buffer[index++] << 8;
	value |= _buffer[index++];
	return value;
}
int16_t cBuffer::readInt16BE()
{
	int16_t value = readInt16BE(readIndex);
	readIndex += 2;
	return value;
}
int16_t cBuffer::readInt16LE(std::size_t index)
{
	int16_t value = 0;
	value |= _buffer[index++];
	value |= _buffer[index++] << 8;
	return value;
}
int16_t cBuffer::readInt16LE()
{
	int16_t value = readInt16LE(readIndex);
	readIndex += 2;
	return value;
}