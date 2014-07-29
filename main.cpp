/* Checklist
 * main.cpp
 *
 * Copyright Scott Andre, 2014
 */

#include <iostream>
#include <cstdlib>

#include "Date.h"
#include "Task.h"

int main(int argc, char ** argv) {

	Date d;

	std::cout << d.getDay() << std::endl;

	return 0;
}

/*
 * Draws the checklist interface.
 * Currently does not support Windows systems. Possible that it someday will, but unlikely.
 */
void draw() {
	system("clear");
	Date now;
	std::cout << now.withoutTime() << "\n\n";
}
