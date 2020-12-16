/*
	Copyright 2020, Won Seong-Yeon. All Rights Reserved.
			KoreaGameMaker@gmail.com
			github.com/GameForPeople
*/

/* This file was created and modified by the TypeConverterGenerator. */

#include "stdafx.h"
#include "Types.h"

TypeConverter::TypeConverter()
{
	Init();
	Load();
}

TypeConverter::~TypeConverter()
{
	Init();
}

void TypeConverter::Init()
{
	m_colorCont.clear();
	m_foodCont.clear();
};

void TypeConverter::Load()
{
	// m_colorCont의 각 요소들에 대한 형변환을 정의합니다.
	m_colorCont.emplace( "NONE",  COLOR::NONE  );
	m_colorCont.emplace( "RED",   COLOR::RED   );
	m_colorCont.emplace( "GREEN", COLOR::GREEN );
	m_colorCont.emplace( "BLUE",  COLOR::BLUE  );
	m_colorCont.emplace( "MAX",   COLOR::MAX   );

	// m_foodCont의 각 요소들에 대한 형변환을 정의합니다.
	m_foodCont.emplace( "NONE",   FOOD::NONE   );
	m_foodCont.emplace( "APPLE",  FOOD::APPLE  );
	m_foodCont.emplace( "BANANA", FOOD::BANANA );
	m_foodCont.emplace( "PIZZA",  FOOD::PIZZA  );
	m_foodCont.emplace( "FISH",   FOOD::FISH   );
	m_foodCont.emplace( "MEET",   FOOD::MEET   );
	m_foodCont.emplace( "MAX",    FOOD::MAX    );

};

std::string TypeConverter::COLORToString( COLOR ele ) const
{
	switch ( ele )
	{
		case COLOR::NONE:        { return "NONE";  }
		case COLOR::RED:         { return "RED";   }
		case COLOR::GREEN:       { return "GREEN"; }
		case COLOR::BLUE:        { return "BLUE";  }
		case COLOR::MAX:         { return "MAX";   }
		default:
			WARN_LOG(" Invalid enumType -> COLOR  ...why? " + std::to_string( static_cast< int >( ele ) ) );
			return "";
	}
};

std::string TypeConverter::FOODToString( FOOD ele ) const
{
	switch ( ele )
	{
		case FOOD::NONE:         { return "NONE";   }
		case FOOD::APPLE:        { return "APPLE";  }
		case FOOD::BANANA:       { return "BANANA"; }
		case FOOD::PIZZA:        { return "PIZZA";  }
		case FOOD::FISH:         { return "FISH";   }
		case FOOD::MEET:         { return "MEET";   }
		case FOOD::MAX:          { return "MAX";    }
		default:
			WARN_LOG(" Invalid enumType -> FOOD  ...why? " + std::to_string( static_cast< int >( ele ) ) );
			return "";
	}
};

COLOR TypeConverter::StringToCOLOR( const std::string& str ) const
{
	if ( auto iter = m_colorCont.find( str ); iter != m_a.end() ) 
	{
		return iter.second;
	}

	return COLOR::NONE;
};

FOOD TypeConverter::StringToFOOD( const std::string& str ) const
{
	if ( auto iter = m_foodCont.find( str ); iter != m_a.end() ) 
	{
		return iter.second;
	}

	return FOOD::NONE;
};

