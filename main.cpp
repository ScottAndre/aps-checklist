/* Checklist
 * main.cpp
 *
 * Created by Scott Andre
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <vector>
#include <utility>

#include "Date.h"
#include "Task.h"
#include "PGAdaptor.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stoi;
using std::vector;

typedef struct {
	bool recurring;
	string recurrence;
	bool persistent;
	Date start_date;
	bool error;
} Options;

void print_help(const char *program_name);

void display_active_tasks();
void draw(const vector<Task> &);

void add_task(int argc, char **args);
void delete_task();

Options default_options();
Options get_options(int argc, char **args);

bool streq(const char *one, const char *two) {
	return strcmp(one, two) == 0;
}

int main(int argc, char ** argv) {
	/* no arguments - display tasks */
	if(argc < 2) {
		display_active_tasks();
		return 0;
	}

	/* help */
	if(streq(argv[1], "h") || streq(argv[1], "help") || streq(argv[1], "-h") || streq(argv[1], "--help")) {
		print_help(argv[0]);
		return 0;
	}

	/* add task */
	if(streq(argv[1], "a") || streq(argv[1], "add") || streq(argv[1], "-a") || streq(argv[1], "--add")) {
		add_task(argc - 2, &argv[2]);
		return 0;
	}

	/* delete task */

	return 0;
}

void print_help(const char *program_name) {
	cout << "Usage:" << endl;
	cout << '\t' << program_name << endl;
	cout << "\t\tDisplays all daily tasks." << endl;
	cout << '\t' << program_name << " add <task> [on <date>] [recurs <recurrence>] [persistent]" << endl;
	cout << "\t\tAdds a new task." << endl;
	cout << "\t\tIf a date is specified, the task will not appear in the list until that date. Recurring tasks will only begin recurring after that date." << endl;
	cout << "\t\t\tDates should be specified in the MM/DD/YY format." << endl;
	cout << "\t\tA recurring task will appear either on certain days of the week, or every n days. Recurring tasks recur forever, or until they are deleted." << endl;
	cout << "\t\t\tRecurrences should be specified either as an integer, or as a list of weekdays. Please specify the list of weekdays in the format \"MTWRFSU\"." << endl;
	cout << "\t\tA persistent task will not leave the list until it has been completed." << endl;
	cout << "Accepted contractions: a: add, d: on, r: recurs, p: persistent" << endl;
	cout << '\t' << program_name << " delete <task_id>" << endl;
	cout << "\t\tDeletes the task with the given ID. IDs are displayed in parentheses after the task description." << endl;
}

void display_active_tasks() {
	PGAdaptor pg;

	auto tasks = pg.retrieve_active_tasks();
	draw(tasks);
}

void draw(const vector<Task> &task_list) {
	vector<Task> incomplete;
	vector<Task> complete;

	for(Task t : task_list) {
		if(t.complete()) {
			complete.push_back(t);
		}
		else {
			incomplete.push_back(t);
		}
	}

	Date now;
	cout << now << ':' << endl;

	for(Task t : incomplete) {
		cout << '\t' << u8"\u2610" << "  " << t.desc() << " (" << t.id() << ')' << endl;
		cout << "\t\tDEBUG: start date: " << t.date() << " --- persistent: " << t.persistent() << " --- recurring: " << t.recurring() << endl;
	}

	if(incomplete.size() > 0 && complete.size() > 0) {
		cout << endl;
	}

	for(Task t : complete) {
		cout << '\t' << u8"\u2611" << "  " << t.desc() << " (" << t.id() << ')' << endl;
	}
}

void add_task(int argc, char **args) {
	PGAdaptor pg;

	string task(args[0]);
	Options opts = default_options();
	if(argc > 1) {
		opts = get_options(argc - 1, &args[1]);
	}
	if(opts.error) {
		cerr << "An error occurred while parsing options. Aborting." << endl;
		exit(EXIT_FAILURE);
	}
	Task *p;

	if(opts.recurring) {
		if(isdigit(opts.recurrence.front())) {
			p = new Task(task, stoi(opts.recurrence), opts.persistent, opts.start_date);
		}
		else {
			p = new Task(task, opts.recurrence, opts.persistent, opts.start_date);
		}
	}
	else {
		p = new Task(task, opts.persistent, opts.start_date);
	}

	Task t = *p;
	delete p;

	pg.save_task(t);
}

Options default_options() {
	Date today;
	Options opts;
	opts.error = false;
	opts.recurring = false;
	opts.persistent = false;
	opts.start_date = today;
	return opts;
}

Options get_options(int argc, char **args) {
	Options opts = default_options();
	int i = 0;
	while(i < argc) {
		if(streq(args[i], "p") || streq(args[i], "persistent") || streq(args[i], "-p") || streq(args[i], "--persistent")) {
			opts.persistent = true;
			i += 1;
		}
		else if(streq(args[i], "r") || streq(args[i], "recurs") || streq(args[i], "recurring") || streq(args[i], "-r") || streq(args[i], "--recurs") || streq(args[i], "--recurring")) {
			if(i == argc - 1 || strspn(args[i+1], "MTWRFSUmtwrfsu0123456789") != strlen(args[i+1])) {
				cerr << "Invalid recurrence format given. Use the \"help\" command to see valid recurrence formats." << endl;
				opts.error = true;
			}
			else {
				opts.recurring = true;
				string recurrence(args[i+1]);
				opts.recurrence = recurrence;
			}
			i += 2;
		}
		else if(streq(args[i], "d") || streq(args[i], "date") || streq(args[i], "on") || streq(args[i], "-d") || streq(args[i], "--date") || streq(args[i], "--on")) {
			if(i == argc - 1 || strspn(args[i+1], "0123456789/") != strlen(args[i+1])) {
				cerr << "Invalid date format given. Use the \"help\" command to see valid date formats." << endl;
				opts.error = true;
			}
			else {
				Date date(args[i+1]);
				opts.start_date = date;
			}
			i += 2;
		}
		else {
			cerr << "Invalid option " << args[i] << ". Use the \"help\" command to get a list of valid options." << endl;
			opts.error = true;
			i += 1;
		}
	}
	return opts;
}
