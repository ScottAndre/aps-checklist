/* Checklist
 * Date.cpp
 *
 * Copyright Scott Andre 2015
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "Date.h"

void Date::set_time() {
	uint32_t one_day_in_s = 86400;
	uint32_t one_hour_in_s = 3600;

	_time -= _time % one_day_in_s;
	// get time difference
	auto local = localtime(&_time);
	auto utc = gmtime(&_time);
	auto diff = (utc->tm_hour + 24) - local->tm_hour;
	// account for time difference
	_time += diff * one_hour_in_s;
}

Date::Date()
:_time(std::time(NULL)) {
	this->set_time();
} 

Date::Date(time_t t)
:_time(t) {
	if(_time < 0) _time = 0;
	this->set_time();
}

std::string Date::to_string() const {
	char container[32];
	auto str_size = std::strftime(container, sizeof(container), "%A, %d %B %Y", localtime(&_time));
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

std::ostream &operator<<(std::ostream &out, const Date &d) {
	out << d.to_string();
	return out;
}
