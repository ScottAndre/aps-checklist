/* Checklist
 * Date.h
 *
 * Copyright Scott Andre 2014
 */

#ifndef CHECKLIST_DATE
#define CHECKLIST_DATE

#include <ctime>
#include <string>
#include <iostream>

class Date {
public:
	Date() // default constructor: gets current date from system clock
	:_time(std::time(NULL)) {} 
	Date(time_t t) // new Date based on a given amount of milliseconds
	:_time(t) {
		if(_time < 0) _time = 0;
	} 
	Date(const Date &d)
	:_time(d._time) {}

	Date &operator=(const Date &d) {
		if(this != &d) _time = d._time; // 'if' is probably unnecessary but it's good practice
		return *this;
	}
	Date &operator+=(const Date &d) {
		_time += d._time;
		return *this;
	}
	Date &operator-=(const Date &d) {
		_time -= d._time;
		if(_time < 0) _time = 0;
		return *this;
	}
	const Date operator+(const Date &d) const {
		return Date(_time + d._time);
	}
	const Date operator-(const Date &d) const {
		return Date(_time - d._time);
	}
	bool operator==(const Date &d) const {
		return _time == d._time;
	}
	bool operator!=(const Date &d) const {
		return _time != d._time;
	}
	bool operator<(const Date &d) const {
		return _time < d._time;
	}
	bool operator>(const Date &d) const {
		return _time > d._time;
	}
	bool operator<=(const Date &d) const {
		return _time <= d._time;
	}
	bool operator>=(const Date &d) const {
		return _time >= d._time;
	}
	
	std::string to_string() const { return std::ctime(&_time); }

	time_t get_raw_time() const { return _time; }
	std::string get_day() const { return this->to_string().substr(0, 3); }
	std::string without_time() const { 
		return this->to_string().substr(0, 11).append( this->to_string().substr(20) );
	}

private:
	time_t _time;

};

/*
Date operator+(const Date &d1, const Date &d2) {
	return Date(d1.getRawTime() + d2.getRawTime());
}
Date operator-(const Date &d1, const Date &d2) {
	return Date(d1.getRawTime() - d2.getRawTime());
}
*/

std::ostream &operator<<(std::ostream &out, const Date &d) {
	out << d.to_string();
	return out;
}


#endif
