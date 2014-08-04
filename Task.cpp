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

void Task::set_recurrence(const std::string period) {
	
}

void Task::set_recurrence(int interval) {
	if(_recurrence != intervallic) {} // bad things happen
	
	_recurrence_interval = interval;
}
