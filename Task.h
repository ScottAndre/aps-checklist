/* Checklist
 * Task.h
 *
 * Copyright Scott Andre 2014
 */

#ifndef CHECKLIST_TASK
#define CHECKLIST_TASK

#include <string>
#include <vector>

enum Recurrence {
	none,     // no recurrence
	interval, // recurs once every x days
	periodic  // recurs on certain days each week (maybe need new name)
};

class Task {
public:
	Task(std::string task);
	~Task();

	Task(const Task &t);
	Task &operator=(const Task &t);

	const std::string get() const { return _task; }

	void markComplete() { _complete = true; }
	void markIncomplete() { _complete = false; }

	bool isComplete() const { return _complete; }
	bool isRecurring() const { return _recurrence != none; }
	bool isPersistent() const { return _persistent; }

	void update(); // for recurring tasks, updates the Task's date to the next occurence

private:
	std::string _task;
	Date _date;
	Recurrence _recurrence;
	int _recurrenceInterval;
	std::vector<std::string> _recurrencePeriod;
	bool _persistent;
	bool _complete;
};

#endif

