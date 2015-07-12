/* Checklist
 * Date.h
 *
 * Copyright Scott Andre 2015
 */

#ifndef CHECKLIST_DATE
#define CHECKLIST_DATE

#include <ctime>
#include <string>
#include <iostream>
#include <utility>

class Date {
public:
	Date(); // default constructor: gets current date from system clock
	Date(time_t t); // new Date based on a given amount of milliseconds

	Date(const Date &d)
	:_time(d._time) {}
	Date(Date &&d)
	:_time(std::move(d._time)) {}

	Date &operator=(const Date &d) {
		if(this != &d) _time = d._time; // 'if' is probably unnecessary but it's good practice
		return *this;
	}
	Date &operator=(Date &&d) {
		if(this != &d) _time = std::move(d._time); // here it most certainly is necessary
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
	Date operator+(const Date &d) const {
		return Date(_time + d._time);
	}
	Date operator-(const Date &d) const {
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

	void set_time();
	
	std::string to_string() const;

	time_t get_raw_time() const { return _time; }
	std::string get_day() const;

private:
	time_t _time;

	uint8_t _day;
	std::string _month;
	std::string  _year;
	std::string _weekday;
};

std::ostream &operator<<(std::ostream &out, const Date &d);

#endif
