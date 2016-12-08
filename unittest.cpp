/* Checklist
 * Unit Testing Module
 *
 * Do not compile in release product.
 *
 * Created by Scott Andre
 */

#include <iostream>
#include <string>
#include <sstream>

#include "Date.h"
#include "Task.h"
#include "main.cpp"

void date_unittest();
void task_unittest();
void main_unittest();

int main(int argc, char ** argv) {
	std::cout << "Running unit test module.\n";
	std::cout << "\nTesting Date class...\n";
	date_unittest();
	std::cout << "\nTesting Task class...\n";
	task_unittest();
	std::cout << "\nTesting main...\n";
	main_unittest();
}

template<typename A, typename B>
void print_error(std::string test_name, A expected, B result) {
	std::cerr << "\033[91mFailed test of " << test_name << "\n" <<
	    "\033[32mExpected value:   \033[0m" << expected << "\n" <<
	    "\033[31mActual value was: \033[0m" << result << "\n\n";
}

void date_unittest() {
	Date test_date(88888);

	// Date::to_string()	
	[&test_date]() {
		std::string expected = "Friday, 02 January 1970";
		auto result = test_date.to_string();
		if(result != expected) {
			print_error("Date::to_string()", expected, result);
		}
	}();

	// Date::get_day()
	[&test_date]() {
		std::string expected = "Friday";
		auto result = test_date.get_day();
		if(result != expected) {
			print_error("Date::get_day()", expected, result);
		}
	}();

	// Date::localtime()
	[&test_date]() {
		std::cerr << "Note to self, your localtime() test needs to somehow account for time difference\n";
		time_t expected = 86400;
		auto result = test_date.localtime();
		if(result != expected) {
			print_error("Date::localtime()", expected, result);
		}
	}();
}

std::string core_to_string(Task_core);
bool operator!=(Task_core, Task_core);

void task_unittest() {
	Date test_date;

	Task n_test_task("Testing", true, test_date);           // Non-recurring, persistent task
	Task p_test_task("Testing", "MTWRF", false, test_date); // Periodically recurring task
	Task i_test_task("Testing", 2, false, test_date);       // Intervallically recurring task
	

	/* Task persistence tests */

	// Task::core()
	[&n_test_task, &p_test_task, &i_test_task, &test_date]() {
		Task_core n_expected;
		n_expected.id = -1;
		n_expected.task = "Testing";
		n_expected.date = test_date.get_raw_time();
		n_expected.recurrence = 0;
		n_expected.recurrence_interval = 0;
		n_expected.recurrence_period = "";
		n_expected.persistent = true;
		n_expected.complete = false;

		Task_core n_result = n_test_task.core();

		Task_core p_expected;
		p_expected.id = -1;
		p_expected.task = "Testing";
		p_expected.date = test_date.get_raw_time();
		p_expected.recurrence = 2;
		p_expected.recurrence_interval = 0;
		p_expected.recurrence_period = "MTWRF";
		p_expected.persistent = false;
		p_expected.complete = false;

		Task_core p_result = p_test_task.core();

		Task_core i_expected;
		i_expected.id = -1;
		i_expected.task = "Testing";
		i_expected.date = test_date.get_raw_time();
		i_expected.recurrence = 1;
		i_expected.recurrence_interval = 2;
		i_expected.recurrence_period = "";
		i_expected.persistent = false;
		i_expected.complete = false;

		Task_core i_result = i_test_task.core();

		if(n_result != n_expected) {
			print_error("Task::core()", core_to_string(n_expected), core_to_string(n_result));
		}
		if(p_result != p_expected) {
			print_error("Task::core()", core_to_string(p_expected), core_to_string(p_result));
		}
		if(i_result != i_expected) {
			print_error("Task::core()", core_to_string(i_expected), core_to_string(i_result));
		}
	}();

	// Task::reconstruct()
	[&n_test_task, &p_test_task, &i_test_task]() {
		Task_core n_core = n_test_task.core();
		Task_core p_core = p_test_task.core();
		Task_core i_core = i_test_task.core();

		Task n_result = Task::reconstruct(n_core);
		Task p_result = Task::reconstruct(p_core);
		Task i_result = Task::reconstruct(i_core);

		if(n_result != n_test_task) {
			print_error("Task::reconstruct()", core_to_string(n_core), core_to_string(n_result.core()));
		}
		if(p_result != p_test_task) {
			print_error("Task::reconstruct()", core_to_string(p_core), core_to_string(p_result.core()));
		}
		if(i_result != i_test_task) {
			print_error("Task::reconstruct()", core_to_string(i_core), core_to_string(i_result.core()));
		}
	}();

}

void main_unittest() {

}

/* Helpers */

bool operator==(Task_core c1, Task_core c2) {
	return (c1.id == c2.id &&
	        c1.task == c2.task &&
	        c1.date == c2.date &&
	        c1.recurrence == c2.recurrence &&
	        c1.recurrence_interval == c2.recurrence_interval &&
	        c1.recurrence_period == c2.recurrence_period &&
	        c1.persistent == c2.persistent &&
	        c1.complete == c2.complete);
}

bool operator!=(Task_core c1, Task_core c2) {
	return !(c1 == c2);
}

std::string core_to_string(Task_core core) {
	std::stringstream result;
	result << "{" <<
		"\n\tid: " << core.id <<
		"\n\ttask: " << core.task <<
		"\n\tdate: " << core.date <<
		"\n\trecurrence: " << core.recurrence <<
		"\n\trecurrence_interval: " << core.recurrence_interval <<
		"\n\trecurrence_period: " << core.recurrence_period <<
		"\n\tpersistent: " << core.persistent <<
		"\n\tcomplete: " << core.complete <<
		"\n}\n";
	return result.str();
}
