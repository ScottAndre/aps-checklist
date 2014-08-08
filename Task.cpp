/* Checklist
 * Task.cpp
 *
 * Copyright Scott Andre 2014
 */

#include <utility>

#include "Task.h"

Task::Task(std::string task, bool p)
:_task(std::move(task)), _recurrence(none), _persistent(p), _complete(false), _recurrence_interval(0) {
	Date now;
	_date = std::move(now);

	_recurrence_period = {};
}

Task::Task(std::string task, std::string r_period, bool p)
:_task(std::move(task)), _recurrence(periodic), _persistent(p), _complete(false), _recurrence_interval(0) {
	Date now;
	_date = std::move(now);

	set_recurrence(r_period);
}

Task::Task(std::string task, int r_interval, bool p)
:_task(std::move(task)), _recurrence(intervallic), _persistent(p), _complete(false) {
	Date now;
	_date = std::move(now);

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
void Task::set_recurrence(const std::string period) {
	if(_recurrence != periodic) {
		std::cerr << "Warning: setting recurrence period on a non-periodic task\n";
	} // bad things happen

	for(char day : period) {
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
char delimiter = ';';

std::string Task::serialize() {

}

Task Task::deserialize(std::string serialized_task) {
	
}

// gets the next value of a serialized task. If no more values, returns ""
std::string get_next_token(std::string &serialized_task) {
	
	for(auto it = serialized_task.begin(); it != serialized_task.end(); it++) {
		if(*it != delimiter) continue;

		std::string token(serialized_task.begin(), it);
		std::string remainder(it + 1, serialized_task.end());
		serialized_task = std::move(remainder);
		return token;
	}
}

#ifdef UNIT_TEST

std::string Unittest::get_next_token(std::string &serialized_task) {
	return get_next_token(serialized_task);
}

#endif
