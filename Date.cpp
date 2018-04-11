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

void init_tm(std::tm *time, unsigned int year, unsigned int month, unsigned int day) {
	time->tm_sec = 0;
	time->tm_min = 0;
	time->tm_hour = 1; // gives us some wiggle room
	time->tm_mday = day;
	time->tm_mon = month - 1; // months since January: 0 - 11
	time->tm_year = year - 1900; // years since 1900
	time->tm_isdst = -1; // forces mktime to try to autodetect Daylight savings Time, not that it really matters for our purposes
}

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

Date::Date(std::string s) {
	// accepts date string formatted as MM/DD/YY
	std::tm time;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	std::sscanf(s.c_str(), "%u/%u/%u", &month, &day, &year);
	if(year < 100)
		year += 2000; // if year is specified as "17" instead of "2017", for instance. This will need to be updated once every 100 years. What terrible programming practice, hard-coding like that!
	init_tm(&time, year, month, day);
	_time = std::mktime(&time);
}

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

int Date::get_day_of_week() const {
	time_t t = this->get_raw_time();
	auto time = std::localtime(&t);
	int day = time->tm_wday;
	// convert to our Weekday representation - Monday is 0, Sunday is 6
	if(day == 0) {
		day = 6;
	}
	else {
		day -= 1;
	}
	return day;
}

std::string Date::get_day_string() const {
	std::string str_representation = this->to_string();
	return str_representation.substr(0, str_representation.find_first_of(','));
}

std::string Date::to_db_representation() const {
	// Uses YYYY-MM-DD format
	time_t t = this->get_raw_time();
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
	init_tm(&time, year, month, day);
	Date d(std::mktime(&time));
	return d;
}

long Date::days_between(Date one, Date two) {
	double diff = std::difftime(one.get_raw_time(), two.get_raw_time()); // difference in seconds
	if(diff < 0) {
		diff *= -1;
	}
	return (long)diff / Date::days; // divide by 24 hrs to get the difference in days
}

std::ostream &operator<<(std::ostream &out, const Date &d) {
	out << d.to_string();
	return out;
}
