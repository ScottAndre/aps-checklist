/* Checklist
 * Task.h
 *
 * Created by Scott Andre
 */

#ifndef CHECKLIST_TASK
#define CHECKLIST_TASK

#include <string>
#include <vector>

#include "Date.h"

enum Recurrence {
	none,        // no recurrence
	intervallic, // recurs once every x days
	periodic     // recurs on certain days each week (maybe need new name)
};

enum Weekday {
	Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday
};

class Task {
friend class PGAdaptor;

public:
	Task(const std::string &task, bool p = false, Date d = Date());
	Task(const std::string &task, int r_interval, bool p = false, Date d = Date());
	Task(const std::string &task, const char *r_period, bool p = false, Date d = Date());
	Task(const std::string &task, const std::string &r_period, bool p = false, Date d = Date());

	Task(const Task &t);
	Task(Task &&t);

	~Task() {}

	Task &operator=(const Task &t);
	Task &operator=(Task &&t);

	bool operator==(const Task &t);
	bool operator!=(const Task &t);

	const std::string desc() const { return _task; }
	const int id() const { return _id; }

	void mark_complete() { _complete = true; }
	void mark_incomplete() { _complete = false; }

	bool complete() const { return _complete; }
	bool recurring() const { return _recurrence != none; }
	bool persistent() const { return _persistent; }

	Date date() const { return _date; }
	Date next_occurrence_on_or_after(const Date d) const;

	void set_date(const Date d);
	void set_recurrence(const std::string &period); // set periodic recurrence for a task. Throws an error if _recurrence != periodic
	void set_recurrence(int interval); // set interval recurrence for a task. Throws an error if _recurrence != intervallic

	static std::string serialize_recurrence_period(const std::vector<Weekday> &);

	bool exists_in_database() const;

private:
	int _id;
	std::string _task;
	Date _date;
	Recurrence _recurrence;
	int _recurrence_interval;
	std::vector<Weekday> _recurrence_period;
	bool _persistent;
	bool _complete;
};

#endif
