/* Checklist
 * Task.cpp
 *
 * Created by Scott Andre
 */

#include <utility>
#include <sstream>

#include "Task.h"

Task::Task(const std::string &task, bool p, Date d)
:_id(-1), _task(task), _date(d), _recurrence(none), _recurrence_interval(0), _persistent(p), _complete(false) {
	_recurrence_period = {};
}

Task::Task(const std::string &task, const char *r_period, bool p, Date d)
:_id(-1), _task(task), _date(d), _recurrence(periodic), _recurrence_interval(0), _persistent(p), _complete(false) {
	set_recurrence(std::string(r_period));
}

Task::Task(const std::string &task, const std::string &r_period, bool p, Date d)
:_id(-1), _task(task), _date(d), _recurrence(periodic), _recurrence_interval(0), _persistent(p), _complete(false) {
	set_recurrence(r_period);
}

Task::Task(const std::string &task, int r_interval, bool p, Date d)
:_id(-1), _task(task), _date(d), _recurrence(intervallic), _persistent(p), _complete(false) {
	set_recurrence(r_interval);
	_recurrence_period = {};
}

Task::Task(const Task &t)
:_id(t._id), _task(t._task), _date(t._date), _recurrence(t._recurrence), _recurrence_interval(t._recurrence_interval),
	_recurrence_period(t._recurrence_period), _persistent(t._persistent), _complete(t._complete) {}

Task::Task(Task &&t)
:_id(std::move(t._id)), _task(std::move(t._task)), _date(std::move(t._date)), _recurrence(t._recurrence), _recurrence_interval(t._recurrence_interval),
	_recurrence_period(std::move(t._recurrence_period)), _persistent(t._persistent), _complete(t._complete) {}

Task &Task::operator=(const Task &t) {
	if(this == &t) return *this;
	_id = t._id;
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
	_id = std::move(t._id);
	_task = std::move(t._task);
	_date = std::move(t._date);
	_recurrence = t._recurrence;
	_recurrence_interval = t._recurrence_interval;
	_recurrence_period = std::move(t._recurrence_period);
	_persistent = t._persistent;
	_complete = t._complete;
	return *this;
}

bool Task::operator==(const Task &t) {
	return _task == t._task
	    && _date == t._date
	    && _recurrence == t._recurrence
	    && _recurrence_interval == t._recurrence_interval
	    && _recurrence_period == t._recurrence_period
	    && _persistent == t._persistent
	    && _complete == t._complete;
}

bool Task::operator!=(const Task &t) {
	return !(*this == t);
}

void Task::update() {
	if(_recurrence == periodic) {

	}
	else if(_recurrence == intervallic) {

	}
}

// Accepts a string in the form "MTWRFSU" where each character indicates a day on which the task should recur
void Task::set_recurrence(const std::string &period) {
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

bool Task::exists_in_database() const {
	return _id != -1;
}

std::string Task::serialize_recurrence_period(const std::vector<Weekday> &period) {
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
