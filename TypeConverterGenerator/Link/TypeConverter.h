/*
	Copyright 2020, Won Seong-Yeon. All Rights Reserved.
			KoreaGameMaker@gmail.com
			github.com/GameForPeople
*/

/* This file was created and modified by the TypeConverterGenerator. */

#pragma once

#include "../Util/BaseSingleton.hpp"

class TypeConverter
	: public WonSY::BASE::TSingleton< TypeConverter >
{
public:
	// 생성자
	TypeConverter();

	// 소멸자
	~TypeConverter();

public:
	// 초기화합니다.
	void Init();

	// 로드합니다.
	void Load();

public:
	// COLOR 값을 String으로 변경하는 함수입니다. 
	std::string COLORToString( COLOR ) const;

	// FOOD 값을 String으로 변경하는 함수입니다. 
	std::string FOODToString( FOOD ) const;

public:
	// String을 COLOR 값으로 변경하는 함수입니다. 
	COLOR StringToCOLOR( const std::string& str ) const;

	// String을 FOOD 값으로 변경하는 함수입니다. 
	FOOD StringToFOOD( const std::string& str ) const;

private:
	std::map< std::string, COLOR >  m_colorCont; // 스트링을 COLOR으로 변경하기 위해 필요한 맵입니다. 
	std::map< std::string, FOOD >   m_foodCont;  // 스트링을 FOOD으로 변경하기 위해 필요한 맵입니다. 
};
