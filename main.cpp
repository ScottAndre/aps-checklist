/* Checklist
 * main.cpp
 *
 * Created by Scott Andre
 */

#include <iostream>
#include <cstdlib>
#include <clocale>
#include <vector>
#include <utility>

#include "Date.h"
#include "Task.h"
#include "PGAdaptor.h"

using std::cout;
using std::endl;

void draw(const std::vector<Task> &);

void read_tasks(std::vector<Task> &);
void write_tasks(const std::vector<Task> &);

void construct_test_tasks(std::vector<Task> &task_list) {
	Task one("Bake a cake");
	Task two("Slaughter your enemies admist merriment and laughter");
	Task three("Bake another cake");

	task_list = { one, two, three };
}

#ifndef UNIT_TEST

int main(int argc, char ** argv) {

	setlocale(LC_ALL, "");

	//std::vector<Task> task_list;

	//construct_test_tasks(task_list);

	//draw(task_list);

	PGAdaptor pg;

	cout << "Testing retrieving tasks." << endl;

	//Task t("Slaughter your enemies amidst merriment and laughter");

	//bool b = pg.insert_task(t);

	//cout << "Insertion " << (b ? "succeeded" : "failed") << "." << endl;

	auto tasks = pg.retrieve_daily_tasks();

	draw(tasks);

	return 0;
}

#endif

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
