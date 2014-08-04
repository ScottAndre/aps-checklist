/* Checklist
 * Task.h
 *
 * Copyright Scott Andre 2014
 */

#ifndef CHECKLIST_TASK
#define CHECKLIST_TASK

#include <string>
#include <vector>

#include "Date.h"

enum Recurrence {
	none,     // no recurrence
	intervallic, // recurs once every x days
	periodic  // recurs on certain days each week (maybe need new name)
};

class Task {
public:
	Task(std::string task, bool p);
	Task(std::string task, const std::string r_period, bool p);
	Task(std::string task, int r_interval, bool p);
	~Task() {}

	Task(const Task &t);
	Task(Task &&t);

	Task &operator=(const Task &t);
	Task &operator=(Task &&t);

	const std::string get() const { return _task; }

	void mark_complete() { _complete = true; }
	void mark_incomplete() { _complete = false; }

	bool is_complete() const { return _complete; }
	bool is_recurring() const { return _recurrence != none; }
	bool is_persistent() const { return _persistent; }

	void update(); // for recurring tasks, updates the Task's date to the next occurence

	void set_recurrence(const std::string period); // set periodic recurrence for a task. Throws an error if _recurrence != periodic
	void set_recurrence(int interval); // set interval recurrence for a task. Throws an error if _recurrence != intervallic

private:
	std::string _task;
	Date _date;
	Recurrence _recurrence;
	int _recurrence_interval;
	std::vector<std::string> _recurrence_period;
	bool _persistent;
	bool _complete;
};

#endif

