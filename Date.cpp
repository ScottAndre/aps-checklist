/* Checklist
 * Date.cpp
 *
 * Created by Scott Andre
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "Date.h"

/* TODO: is this function even necessary? this looks like the ravings of a madman */
time_t Date::localtime() const {
	uint32_t one_day_in_s = 86400;
	uint32_t one_hour_in_s = 3600;

	auto local_time = _time;
	local_time -= _time % one_day_in_s;
	// get time difference
	auto local = std::localtime(&_time);
	auto utc = std::gmtime(&_time);
	auto diff = (utc->tm_hour + 24) - local->tm_hour;
	// account for time difference
	local_time += diff * one_hour_in_s;
	return local_time;
}

Date::Date()
:_time(std::time(NULL)) {}

Date::Date(time_t t)
:_time(t) {
	if(_time < 0) _time = 0;
}

std::string Date::to_string() const {
	char container[32];
	//time_t local_time = this->localtime();
	auto str_size = std::strftime(container, sizeof(container), "%A, %d %B %Y", std::localtime(&_time));
	if(str_size == 0) {
		std::cout << "strftime failed in Date::to_string().";
		std::exit(EXIT_FAILURE);
	}
	std::string str_representation(container);
	return str_representation;
}

std::string Date::get_day() const {
	std::string str_representation = this->to_string();
	return str_representation.substr(0, str_representation.find_first_of(','));
}

std::string Date::to_db_representation(time_t t) {
	// Uses YYYY-MM-DD format
	char rep[11];
	auto time = std::localtime(&t);
	std::strftime(rep, 11, "%Y-%m-%d", time);
	std::string srep(rep);
	return srep;
}

Date Date::from_db_representation(std::string s) {
	// Assuming YYYY-MM-DD format
	std::tm time;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	std::sscanf(s.c_str(), "%u-%u-%u", &year, &month, &day);
	time.tm_sec = 0;
	time.tm_min = 0;
	time.tm_hour = 12; // gives us some wiggle room
	time.tm_mday = day;
	time.tm_mon = month;
	time.tm_year = year;
	time.tm_isdst = -1; // forces mktime to try to autodetect Daylight savings Time, not that it really matters for our purposes
	Date d(std::mktime(&time));
	return d;
}

std::ostream &operator<<(std::ostream &out, const Date &d) {
	out << d.to_string();
	return out;
}
