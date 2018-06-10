// SiriClienteCpp.cpp

#include "stdafx.h"
#include <gtest/gtest.h>

//#include "CACT/ca_ct.h"

#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/filesystem.hpp>

#include <fstream>
#include <random>
#include <chrono>

namespace fs = boost::filesystem;

std::string		server_address			("http://localhost:8080/");
std::wstring	consumer_address		(L"http://localhost:8081/");

bool siri_debug = true;		// true:	print debug files
bool siri_proxy = true;		// true:	proxy on 8888 must be ON (i.e. Fiddler opened)
bool random_lines = true;	// true:	get_random_line() returns a random line from available ones, rather than desired_line
bool random_stops = true;	// true:	get_random_line() returns a random stop from available ones, rather than desired_stop

#ifdef UNICODE
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
#else
	std::locale loc(std::locale::classic());
#endif

std::wstring user					(L"user");
std::wstring unauth_user			(L"unauthorised_user");
std::wstring pwd					(L"password");

boost::gregorian::date start_date(2016, 12, 15);

long start_hour =			8;		// preferrably < simulation starting hour to test all cases
int64_t desired_hours =		9;
int64_t too_many_hours =	25;		// max. 24
unsigned desired_days =		10;
unsigned too_many_days =	33;		// max. 31

std::wstring desired_language		(L"en");		// by default
std::wstring velocity_units			(L"km/h");

std::wstring desired_stop			(L"2031196");		// 10.	ALISON ROAD [2]		- 2031196
std::wstring non_existing_stop		(L"99999");

std::wstring desired_line			(L"1");
std::wstring non_existing_line		(L"99999");

std::wstring desired_vehicle		(L"1");
std::wstring non_existing_vehicle	(L"99999");

std::wstring desired_direction		(L"CIRCULAR QUAY [1]");		// Ida 1

std::wstring ALL_LINES_CODE			(L"0");
std::wstring ALL_STOPS_CODE			(L"0");
std::wstring WRONG_ALL_LINES_CODE	(L"1");
std::wstring WRONG_ALL_STOPS_CODE	(L"1");

std::wstring unauth_error_text							(L"Unauthorised user");
std::wstring missing_parameter_error_text				(L"Missing parameter: ");
std::wstring too_many_days_error_text					(L"31");
std::wstring too_many_hours_error_text					(L"24");
std::wstring unknown_stop_error_text					(L"Unknown data: ");
std::wstring unknown_vehicle_error_text					(L"Unknown data: ");
std::wstring sub_refused_error_text						(L"Ill-formed request: sub refused");
std::wstring all_subs_refused_error_text				(L"Ill-formed service request: all subs refused");
std::wstring unknown_subscription_error_text			(L"Unknown subscription");
std::wstring unknown_subscriber_error_text				(L"Unknown subscriber");
std::wstring unknown_ALL_LINES_CODE_error_text			(L"Incorrect ALL_LINES_CODE");
std::wstring unknown_ALL_STOPS_CODE_error_text			(L"Incorrect ALL_STOPS_CODE");


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	testing::FLAGS_gtest_repeat = 10;
	int nRetCode = RUN_ALL_TESTS();

	//if(nRetCode)
		system("pause");

	return nRetCode;
}
