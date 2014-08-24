/* Checklist
 * Unit Testing Module
 *
 * Do not compile in release product.
 *
 * Copyright Scott Andre 2014
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
		"\033[32mExpected value: \033[0m" << expected << "\n" <<
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
	Task n_test_task("Testing", false);          // Non-recurring task
	Task i_test_task("Testing", "MTWRF", false); // Periodically recurring task
	Task p_test_task("Testing", 2, false);       // Intervallically recurring task
	

	/* Serialization tests */

	// Test get_next_token()
	[]() {
		std::string serialized_str = "one;two;three;";
		std::string expected_val = "one";
		std::string expected_remainder = "two;three;";
		auto result_val = Unittest::_task_internal_get_next_token(serialized_str);
		if(result_val != expected_val || serialized_str != expected_remainder) {
			print_error("get_next_token() (Task internal)", expected_val + " (value)", result_val + " (value)");
			print_error("get_next_token() (Task internal)", expected_remainder + " (remainder)", serialized_str + " (remainder)");
		}
	}();

	// Task::serialize()

	// Task::deserialize()
}

void main_unittest() {

}
