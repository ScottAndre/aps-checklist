/* Checklist
 * main.cpp
 *
 * Copyright Scott Andre, 2014
 */

#include <iostream>
#include <cstdlib>
#include <vector>

#include "Date.h"
#include "Task.h"

void draw();

int main(int argc, char ** argv) {

	//std::vector<Task> task_list;

	Date d;

	std::cout << d.get_day() << std::endl;

	draw();

	return 0;
}

/*
 * Draws the checklist interface.
 * Currently does not support Windows systems. Possible that it someday will, but unlikely.
 */
void draw() {
	system("clear");
	Date now;
	std::cout << now.without_time() << "\n\n";
}
