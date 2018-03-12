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

Date Task::next_occurrence_on_or_after(const Date d) const {
	if(d <= _date) {
		return _date;
	}

	if(_recurrence == periodic) {
		int w = d.get_day_of_week();
		//Weekday next_occurrence;
		int min_distance = 8; // the next occurrence will never be more than a week away
		// loop through recurrence_period, find the day after w with the lowest distance from w
		for(Weekday occurrence : _recurrence_period) {
			int distance = (int)occurrence - w;
			if(distance < 0) {
				distance += 7; // if this occurrence is a day earlier in the week than w, add 7 days to it to get next week's occurrence
			}
			if(distance < min_distance) {
				//next_occurrence = occurrence;
				min_distance = distance;
			}
		}
		Date difference(min_distance * Date::days);
		return d + difference;
	}
	else if(_recurrence == intervallic) {
		int w = Date::days_between(d, _date);
		// round up to the next multiple of _reccurrence_interval
		int x = (w / _recurrence_interval) * _recurrence_interval;
		if(w % _recurrence_interval != 0) {
			x += _recurrence_interval;
		}
		Date difference(x * Date::days);
		return _date + difference;
	}
	else {
		std::cerr << "WARNING: tried to get next occurrence of a non-recurring task." << std::endl;
		Date not_real(0);
		return not_real;
	}
}

void Task::set_date(const Date d) {
	_date = d;
}

// Accepts a string in the form "MTWRFSU" where each character indicates a day on which the task should recur
void Task::set_recurrence(const std::string &period) {
	if(_recurrence != periodic) {
		std::cerr << "Warning: setting recurrence period on a non-periodic task" << std::endl;
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
