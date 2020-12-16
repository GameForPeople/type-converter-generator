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
	// ������
	TypeConverter();

	// �Ҹ���
	~TypeConverter();

public:
	// �ʱ�ȭ�մϴ�.
	void Init();

	// �ε��մϴ�.
	void Load();

public:
	// COLOR ���� String���� �����ϴ� �Լ��Դϴ�. 
	std::string COLORToString( COLOR ) const;

	// FOOD ���� String���� �����ϴ� �Լ��Դϴ�. 
	std::string FOODToString( FOOD ) const;

public:
	// String�� COLOR ������ �����ϴ� �Լ��Դϴ�. 
	COLOR StringToCOLOR( const std::string& str ) const;

	// String�� FOOD ������ �����ϴ� �Լ��Դϴ�. 
	FOOD StringToFOOD( const std::string& str ) const;

private:
	std::map< std::string, COLOR >  m_colorCont; // ��Ʈ���� COLOR���� �����ϱ� ���� �ʿ��� ���Դϴ�. 
	std::map< std::string, FOOD >   m_foodCont;  // ��Ʈ���� FOOD���� �����ϱ� ���� �ʿ��� ���Դϴ�. 
};
