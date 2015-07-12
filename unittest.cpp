/* Checklist
 * Unit Testing Module
 *
 * Do not compile in release product.
 *
 * Copyright Scott Andre 2015
 */

#include <iostream>
#include <string>

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

	// Test Date::to_string()	
	[&test_date]() {
		std::string expected = "Friday, 02 January 1970";
		auto result = test_date.to_string();
		if(result != expected) {
			print_error("Date::to_string()", expected, result);
		}
	}();

	// Test Date::get_day()
	[&test_date]() {
		std::string expected = "Friday";
		auto result = test_date.get_day();
		if(result != expected) {
			print_error("Date::get_day()", expected, result);
		}
	}();

	// Test Date::set_time() - should have automatically run when test_date was created
	[&test_date]() {
		std::cerr << "Note to self, your set_time() test needs to somehow account for time difference\n";
		time_t expected = 86400;
		auto result = test_date.get_raw_time();
		if(result != expected) {
			print_error("Date::set_time()", expected, result);
		}
	}();
}

void task_unittest() {
	Task n_test_task("Testing;");          // Non-recurring task
	Task p_test_task("Testing", "MTWRF");  // Periodically recurring task
	Task i_test_task("Testing", 2);        // Intervallically recurring task
	

	/* Serialization tests */

	// Test get_next_token()
	[]() {
		std::string serialized_str = "on\\;e;two;three;";
		std::string expected_val = "on\\;e";
		std::string expected_remainder = "two;three;";
		auto result_val = Unittest::_task_internal_get_next_token(serialized_str);
		if(result_val != expected_val || serialized_str != expected_remainder) {
			print_error("get_next_token() (Task internal)", expected_val + " (value)", result_val + " (value)");
			print_error("get_next_token() (Task internal)", expected_remainder + " (remainder)", serialized_str + " (remainder)");
		}
	}();

	// Test escape_delimiter()
	[]() {
		std::string str = "whee;whee;yeehaw;;";
		std::string expected = "whee\\;whee\\;yeehaw\\;\\;";
		auto result = Unittest::_task_internal_escape_delimiter(str);
		if(result != expected) {
			print_error("escape_delimiter() (Task internal)", expected, result);
		}
	}();

	// Task::serialize()
	[&n_test_task, &i_test_task, &p_test_task]() {
		Date today;
		std::string n_expected = "Testing\\;;" + std::to_string(today.get_raw_time()) + ";0;0;0;";
		std::string i_expected = "Testing;" + std::to_string(today.get_raw_time()) + ";1;2;0;0;";
		std::string p_expected = "Testing;" + std::to_string(today.get_raw_time()) + ";2;MTWRF;0;0;";
		auto n_result = n_test_task.serialize();
		auto i_result = i_test_task.serialize();
		auto p_result = p_test_task.serialize();
		if(n_result != n_expected) 
			print_error("Task::serialize() (r = none)", n_expected, n_result);
		if(i_result != i_expected)
			print_error("Task::serialize() (r = intervallic)", i_expected, i_result);
		if(p_result != p_expected)
			print_error("Task::serialize() (r = periodic)", p_expected, p_result);
	}();

	// Task::deserialize()
}

void main_unittest() {

}
