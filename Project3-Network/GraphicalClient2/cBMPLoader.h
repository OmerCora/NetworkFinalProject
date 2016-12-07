/*
* Program Name : INFO6017_AI_Project4 - cBMPLoader.h
* Purpose      : INFO6017 - Artificial Intelligence Project #4
* Description  : act as library class for to get pixel information from BMP file
* Date         : December 8, 2016
* Coder        : Jaehuhn Park
*/

#ifndef _CTextureFromBMP_HEADER_GUARD_
#define _CTextureFromBMP_HEADER_GUARD_

#include <fstream>
#include <string>
#include "Texture\C24BitBMPpixel.h"

typedef unsigned char uchar;

class cBMPLoader
{
public:
	cBMPLoader();
	~cBMPLoader();

	bool LoadBMP( std::string fileName );		
	
	int GetLastErrorNumber(void);
	// Some getters.
	unsigned long GetHeight(void);
	unsigned long GetWidth(void);
	unsigned long GetOriginalHeight(void);
	unsigned long GetOriginalWidth(void);
	unsigned long GetFileSize(void);
	unsigned long GetFileSizeInBytes(void);
	unsigned short GetReserved1(void);
	unsigned short GetReserved2(void);
	unsigned long GetBitsPerPixel(void);
	unsigned long GetOffsetInBits(void);
	unsigned long GetImageSizeInBytes(void);
	unsigned long GetCompressionMode(void);
	unsigned long GetPixelsPerMeterX(void);
	unsigned long GetPixelsPerMeterY(void);
	unsigned long GetNumberOfLookUpTables(void);
	unsigned long GetNumberOfImportantColours(void);
	// 
	bool getIsCubeMap(void);
	bool getIs2DTexture(void);

	// The actual image information
	C24BitBMPpixel* m_p_theImages;
	int m_NumPixel;

private:
	std::string m_textureName;
	std::string m_fileNameFullPath;	
	bool m_bIsCubeMap;
	bool m_bIs2DTexture;
	unsigned long m_Height;
	unsigned long m_Width;
	unsigned long m_OriginalHeight;
	unsigned long m_OriginalWidth;
	unsigned short m_reserved1;
	unsigned short m_reserved2;
	unsigned long m_FileSize;
	unsigned long m_offsetInBits;
	unsigned long m_headerSize;
	unsigned long m_numberOfRows;
	unsigned long m_numberOfColumns;
	unsigned short m_numberOfPlanes;
	unsigned short m_bitPerPixel;	// 1, 4, 8, 15, 24, 32, or 64
	unsigned long m_compressionMode;	// 0 if NOT compressed (i.e. 24 bit bitmap)
	unsigned long m_imageSizeInBytes;	// Total bytes in image - may be zero
	unsigned long m_PixelsPerMeterX;
	unsigned long m_PixelsPerMeterY;
	unsigned long m_numberOfLookUpTableEntries;	// Could be 0 for 24 bit format
	unsigned long m_numberOfImportantColours;	// 0 if all colours are important
	// These functions shift numbers by one, two, and three bytes.
	unsigned long ShiftAndAdd_Byte_to_ULong(unsigned long theULong, char theByte, int bytesToShift);
	unsigned short ShiftAndAdd_Byte_to_UShort(unsigned short theUShort, char theByte, int bytesToShift);
	// These are used to read the data from a file (LoadBMP method).
	unsigned long ReadAnUnsignedLong(std::ifstream& theStream);
	unsigned short ReadAnUnsignedShort(std::ifstream& theStream);
	char ReadAByte(std::ifstream& theStream);
	// These are used to read the data from a file (LoadBMP method).
	// * index is incremented appropriate amount (to next char)
	// * no bounds checking for array
	unsigned long ReadNextUnsignedLong(char* data, unsigned long &index);
	unsigned short ReadNextUnsignedShort(char* data, unsigned long &index);
	char ReadNextChar(char* data, unsigned long &index);
	//
	void WriteAsUnsignedShort( unsigned short value, std::ofstream& theStream );
	void WriteAsUnsignedLong( unsigned long value, std::ofstream& theStream );


	// This is for error handling:
	int m_lastErrorNum;
	enum ERORRCONSTANTS
	{
		ERROR_NO_ERROR = 0,
		ERORR_FILE_WONT_OPEN = 1,
		ERROR_NOT_A_BMP_FILE = 2,
		ERROR_NOT_A_24_BIT_BITMAP = 3,
		ERROR_NOT_ENOUGHT_MEMORY_FOR_BITMAP = 4
	};
	//
	bool m_bHave_cout_output;
};

#endif
