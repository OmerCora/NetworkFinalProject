#ifndef _CGLColourHelper_HG_
#define _CGLColourHelper_HG_

#include <glm\glm.hpp>			// for the vec3
#include <string>
#include <map>
#include <vector>

// This is a singleton because there's a couple large maps of data
//	that are used for the random lookup. Instead of loading this every time,
//	we'll do it only once

class cGLColourHelper
{
public:
	//CColourHelper();
	enum enumColours
	{
		COLOUR_ALICE_BLUE,		COLOUR_ANTIQUE_WHITE,	COLOUR_AQUA,			COLOUR_AQUAMARINE, 
		COLOUR_AZURE,			COLOUR_BEIGE, 			COLOUR_BISQUE,			COLOUR_BLACK, 
		COLOUR_BLANCHED_ALMOND, COLOUR_BLUE,			COLOUR_BLUE_VIOLET, 	COLOUR_BROWN, 
		COLOUR_BURLY_WOOD, 		COLOUR_CADET_BLUE, 		COLOUR_CHARTREUSE, 		COLOUR_CHOCOLATE, 
		COLOUR_CORAL,			COLOUR_CORNFLOWER_BLUE, COLOUR_CORNSILK,		COLOUR_CRIMSON, 
		COLOUR_CYAN,			COLOUR_DARK_BLUE, 		COLOUR_DARK_CYAN,		COLOUR_DARK_GOLDEN_ROD, 
		COLOUR_DARK_GRAY,		COLOUR_DARK_GREEN,		COLOUR_DARK_KHAKI, 		COLOUR_DARK_MAGENTA, 
		COLOUR_DARK_OLIVE_GREEN,	COLOUR_DARK_ORANGE, COLOUR_DARK_ORCHID, 	COLOUR_DARK_RED, 
		COLOUR_DARK_SALMON, 		COLOUR_DARK_SEA_GREEN,		COLOUR_DARK_SLATE_BLUE, 	COLOUR_DARK_SLATE_GRAY, 
		COLOUR_DARK_TURQUOISE, 		COLOUR_DARK_VIOLET, 		COLOUR_DEEP_PINK,			COLOUR_DEEP_SKY_BLUE, 
		COLOUR_DIM_GRAY, 		COLOUR_DODGER_BLUE, 	COLOUR_FIRE_BRICK, 		COLOUR_FLORAL_WHITE, 
		COLOUR_FOREST_GREEN, 	COLOUR_FUCHSIA, 		COLOUR_GAINSBORO, 		COLOUR_GHOST_WHITE, 
		COLOUR_GOLD,			COLOUR_GOLDEN_ROD, 		COLOUR_GRAY,			COLOUR_GREEN, 
		COLOUR_GREEN_YELLOW, 	COLOUR_HONEY_DEW, 		COLOUR_HOT_PINK, 		COLOUR_INDIAN_RED, 
		COLOUR_INDIGO,			COLOUR_IVORY,			COLOUR_KHAKI,			COLOUR_LAVENDER, 
		COLOUR_LAVENDER_BLUSH, 	COLOUR_LAWN_GREEN, 		COLOUR_LEMON_CHIFFON, 	COLOUR_LIGHT_BLUE, 
		COLOUR_LIGHT_CORAL, 	COLOUR_LIGHT_CYAN, 		COLOUR_LIGHT_GOLDEN_ROD_YELLOW, 	COLOUR_LIGHT_GRAY, 
		COLOUR_LIGHT_GREEN, 	COLOUR_LIGHT_PINK, 		COLOUR_LIGHT_SALMON, 		COLOUR_LIGHT_SEA_GREEN, 
		COLOUR_LIGHT_SKY_BLUE, 	COLOUR_LIGHT_SLATE_GRAY, 	COLOUR_LIGHT_STEEL_BLUE, 	COLOUR_LIGHT_YELLOW, 
		COLOUR_LIME,			COLOUR_LIME_GREEN, 		COLOUR_LINEN, 		COLOUR_MAGENTA, 
		COLOUR_MAROON,			COLOUR_MEDIUM_AQUA_MARINE, 		COLOUR_MEDIUM_BLUE, 		COLOUR_MEDIUM_ORCHID, 
		COLOUR_MEDIUM_PURPLE, 	COLOUR_MEDIUM_SEA_GREEN, 		COLOUR_MEDIUM_SLATE_BLUE, 	COLOUR_MEDIUM_SPRING_GREEN, 
		COLOUR_MEDIUM_TURQUOISE, 	COLOUR_MEDIUM_VIOLET_RED, 		COLOUR_MIDNIGHT_BLUE, 		COLOUR_MINT_CREAM, 
		COLOUR_MISTY_ROSE, 		COLOUR_MOCCASIN, 		COLOUR_NAVAJO_WHITE, 		COLOUR_NAVY, 
		COLOUR_OLD_LACE, 		COLOUR_OLIVE, 		COLOUR_OLIVE_DRAB, 		COLOUR_ORANGE, 
		COLOUR_ORANGE_RED, 		COLOUR_ORCHID, 		COLOUR_PALE_GOLDEN_ROD, 		COLOUR_PALE_GREEN, 
		COLOUR_PALE_TURQUOISE, 	COLOUR_PALE_VIOLET_RED, 	COLOUR_PAPAYA_WHIP, 	COLOUR_PEACH_PUFF, 
		COLOUR_PERU, 			COLOUR_PINK, 		COLOUR_PLUM, 		COLOUR_POWDER_BLUE, 
		COLOUR_PURPLE,			COLOUR_REBECCA_PURPLE, 		COLOUR_RED, 		COLOUR_ROSY_BROWN, 
		COLOUR_ROYAL_BLUE, 		COLOUR_SADDLE_BROWN, 		COLOUR_SALMON, 		COLOUR_SANDY_BROWN, 
		COLOUR_SEA_GREEN, 		COLOUR_SEA_SHELL, 		COLOUR_SIENNA, 		COLOUR_SILVER, 
		COLOUR_SKY_BLUE, 		COLOUR_SLATE_BLUE, 		COLOUR_SLATE_GRAY, 		COLOUR_SNOW, 
		COLOUR_SPRING_GREEN, 	COLOUR_STEEL_BLUE, 		COLOUR_TAN, 		COLOUR_TEAL, 
		COLOUR_THISTLE, 		COLOUR_TOMATO, 		COLOUR_TURQUOISE, 		COLOUR_VIOLET, 
		COLOUR_WHEAT, 			COLOUR_WHITE, 		COLOUR_WHITE_SMOKE, 	COLOUR_YELLOW, 
		COLOUR_YELLOW_GREEN,	
		COLOUR_CUSTOM,	/* when the colours are set or random, but not one of the presets */
		COLOUR_UNKNOWN	/* will return the 'unknown colour' */
	};
	class CColour
	{
	public:
		CColour() : enumName(cGLColourHelper::COLOUR_UNKNOWN) {} ;		
		CColour( std::string sName, cGLColourHelper::enumColours colourEnum, glm::vec3 initRGB );
		CColour( std::string sName, cGLColourHelper::enumColours colourEnum, float r, float g, float b );
		std::string name;
		cGLColourHelper::enumColours enumName;
		glm::vec3 rgb;		// Constructor sets to all zeros

	};

	// Singleton 'constructor'
	static cGLColourHelper* getInstance( void );

	glm::vec3 getColourRGB( cGLColourHelper::enumColours colourEnum );
	glm::vec3 getColourRGB( std::string colourName );
	CColour getColour( cGLColourHelper::enumColours colourEnum );
	CColour getColour( std::string colourName );
	
	CColour getRandomColour(void);
	glm::vec3 getRandomColourRGB(void);
	cGLColourHelper::enumColours getRandomColourEnum(void);
	void ShuffleRandomColours(void);

	std::string getColourNameFromEnum( cGLColourHelper::enumColours colourEnum );
	cGLColourHelper::enumColours getColourEnumFromName( std::string colour );


private:
	static cGLColourHelper* m_p_theInstance;
	cGLColourHelper();

	// Loads the "unknown" colour (which is enum COLOUR_UNKNONW, but could be any actual colour (we'll use black)
	CColour m_getUnknownColour(void);

	// This is the 'list' of all the colours, indexed by enum
	std::map< cGLColourHelper::enumColours, cGLColourHelper::CColour >	m_mapColours;
	// This is a "colour name" (aka string) to enum lookup
	std::map< std::string, cGLColourHelper::enumColours >				m_mapColourNameToEnum;

	std::vector< cGLColourHelper::enumColours >				m_vecRandomColourEnumLookup;
	// Index into the array above
	std::vector< cGLColourHelper::enumColours >::size_type	m_nextRandomIndex;		

	template <class T>
	T getRand(T lowRange, T highRange)
	{
		if ( lowRange > highRange )
		{
			T temp = lowRange;
			lowRange = highRange;
			highRange = temp;
		}
		T delta = highRange - lowRange;
		delta = static_cast<T>( ( static_cast<float>( rand() ) / static_cast<float>(RAND_MAX + 1) ) * delta );
		return delta + lowRange;
	}
};

#endif
