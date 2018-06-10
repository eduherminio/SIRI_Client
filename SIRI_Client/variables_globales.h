#pragma once

#include "soapSiriProducerDocBindingProxy.h"
//#include "GMV configuration file reader"
#include "CACT/ca_ct.h"
#include <gtest/gtest.h>
#include <locale>
#include <fstream>
#include <random>
#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/chrono.hpp>
#include <boost/date_time.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>


namespace fs = boost::filesystem;

#define auth_user	user
#define auth_pwd	pwd

extern bool siri_debug;
extern bool siri_proxy;
extern bool random_lines;
extern bool random_stops;

extern std::string	server_address;
extern std::wstring consumer_address;

extern std::locale	loc;

extern std::wstring ALL_LINES_CODE;
extern std::wstring ALL_STOPS_CODE;
extern std::wstring WRONG_ALL_LINES_CODE;
extern std::wstring WRONG_ALL_STOPS_CODE;

extern std::wstring user;
extern std::wstring unauth_user;
extern std::wstring pwd;
extern std::wstring desired_stop;
extern std::wstring non_existing_stop;
extern std::wstring desired_line;
extern std::wstring non_existing_line;
extern std::wstring desired_vehicle;
extern std::wstring non_existing_vehicle;
extern std::wstring desired_direction;
extern std::wstring velocity_units;
extern std::wstring desired_language;

extern boost::gregorian::date start_date;
extern long start_hour;
extern int64_t desired_hours;
extern int64_t too_many_hours;
extern unsigned desired_days;
extern unsigned too_many_days;

extern std::wstring unauth_error_text;
extern std::wstring missing_parameter_error_text;
extern std::wstring too_many_days_error_text;
extern std::wstring too_many_hours_error_text;
extern std::wstring unknown_stop_error_text;
extern std::wstring unknown_vehicle_error_text;
extern std::wstring unknown_ALL_LINES_CODE_error_text;
extern std::wstring unknown_ALL_STOPS_CODE_error_text;

extern std::wstring sub_refused_error_text;
extern std::wstring all_subs_refused_error_text;
extern std::wstring unknown_subscriber_error_text;
extern std::wstring unknown_subscription_error_text;
