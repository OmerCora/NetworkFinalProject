/*
* Program Name : INFO6017_AI_Project4 - cBMPLoader.cpp
* Purpose      : INFO6017 - Artificial Intelligence Project #4
* Description  : LoadBMP will read the BMP file and store the pixel information
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/

#include "cBMPLoader.h"
#include <iostream>
#include <fstream>
#include <iostream>

// These are used to read the data from a file (LoadBMP method).
// * index is incremented appropriate amount (to next char)
// * no bounds checking for array
char cBMPLoader::ReadNextChar(char* data, unsigned long &index)
{
	char returnVal = static_cast<unsigned char>(data[index]);
	index++;
	return returnVal;
}

unsigned short cBMPLoader::ReadNextUnsignedShort(char* data, unsigned long &index)
{
	unsigned short usTheReturnVal = 0;
	usTheReturnVal = this->ShiftAndAdd_Byte_to_UShort(usTheReturnVal, static_cast<unsigned char>(data[index]), 0);
	index++;
	usTheReturnVal = this->ShiftAndAdd_Byte_to_UShort(usTheReturnVal, static_cast<unsigned char>(data[index]), 1);
	index++;
	return usTheReturnVal;
}

unsigned long cBMPLoader::ReadNextUnsignedLong(char* data, unsigned long &index)
{
	unsigned long ulTheReturnVal = 0;
	// Read four bytes, but return them in reverse order.
	ulTheReturnVal = this->ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 0);
	index++;
	ulTheReturnVal = this->ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 1);
	index++;
	ulTheReturnVal = this->ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 2);
	index++;
	ulTheReturnVal = this->ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(data[index]), 3);
	index++;
	return ulTheReturnVal;
}


bool cBMPLoader::LoadBMP( std::string fileName )
{
	std::ifstream theFile;
	if ( this->m_bHave_cout_output )
	{
		//std::cout << "Reading texture file: " << fileName;
	}
	theFile.open(fileName.c_str(), std::ios_base::binary);
	if ( !theFile )
	{
		this->m_lastErrorNum = cBMPLoader::ERORR_FILE_WONT_OPEN;
		std::cout << "Can't find the file"<< fileName << std::endl;
		return false;
	}
	// Scan to the end to get the size
	theFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = theFile.tellg();
	// Return to start
	theFile.seekg(0, std::ios::beg);
		
	char* pRawData = new char[ static_cast<unsigned int>(fileSize) ];
	
	theFile.read( pRawData, fileSize );
	theFile.close();

	// Now go through and decode the BMP file.
	unsigned long curIndex = 0;
	char letter1 = this->ReadNextChar( pRawData, curIndex ); 
	char letter2 = this->ReadNextChar( pRawData, curIndex );
	if ((letter1 != 'B') && (letter2 != 'M'))
	{
		this->m_lastErrorNum = cBMPLoader::ERROR_NOT_A_BMP_FILE;
		return false;
	}
	// File is BMP
	this->m_FileSize = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_reserved1 = this->ReadNextUnsignedShort(pRawData, curIndex);
	this->m_reserved2 = this->ReadNextUnsignedShort(pRawData, curIndex);
	this->m_offsetInBits = this->ReadNextUnsignedLong(pRawData, curIndex);
    this->m_headerSize = this->ReadNextUnsignedLong(pRawData, curIndex);
    this->m_numberOfRows = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_Height = this->m_OriginalHeight = this->m_numberOfRows;
    this->m_numberOfColumns = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_Width = this->m_OriginalWidth = this->m_numberOfColumns;
    this->m_numberOfPlanes = this->ReadNextUnsignedShort(pRawData, curIndex);
    this->m_bitPerPixel = this->ReadNextUnsignedShort(pRawData, curIndex);
	// Is is a 24 bit bitmap?
	if (this->m_bitPerPixel != 24)
	{
		this->m_lastErrorNum = cBMPLoader::ERROR_NOT_A_24_BIT_BITMAP;
		return false;
	}

	this->m_compressionMode = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_imageSizeInBytes = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_PixelsPerMeterX = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_PixelsPerMeterY = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_numberOfLookUpTableEntries = this->ReadNextUnsignedLong(pRawData, curIndex);
	this->m_numberOfImportantColours = this->ReadNextUnsignedLong(pRawData, curIndex);
				
	// 
	long bytesPerRow = ((3 * this->m_numberOfRows + 3) / 4) * 4;
	long numberOfPaddingBytes = bytesPerRow - 3 * this->m_numberOfColumns;

	// Allocate enough space...
	this->m_p_theImages = new C24BitBMPpixel[this->m_numberOfRows * this->m_numberOfColumns];
	
	// Did we run out of memory?
	if (!this->m_p_theImages)
	{
		this->m_lastErrorNum = cBMPLoader::ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP;
		return false;
	}


	// Read the bitmap into memory...
	unsigned long bytesRead = 0;
	unsigned long totalBytesInFile = this->m_numberOfRows * this->m_numberOfColumns;
	unsigned long pixelCount = 0;

	for (unsigned long row = 0; row < m_numberOfRows; row++)
	{
		for (unsigned long col = 0; col < m_numberOfColumns; col++)
		{
			char theblue = this->ReadNextChar(pRawData, curIndex);
			char thegreen = this->ReadNextChar(pRawData, curIndex);
			char thered = this->ReadNextChar(pRawData, curIndex);

			this->m_p_theImages[pixelCount].redPixel = static_cast<unsigned char>(thered);
			this->m_p_theImages[pixelCount].greenPixel = static_cast<unsigned char>(thegreen);
			this->m_p_theImages[pixelCount].bluePixel = static_cast<unsigned char>(theblue);
							//m_p_theImages[pixelCount].alphaPixel = 255;
			pixelCount++;
			bytesRead += 3;	// Add three bytes
		}// for (unsigned long col


		m_NumPixel = pixelCount;


		// We are at the end of the row and there will still be bytes padding up to a multiple of 4...
		// Read and discard these...
		for (int discardedBytes = 0; discardedBytes < numberOfPaddingBytes; discardedBytes++)
		{
			char tempChar = this->ReadNextChar(pRawData, curIndex);
		}
		// How much have we read?
		// NOTE: This really isn't percent, but it does the job of printing out fewer dots.
		int percentRead = static_cast<int>( (static_cast<double>(bytesRead) / static_cast<double>(totalBytesInFile)) * 100);
		if ((percentRead % 25) == 0)
		{
			if ( this->m_bHave_cout_output )
			{
				std::cout << ".";
			}
		}
	}// for (unsigned long row...
	if ( this->m_bHave_cout_output )
	{
		std::cout << "complete." << std::endl;
	}
	// BMP file is read....



	return true;
}

// This is for error handling.
//
int cBMPLoader::GetLastErrorNumber(void)
{
	return m_lastErrorNum;
}


// These functions shift numbers by one, two, and three bytes.
unsigned long cBMPLoader::ShiftAndAdd_Byte_to_ULong(unsigned long theULong, char theByte, int bytesToShift)
{
	// Too many bytes to shift (there are only 4 bytes in a long, so max is 3)?
	if (bytesToShift > 3)
	{
		bytesToShift = 0;
	}
	unsigned short bitsToShift = bytesToShift * 8;	// 8 bits / byte.
	// Now, convert the uChar to a uLong, 
	//	shift it the number of bits,
	//	then or it (add it) to the long
	unsigned char uByte = theByte;
	theULong = theULong | (static_cast<unsigned long>(uByte) << bitsToShift);
	return theULong;
}

unsigned short cBMPLoader::ShiftAndAdd_Byte_to_UShort(unsigned short theUShort, char theByte, int bytesToShift)
{
	// Too many bytes to shift (there is only 2 bytes in a long, so max is 1)?
	if (bytesToShift > 1)
	{
		bytesToShift = 0;
	}
	unsigned short bitsToShift = bytesToShift * 8;	// 8 bits / byte.
	// Now, convert the uChar to a uLong, 
	//	shift it the number of bits,
	//	then or it (add it) to the long
	unsigned char uByte = theByte;
	theUShort = theUShort | (static_cast<unsigned long>(uByte) << bitsToShift);
	return theUShort;
}


// These are used to read the data from a file.
unsigned long cBMPLoader::ReadAnUnsignedLong(std::ifstream& theStream)
{
	unsigned long ulTheReturnVal = 0;
	// Read four bytes, but return them in reverse order.
	char TheByte = 0;
	theStream.get(TheByte);
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(TheByte), 0);
	theStream.get(TheByte);
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(TheByte), 1);
	theStream.get(TheByte);
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(TheByte), 2);
	theStream.get(TheByte);
	ulTheReturnVal = ShiftAndAdd_Byte_to_ULong(ulTheReturnVal, static_cast<unsigned char>(TheByte), 3);
	return ulTheReturnVal;
}


void cBMPLoader::WriteAsUnsignedShort( unsigned short value, std::ofstream& theStream )
{
	// AB --> BA
	char TheByte[2] = {0};
	TheByte[1] = static_cast<char>(( value & 0xFF00 ) >> 8 );
	TheByte[0] = static_cast<char>( value & 0x00FF );

	theStream.put( TheByte[0] );
	theStream.put( TheByte[1] );
}

void cBMPLoader::WriteAsUnsignedLong( unsigned long value, std::ofstream& theStream )
{
	// ABCD --> DCBA
	char TheByte[4] = {0};
	TheByte[3] = static_cast<char>(( value & 0xFF000000 ) >> 24 );
	TheByte[2] = static_cast<char>(( value & 0x00FF0000 ) >> 16 );
	TheByte[1] = static_cast<char>(( value & 0x0000FF00 ) >> 8 );
	TheByte[0] = static_cast<char>( value & 0x000000FF );

	theStream.put( TheByte[0] );
	theStream.put( TheByte[1] );
	theStream.put( TheByte[2] );
	theStream.put( TheByte[3] );
}

unsigned short cBMPLoader::ReadAnUnsignedShort(std::ifstream& theStream)
{
	unsigned short usTheReturnVal = 0;
	// Read two bytes, but reverse the order.
	char TheByte;
	theStream.get(TheByte);
	usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, TheByte, 0);
	theStream.get(TheByte);
	usTheReturnVal = ShiftAndAdd_Byte_to_UShort(usTheReturnVal, TheByte, 1);
	return usTheReturnVal;
}

char cBMPLoader::ReadAByte(std::ifstream& theStream)
{
	char theByte = 0;
	theStream.get(theByte);
	return theByte;
}

bool cBMPLoader::getIsCubeMap(void)
{
	return this->m_bIsCubeMap;
}

bool cBMPLoader::getIs2DTexture(void)
{
	return this->m_bIs2DTexture;
}

// The actual image information
//C24BitBMPpixel m_p_theImages[];	
unsigned long cBMPLoader::GetHeight(void)
{
	return m_Height;
}

unsigned long cBMPLoader::GetWidth(void)
{
	return m_Width;
}

unsigned long cBMPLoader::GetOriginalHeight(void)
{
	return m_OriginalHeight;
}

unsigned long cBMPLoader::GetOriginalWidth(void)
{
	return m_OriginalWidth;
}

unsigned long cBMPLoader::GetFileSize(void)
{
	return m_FileSize;
}

unsigned long cBMPLoader::GetFileSizeInBytes(void)
{
	return m_FileSize;
}

unsigned short cBMPLoader::GetReserved1(void)
{
	return m_reserved1;
}

unsigned short cBMPLoader::GetReserved2(void)
{
	return m_reserved2;
}

unsigned long cBMPLoader::GetImageSizeInBytes(void)
{
	return m_imageSizeInBytes;
}

unsigned long cBMPLoader::GetBitsPerPixel(void)
{
	return m_bitPerPixel;
}

unsigned long cBMPLoader::GetOffsetInBits(void)
{
	return m_offsetInBits;
}

unsigned long cBMPLoader::GetCompressionMode(void)
{
	return m_compressionMode;
}

unsigned long cBMPLoader::GetPixelsPerMeterX(void)
{
	return m_PixelsPerMeterX;
}

unsigned long cBMPLoader::GetPixelsPerMeterY(void)
{
	return m_PixelsPerMeterY;
}

unsigned long cBMPLoader::GetNumberOfLookUpTables(void)
{
	return m_numberOfLookUpTableEntries;
}

unsigned long cBMPLoader::GetNumberOfImportantColours(void)
{
	return m_numberOfImportantColours;
}


cBMPLoader::cBMPLoader()
	: m_Height(0), m_Width(0), m_OriginalHeight(0), m_OriginalWidth(0), m_p_theImages(0),
	m_lastErrorNum(0), m_FileSize(0), m_offsetInBits(0), m_headerSize(0),
	m_numberOfRows(0), m_numberOfColumns(0), m_numberOfPlanes(0),
	m_bitPerPixel(0), m_compressionMode(0), m_imageSizeInBytes(0),
	m_PixelsPerMeterX(0), m_PixelsPerMeterY(0),
	m_numberOfLookUpTableEntries(0), m_numberOfImportantColours(0),
	m_bIsCubeMap(false), m_bIs2DTexture(false), m_NumPixel(0)
{
	return;
}

cBMPLoader::~cBMPLoader()
{
	return;
}

