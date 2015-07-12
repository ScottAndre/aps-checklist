/* Checklist
 * Task.cpp
 *
 * Copyright Scott Andre 2015
 */

#include <utility>
#include <sstream>

#include "Task.h"

Task::Task(std::string task, bool p, Date d)
:_task(task), _date(d), _recurrence(none), _persistent(p), _complete(false), _recurrence_interval(0) {
	_recurrence_period = {};
}

Task::Task(std::string task, const char *r_period, bool p, Date d)
:_task(task), _date(d), _recurrence(periodic), _persistent(p), _complete(false), _recurrence_interval(0) {
	set_recurrence(r_period);
}

Task::Task(std::string task, int r_interval, bool p, Date d)
:_task(task), _date(d), _recurrence(intervallic), _persistent(p), _complete(false) {
	set_recurrence(r_interval);
	_recurrence_period = {};
}

Task::Task(const Task &t)
:_task(t._task), _date(t._date), _recurrence(t._recurrence), _recurrence_interval(t._recurrence_interval),
	_recurrence_period(t._recurrence_period), _persistent(t._persistent), _complete(t._complete) {}

Task::Task(Task &&t)
:_task(std::move(t._task)), _date(std::move(t._date)), _recurrence(t._recurrence), _recurrence_interval(t._recurrence_interval),
	_recurrence_period(std::move(t._recurrence_period)), _persistent(t._persistent), _complete(t._complete) {}

Task &Task::operator=(const Task &t) {
	if(this == &t) return *this;
	_task = t._task;
	_date = t._date;
	_recurrence = t._recurrence;
	_recurrence_interval = t._recurrence_interval;
	_recurrence_period = t._recurrence_period;
	_persistent = t._persistent;
	_complete = t._complete;
	return *this;
}

Task &Task::operator=(Task &&t) {
	if(this == &t) return *this;
	_task = std::move(t._task);
	_date = std::move(t._date);
	_recurrence = t._recurrence;
	_recurrence_interval = t._recurrence_interval;
	_recurrence_period = std::move(t._recurrence_period);
	_persistent = t._persistent;
	_complete = t._complete;
	return *this;
}

void Task::update() {
	if(_recurrence == periodic) {

	}
	else if(_recurrence == intervallic) {

	}
}

// Accepts a string in the form "MTWRFSU" where each character indicates a day on which the task should recur
void Task::set_recurrence(const char *period) {
	if(_recurrence != periodic) {
		std::cerr << "Warning: setting recurrence period on a non-periodic task\n";
	} // bad things happen

	for(char day : std::string(period)) {
		switch(day) {
			case 'M':
			case 'm':
				_recurrence_period.push_back(Monday); break;
			case 'T':
			case 't':
				_recurrence_period.push_back(Tuesday); break;
			case 'W':
			case 'w':
				_recurrence_period.push_back(Wednesday); break;
			case 'R':
			case 'r':
				_recurrence_period.push_back(Thursday); break;
			case 'F':
			case 'f':
				_recurrence_period.push_back(Friday); break;
			case 'S':
			case 's':
				_recurrence_period.push_back(Saturday); break;
			case 'U':
			case 'u':
				_recurrence_period.push_back(Sunday); break;
		}
	}
}

void Task::set_recurrence(int interval) {
	if(_recurrence != intervallic) {
		std::cerr << "Warning: setting recurrence interval on a non-intervallic task\n";
	} // bad things happen
	
	_recurrence_interval = interval;
}

std::string get_next_token(std::string &);
std::string escape_delimiter(const std::string &);
std::string serialize_recurrence_period(const std::vector<Weekday> &);
char delimiter = ';';

std::string Task::serialize() {
	std::stringstream serialized_task;
	
	serialized_task << escape_delimiter(_task) << delimiter
		<< _date.get_raw_time() << delimiter
		<< _recurrence << delimiter;

	switch(_recurrence) {
		case intervallic:
			serialized_task << _recurrence_interval << delimiter; break;
		case periodic:
			serialized_task << serialize_recurrence_period(_recurrence_period) << delimiter; break;
		default:
			break;
			// do nothing
	}

	serialized_task << _persistent << delimiter
		<< _complete << delimiter;

	return serialized_task.str();
}

Task Task::deserialize(std::string serialized_task) {
	auto task = get_next_token(serialized_task);
	// undo the escape of delimiter character
	auto pos = 0;
	do { 
		pos = task.find("\\" + delimiter, pos);
		if(pos != std::string::npos) {
			task.replace(pos, 2, std::to_string(delimiter));
		}
	} while(pos != std::string::npos);

	// cannot implement deserialize until Tasks implement constructors for varying Dates (_date is not automatically set to day of creation)

}

// gets the next value of a serialized task. If no more values, returns ""
std::string get_next_token(std::string &serialized_task) {
	
	for(auto it = serialized_task.begin(); it != serialized_task.end(); it++) {
		if(*it != delimiter) continue;
		if(it != serialized_task.begin() && *(it - 1) == '\\') continue;

		std::string token(serialized_task.begin(), it);
		std::string remainder(it + 1, serialized_task.end());
		serialized_task = std::move(remainder);
		return token;
	}

	return "";
}

std::string escape_delimiter(const std::string &str) {
	std::stringstream escaped_str;

	for(auto it = str.begin(); it != str.end(); it++) {
		if(*it == delimiter) {
			escaped_str << "\\";
		}
		escaped_str << *it;
	}

	return escaped_str.str();
}

std::string serialize_recurrence_period(const std::vector<Weekday> &period) {
	std::stringstream serialized_period;
	
	for(auto it = period.begin(); it != period.end(); it++) {
		switch(*it) {
			case Monday:
				serialized_period << 'M'; break;
			case Tuesday:
				serialized_period << 'T'; break;
			case Wednesday:
				serialized_period << 'W'; break;
			case Thursday:
				serialized_period << 'R'; break;
			case Friday:
				serialized_period << 'F'; break;
			case Saturday:
				serialized_period << 'S'; break;
			case Sunday:
				serialized_period << 'U'; break;		
			default:
				break;
				//do nothing
		}
	}

	return serialized_period.str();
}

#ifdef UNIT_TEST

std::string Unittest::_task_internal_get_next_token(std::string &serialized_task) {
	return get_next_token(serialized_task);
}

std::string Unittest::_task_internal_escape_delimiter(const std::string &str) {
	return escape_delimiter(str);
}

#endif
