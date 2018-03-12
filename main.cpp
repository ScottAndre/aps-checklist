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
void delete_tasks(int argc, char **args);
void complete_tasks(int argc, char **args);
void incomplete_tasks(int argc, char **args);

void cleanup();

Options default_options();
Options get_options(int argc, char **args);

bool streq(const char *one, const char *two) {
	return strcmp(one, two) == 0;
}

int main(int argc, char ** argv) {
	/* run cleanup first, updating task recurrences */
	cleanup();

	/* no arguments - display tasks */
	if(argc < 2) {
		display_active_tasks();
		return EXIT_SUCCESS;
	}

	/* help */
	if(streq(argv[1], "h") || streq(argv[1], "help") || streq(argv[1], "-h") || streq(argv[1], "--help")) {
		print_help(argv[0]);
		return EXIT_SUCCESS;
	}

	/* add task */
	if(streq(argv[1], "a") || streq(argv[1], "add") || streq(argv[1], "-a") || streq(argv[1], "--add")) {
		if(argc == 2) {
			cerr << "No task to add. Aborting." << endl;
			return EXIT_FAILURE;
		}
		add_task(argc - 2, &argv[2]);
		return EXIT_SUCCESS;
	}

	/* delete task */
	if(streq(argv[1], "d") || streq(argv[1], "delete") || streq(argv[1], "-d") || streq(argv[1], "--delete")) {
		if(argc == 2) {
			cerr << "No tasks to delete. Aborting." << endl;
			return EXIT_FAILURE;
		}
		delete_tasks(argc - 2, &argv[2]);
		return EXIT_SUCCESS;
	}

	/* complete task */
	if(streq(argv[1], "c") || streq(argv[1], "complete") || streq(argv[1], "-c") || streq(argv[1], "--complete")) {
		if(argc == 2) {
			cerr << "No tasks to mark complete. Aborting." << endl;
			return EXIT_FAILURE;
		}
		complete_tasks(argc - 2, &argv[2]);
		return EXIT_SUCCESS;
	}

	/* incomplete task */
	if(streq(argv[1], "i") || streq(argv[1], "incomplete") || streq(argv[1], "-i") || streq(argv[1], "--incomplete")) {
		if(argc == 2) {
			cerr << "No tasks to mark incomplete. Aborting." << endl;
			return EXIT_FAILURE;
		}
		incomplete_tasks(argc - 2, &argv[2]);
		return EXIT_SUCCESS;
	}

	cerr << "Unrecognized command " << argv[1] << ". Use the \"help\" command to see valid commands." << endl;
	return EXIT_FAILURE;
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
	cout << "\t\tAccepted contractions: a: add, d: on, r: recurs, p: persistent" << endl;
	cout << '\t' << program_name << " delete <task_id> [<task_id> ...]" << endl;
	cout << "\t\tDeletes the task(s) with the given ID(s). IDs are displayed in parentheses after the task description." << endl;
	cout << "\t\tAccepted contractions: d: delete" << endl;
	cout << '\t' << program_name << " complete <task_id> [<task_id> ...]" << endl;
	cout << "\t\tMarks the task(s) complete." << endl;
	cout << '\t' << program_name << " incomplete <task_id> [<task_id> ...]" << endl;
	cout << "\t\tMarks the task(s) incomplete." << endl;
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

	if(task_list.size() == 0) {
		cout << "\tNothing to do!" << endl;
	}

	for(Task t : incomplete) {
		cout << '\t' << u8"\u2610" << "  " << t.desc() << " (ID: " << t.id() << ')' << endl;
		//cout << "\t\tDEBUG: start date: " << t.date() << " --- persistent: " << t.persistent() << " --- recurring: " << t.recurring() << endl;
	}

	if(incomplete.size() > 0 && complete.size() > 0) {
		cout << endl;
	}

	for(Task t : complete) {
		cout << '\t' << u8"\u2611" << "  " << t.desc() << " (ID: " << t.id() << ')' << endl;
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

	bool success = pg.save_task(t);
	if(!success) {
		cerr << "An error occurred during task creation. See the log for more details. Aborting." << endl;
	}
}

void delete_tasks(int argc, char **args) {
	PGAdaptor pg;

	bool success = true;
	for(int i = 0; i < argc; i++) {
		int task_id = stoi(args[i]);
		bool s = pg.delete_task(task_id);
		success = success && s;
	}
	if(!success) {
		cerr << "An error occurred during task deletion. See the log for more details. Aborting." << endl;
	}
}

void complete_tasks(int argc, char **args) {
	PGAdaptor pg;

	bool success = true;
	for(int i = 0; i < argc; i++) {
		int task_id = stoi(args[i]);
		Task task = pg.retrieve_task(task_id);
		task.mark_complete();
		bool s = pg.save_task(task);
		success = success && s;
	}
	if(!success) {
		cerr << "An error occurred during task completion. See the log for more details. Aborting." << endl;
	}
}

void incomplete_tasks(int argc, char **args) {
	PGAdaptor pg;

	bool success = true;
	for(int i = 0; i < argc; i++) {
		int task_id = stoi(args[i]);
		Task task = pg.retrieve_task(task_id);
		task.mark_incomplete();
		bool s = pg.save_task(task);
		success = success && s;
	}
	if(!success) {
		cerr << "An error occurred during task incompletion. See the log for more details. Aborting." << endl;
	}
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

void cleanup() {
	PGAdaptor pg;
	Date today;
	Date last_cleanup_date = pg.retrieve_last_cleanup_date();
	long days_since_last_cleanup = Date::days_between(today, last_cleanup_date);

	if(days_since_last_cleanup == 0) {
		return;
	}

	bool success = true;
	auto tasks = pg.retrieve_tasks_needing_update();

	for(Task t : tasks) {
		//cout << "DEBUG: Updating task: " << t.desc() << endl;
		Date next_occurrence = t.next_occurrence_on_or_after(today);
		//cout << "DEBUG: Next occurrence: " << next_occurrence.to_string() << endl;
		t.set_date(next_occurrence);
		t.mark_incomplete();
		success = pg.save_task(t);
	}

	success = pg.update_last_cleanup_date(today);
	if(!success) {
		cerr << "One or more errors occurred while updating recurring tasks. See the log for details." << endl;
	}
}
