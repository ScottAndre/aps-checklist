/* Checklist
 * main.cpp
 *
 * Copyright Scott Andre, 2014
 */

#include <iostream>
#include <cstdlib>
#include <clocale>
#include <vector>
#include <utility>

#include "Date.h"
#include "Task.h"

void draw(const std::vector<Task> &);

void construct_test_tasks(std::vector<Task> &task_list) {
	Task one("Bake a cake", false);
	Task two("Slaughter your enemies admist merriment and laughter", false);
	Task three("Bake another cake", false);

	task_list = { one, two, three };
}

int main(int argc, char ** argv) {

	setlocale(LC_ALL, "");

	std::vector<Task> task_list;

	construct_test_tasks(task_list);

	draw(task_list);

	return 0;
}

/*
 * Draws the checklist interface.
 * Currently does not support Windows systems. Possible that it someday will, but unlikely.
 */
void draw(const std::vector<Task> &task_list) {
	system("clear");
	Date now;
	std::cout << now << "\n\n";

	for(Task t : task_list) {
		std::cout << '\t' << u8"\u2611" << u8"\u2610" << ' ' << t.get() << "\n";
	}
}
