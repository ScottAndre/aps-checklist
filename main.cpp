/* Checklist
 * main.cpp
 *
 * Copyright Scott Andre, 2014
 */

#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

#include "Date.h"
#include "Task.h"

void draw();

int main(int argc, char ** argv) {

	//std::vector<Task> task_list;

	Date now;

	std::cout << now << "\n";

	Date next = std::move(now);

	std::cout << next << "\n" << "Undefined behavior!!!!" << "\n";

	std::cout << now << "\n";

	//draw(task_list);

	return 0;
}

/*
 * Draws the checklist interface.
 * Currently does not support Windows systems. Possible that it someday will, but unlikely.
 */
void draw(std::vector<Task> &task_list) {
	system("clear");
	Date now;
	std::cout << now.without_time() << "\n\n";
}
