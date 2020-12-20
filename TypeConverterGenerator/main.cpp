/*
	Copyright 2020, Won Seong-Yeon. All Rights Reserved.
		KoreaGameMaker@gmail.com
		github.com/GameForPeople
*/

#ifndef WONSY_PCH
#include <iostream>
#include <chrono>
#include <thread>

#include <vector>
#include <map>

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

// ������ ����
struct TypeData
{
	std::string                enumName;
	//std::string              enumType;
	std::vector< std::string > dataNameCont;
};

std::vector< std::string > DoTokenize ( const std::string& stringValue, const char delimiter = ' ');
void                       InfoLog    ( const std::string& msg                                    );
void                       WarnLog    ( const std::string& msg                                    );
void                       ErrorLog   ( const std::string& msg                                    );

int main( int argc, char *argv[] )
{
	std::cout << ""                                                     << std::endl;
	std::cout << "Type Converter Generator ver 0.1"                     << std::endl;
	std::cout << "Copyright 2020, Won Seong-Yeon. All Rights Reserved." << std::endl;
	std::cout << "		KoreaGameMaker@gmail.com"                       << std::endl;
	std::cout << "		github.com/GameForPeople"                       << std::endl;
	std::cout << "" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "" << std::endl;

	InfoLog( "Run Type Converter Generator!" );

	// ���� �˻�
	if ( argc < 5 ) 
	{
		WarnLog( "argc is under 5, use default argv!" );

		argv[1] = (char *)("Link");
		argv[2] = (char *)("Link/Types.h");
		argv[3] = (char *)("Link/TypeConverter.h");
		argv[4] = (char *)("Link/TypeConverter.cpp");
	}

	std::map< std::string /* => TypeData Class Name */, TypeData > typeDataCont;

	const std::filesystem::path linkPath           = std::string( argv[1] );
	const std::filesystem::path sourceFilePath     = std::string( argv[2] );
	const std::filesystem::path destHeaderFilePath = std::string( argv[3] );
	const std::filesystem::path destSourceFilePath = std::string( argv[4] );

	// ��ũ �˻�
	{
		// �ҽ� ��ũ �˻�
		if ( !std::filesystem::exists( linkPath ) )
		{
			ErrorLog( "����! �ش� ��ũ�� �������� ����! " + linkPath.generic_string() );
			return 0;
		}

		// �ҽ� ��ũ �˻�
		if ( !std::filesystem::exists( sourceFilePath ) )
		{
			ErrorLog( "����! �ش� �ҽ� ������ �������� ����! " + sourceFilePath.generic_string() );
			return 0;
		}
	}

	// ���� �ε� �� �Ľ�
	{
		const std::string sourceFileFullName  = sourceFilePath.filename().generic_string();
		const std::string sourceFileExtension = sourceFilePath.extension().generic_string();
		const std::string sourceFileName      = sourceFilePath.filename().stem().generic_string();

		std::ifstream sourceFileStream( sourceFilePath );
		if ( !sourceFileStream.is_open() )
		{
			ErrorLog( "���� �𸣰ڴµ� source ���� ���¿� �ϴ� �����߽��ϴ�. : " + sourceFileFullName );
		}

		TypeData    tempTypeData;
		std::string lineBuffer   = "";
		bool        isOnReadEnum = false;

		// ��� ���� �о� TypeData�� �Ľ��Ѵ�.
		while ( 7777777 )
		{
			if ( sourceFileStream.eof() )
				break;

			std::getline( sourceFileStream, lineBuffer );

			std::vector< std::string > wordCont = DoTokenize( lineBuffer );
			if ( wordCont.size() == 0 )
				continue;

			// Enum�� �а� �ִ� �� ���ο� ���� �ٸ��� ó����
			if ( !isOnReadEnum )
			{
				if ( wordCont[0] != "enum" )
					continue;

				tempTypeData.enumName = wordCont.size() > 2 ? wordCont[2] : wordCont[1];
				isOnReadEnum          = true;
			}
			else
			{
				if ( wordCont.size() == 0 )
				{
					WarnLog( "����? " + tempTypeData.enumName );
					continue;
				}
				else if ( wordCont[0][0] == '{' )
				{
					continue;
				}
				else if ( wordCont[0][0] == '}' )
				{
					isOnReadEnum = false;
					typeDataCont.emplace( tempTypeData.enumName, tempTypeData );
					tempTypeData = TypeData();
					continue;
				}

				// ���� /t ���ڸ� �����ϰ�, ���� , �� ���� ������ ��, Data Name�� �ִ´�.
				wordCont[0].erase( wordCont[0].begin() );
				tempTypeData.dataNameCont.emplace_back( DoTokenize( wordCont[0] ,',')[0] ); 
			}
		}
	}

	// file Generate!
	{
		// �ּ��� �ۼ��Ѵ�.
		auto WriteComment = []( std::ofstream& fos ) -> void
		{
			fos 
			<< "/*"                                                                      << std::endl
			<< "	Copyright 2020, Won Seong-Yeon. All Rights Reserved."                << std::endl
			<< "			KoreaGameMaker@gmail.com"                                    << std::endl
			<< "			github.com/GameForPeople"                                    << std::endl
			<< "*/"                                                                      << std::endl
			<< ""                                                                        << std::endl
			<< "/* This file was created and modified by the TypeConverterGenerator. */" << std::endl
			<< ""                                                                        << std::endl;
		};

		// ������ ������ ���߱� ����, ��ü ���� ���� �Է��ϸ� ��Ʈ���� ���̿� �°� ������ ���� ��Ʈ���� �����Ѵ�.
		auto MSF /*MakeSpaceFormat*/ = []( const std::string& str, int spaceCount ) -> std::string
		{
			if ( str.size() > spaceCount ) 
			{ 
				WarnLog( "Over Space Count " + str ); 
				return str;
			}

			std::string retStr = str;

			for ( int i = 0; i < spaceCount - str.size(); ++i ) { retStr.append(" "); }

			return retStr;
		};

		// �ձ��ڰ� �ҹ����� ��Ʈ������ �����Ͽ� ��ȯ�ϴ� �Լ�
		auto MFL /*MakeFirstLower*/ = []( const std::string& str ) -> std::string
		{  
			std::string retStr = str;
			if ( retStr[ 0 ] < 'a' ) { retStr[ 0 ] = retStr[ 0 ] + ( 'a' - 'A' ); }

			return retStr;
		};

		// ��� ���ڸ� �ҹ����� ��Ʈ������ �����Ͽ� ��ȯ�ϴ� �Լ�
		auto MAL /*MakeFirstLower*/ = []( const std::string& str ) -> std::string
		{  
			std::string retStr = str;

			for ( auto& ch : retStr )
			{
				if ( 'A'<= ch && ch < 'a' ) { ch = ch + ( 'a' - 'A' ); }
			}

			return retStr;
		};

		// �Է¹��� ��Ʈ�� �����̳� �߿��� ���� �̸��� �� ��Ʈ���� ����� ��ȯ�ϴ� �Լ�
		auto GetMaxLength = [ /* void */ ]( const auto& Cont ) noexcept -> int 
		{
			int maxLength = 0;

			for ( const auto& ele : Cont )
			{
				const std::string str = []( const auto& ele ) noexcept -> const std::string
				{
					if constexpr ( std::is_same< const std::vector<std::string>&, decltype( Cont ) >::value )
					{
						return ele;
					}
					else if constexpr ( std::is_same< const std::map< std::string, TypeData >&, decltype( Cont ) >::value )
					{
						return ele.first;
					}
				}( ele );

				if ( str.size() > maxLength ) 
				{
					maxLength = static_cast<int>( str.size() );
				}
			}

			return maxLength;
		};

		// TypeConverter.h
		{
			      std::ofstream fos      ( destHeaderFilePath );
			const std::string   className( "TypeConverter"    );

			WriteComment( fos );

			// ��ũ�� ���� ���Ͽ� ����.
			{
				fos 
				<< "#pragma once" << std::endl
				<< ""             << std::endl;
			}

			// ����� ���Ͽ� ����.
			{
				fos 
				<< "#include \"../../Util/BaseSingleton.hpp\""                         << std::endl
				<< "#include \"" + sourceFilePath.filename().generic_string() + "\""   << std::endl
				<< ""             << std::endl;
			}

			// ���� �̸� �� ���, ������, �Ҹ��ڸ� �����Ѵ�.
			{
				fos	
				<< "class " << className                                        << std::endl
				<< "	: public WonSY::BASE::TSingleton< " + className + " >"  << std::endl
				<< "{"                                                          << std::endl
				<< "public:"                                                    << std::endl
				<< "	// ������"                                              << std::endl
				<< "	" + className + "();"                                   << std::endl
				<< ""                                                           << std::endl
				<< "	// �Ҹ���"                                              << std::endl
				<< "	~" + className + "();"                                  << std::endl
				<< ""                                                           << std::endl;
			}

			// �ش� Ŭ�������� ����� �Լ����� �����մϴ�.
			{
				fos 
				<< "public:"                 << std::endl
				<< "	// �ʱ�ȭ�մϴ�."    << std::endl
				<< "	void Init();"        << std::endl
				<< ""                        << std::endl
				<< "	// �ε��մϴ�."      << std::endl
				<< "	void Load();"        << std::endl
				<< ""                        << std::endl;
			}

			// ����ȯ �Լ����� �����մϴ�.
			{
				// enum to string �Լ����� �����մϴ�.
				{
					fos	
						<< "public:"                           << std::endl;
					
					for ( const auto& typeData : typeDataCont )
					{
						const auto& enumName = typeData.second.enumName;

						fos	
						<< "	// " + enumName + " ���� String���� �����ϴ� �Լ��Դϴ�. "       << std::endl
						<< "	std::string " + enumName + "ToString( " + enumName + " ) const;" << std::endl
						<< ""                                                                    << std::endl;
					}
				}

				// string to enum �Լ����� �����մϴ�.
				{
					fos	
						<< "public:"                           << std::endl;
					
					for ( const auto& typeData : typeDataCont )
					{
						const auto& enumName = typeData.second.enumName;

						fos	
						<< "	// String�� " + enumName + " ������ �����ϴ� �Լ��Դϴ�. "                 << std::endl
						<< "	" +enumName + " StringTo" + enumName + "( const std::string& str ) const;" << std::endl
						<< ""                                                                              << std::endl;
					}
				}
			}

			// ����������� ����-�����մϴ�.
			{
				fos	
					<< "private:"              << std::endl;
				
				const auto maxLength = GetMaxLength( typeDataCont );

				for ( const auto& typeData : typeDataCont )
				{
					const auto& enumName = typeData.second.enumName;

					fos	
					<< "	std::map< std::string, " + MSF( enumName + " >", maxLength + 3 ) + " m_" + MSF( MAL( enumName ) + "Cont;",  maxLength + 6 ) 
						<< "// ��Ʈ���� " + enumName + "���� �����ϱ� ���� �ʿ��� ���Դϴ�. "                                     << std::endl;
				}
			}

			// Ŭ������ �ݴ´�.
			{
				fos	
				<< "};" << std::endl;
			}
		}

		{}

		// TypeConverter.cpp �ۼ�
		{
			      std::ofstream fos      ( destSourceFilePath );
			const std::string   className( "TypeConverter"    );
				
			// Write Comment
			WriteComment( fos );

			// ����� ���Ͽ� ����.
			{
				fos 
				<< "#include \"stdafx.h\""                                             << std::endl
				<< "#include \"Utils.h\""                                              << std::endl
				<< "#include \"" + className + ".h\""                                  << std::endl
				<< ""                                                                  << std::endl;
			}

			// �⺻ ������, �Ҹ��ڸ� �����Ѵ�.
			{
				fos 
				<< className << "::" << className << "()"  << std::endl
				<< "{"                                     << std::endl
				<< "	Init();"                           << std::endl
				<< "	Load();"                           << std::endl
				<< "}"                                     << std::endl
				<< ""                                      << std::endl;

				fos 
				<< className << "::~" << className << "()" << std::endl
				<< "{"                                     << std::endl
				<< "	Init();"                           << std::endl
				<< "}"                                     << std::endl
				<< ""                                      << std::endl;
			}

			// init �Լ��� �����Ѵ�.
			{
				fos 
					<< "void " + className + "::Init()"        << std::endl
					<< "{"                                     << std::endl;

				for ( const auto& typeData : typeDataCont )
				{
					const auto& enumName = typeData.second.enumName;

					fos
					<< "	m_" + MAL( enumName ) + "Cont.clear();" <<std::endl;
				}

				fos
					<< "};"                                    << std::endl
					<< ""                                      << std::endl;
			}

			// Load �Լ��� �����Ѵ�.
			{
				fos 
						<< "void " + className + "::Load()"        << std::endl
						<< "{"                                     << std::endl;

				for ( const auto& typeData : typeDataCont )
				{
					const auto& enumName      = typeData.second.enumName;
					const auto& memberVarName = "m_" + MAL( enumName ) + "Cont";
					const auto  maxLength     = GetMaxLength( typeData.second.dataNameCont );

					fos
						<< "	// " + memberVarName + "�� �� ��ҵ鿡 ���� ����ȯ�� �����մϴ�." <<std::endl;

					for ( const auto& dataName : typeData.second.dataNameCont )
					{
						fos
						<< "	" + memberVarName + ".emplace( \"" + MSF( MSF( dataName + "\", ", maxLength + 3 ) + enumName + "::" + dataName , maxLength * 2 + 5 + static_cast<int>(enumName.size()) ) + " );" << std::endl;
					}

					fos
						<< "" << std::endl;
				}

				fos
						<< "};"                                    << std::endl
						<< ""                                      << std::endl;
			}

			// enum To String�� ���� �����Ѵ�.
			{
				for ( const auto& typeData : typeDataCont )
				{
					const auto& enumName  = typeData.second.enumName;
					const auto  maxLength = GetMaxLength( typeData.second.dataNameCont );

					fos 
						<< "std::string TypeConverter::" + enumName + "ToString( " + enumName + " ele ) const" << std::endl
						<< "{" << std::endl
						<< "	switch ( ele )" << std::endl
						<< "	{" << std::endl;

					for ( const auto& dataName : typeData.second.dataNameCont )
					{
						fos 
						<< "		case " + MSF( enumName + "::" + dataName + ":" , maxLength + 10 + static_cast< int >( enumName.size() ) ) + "{ return \"" + MSF( dataName + "\"; " , maxLength + 3 ) + "}" << std::endl;
					}

					fos 
						<< "		default:"                                                                                                          << std::endl
						<< "			WARN_LOG(\" Invalid enumType -> " + enumName + "  ...why? \" + std::to_string( static_cast< int >( ele ) ) );" << std::endl
						<< "			return \"\";"                                                                                                  << std::endl
						<< "	}"                                                                                                                     << std::endl
						<< "};"                                                                                                                        << std::endl
						<< ""                                                                                                                          << std::endl;
				}
			}

			// string to enum�� �����Ѵ�.
			{
				for ( const auto& typeData : typeDataCont )
				{
					const auto& enumName = typeData.second.enumName;

					fos 
					<< enumName + " TypeConverter::StringTo"+ enumName + "( const std::string& str ) const"                         << std::endl
					<< "{"                                                                                                          << std::endl
					<< "	if ( auto iter = m_"+ MAL( enumName ) +"Cont.find( str ); iter != m_"+ MAL( enumName ) +"Cont.end() ) " << std::endl
					<< "	{"                                                                                                      << std::endl
					<< "		return iter->second;"                                                                               << std::endl
					<< "	}"                                                                                                      << std::endl
					<< ""                                                                                                           << std::endl
					<< "	return " + enumName + "::NONE;"                                                                         << std::endl
					<< "};"                                                                                                         << std::endl
					<< ""                                                                                                           << std::endl;
				}
			}
		}
	}
}

// ","�� �������� ��ũ�������� ����� �����Ѵ�.
std::vector< std::string > DoTokenize( const std::string& stringValue, const char delimiter /* = ',' */ )
{
	std::vector< std::string > retCont;
	std::string                token;
	std::stringstream          stringStream( stringValue );

	while ( std::getline( stringStream, token, delimiter ) )
	{
		retCont.emplace_back( token );
	}

	return retCont;
}

void InfoLog( const std::string& msg )
{
	SetConsoleTextAttribute ( GetStdHandle( STD_OUTPUT_HANDLE ), 0x08 | ( 0x00 << 4 ) );
	std::cout << "[INFO] " << msg << std::endl;
	SetConsoleTextAttribute ( GetStdHandle(STD_OUTPUT_HANDLE), 0x0F | (0 << 4) );
}

void WarnLog( const std::string& msg )
{
	SetConsoleTextAttribute ( GetStdHandle(STD_OUTPUT_HANDLE), 0x0F | ( 0x06 << 4 ) );
	std::cout << "[WARN] " << msg << std::endl;
	SetConsoleTextAttribute ( GetStdHandle(STD_OUTPUT_HANDLE), 0x0F | (0 << 4) );
}

void ErrorLog( const std::string& msg )
{
using namespace std::chrono_literals;
	SetConsoleTextAttribute ( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0F | ( 0x0C << 4 ) );

	std::cout << "[ERROR] " << msg << std::endl;

	for ( int i = 3; i > 0; --i )
	{
		std::cout << i << std::endl;
		std::this_thread::sleep_for( 1s );
	}

	std::cout << "QUIT" << std::endl;

	SetConsoleTextAttribute ( GetStdHandle( STD_OUTPUT_HANDLE ), 0x0F | (0 << 4) );

	while( 4444 ) { }
}
