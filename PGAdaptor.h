/* Checklist
 * database.h
 *
 * Created by Scott Andre
 *
 * Header file for the Checklist postgres adaptor.
 */

#ifndef CHECKLIST_PG_ADAPTOR
#define CHECKLIST_PG_ADAPTOR

#include <vector>
#include <pqxx/pqxx>

#include "BaseDBAdaptor.h"
#include "Date.h"
#include "Task.h"

#define DATABASE "checklist"
#define DB_TASKS_TABLE "tasks"
#define DB_CONTROL_TABLE "control_data"
#define DB_USER  "checklist"
#define DB_PASS  "checklist"
#define DB_CONTROL_CLEANUP_NAME "last_cleanup_date"

class PGAdaptor : public DBAdaptor {
public:
	PGAdaptor(); // an adaptor object is needed to perform database operations. It creates and destroys a connection per operation requested (function call)
	~PGAdaptor();

	Task retrieve_task(int t_id);
	bool save_task(Task &t);
	bool insert_task(Task &t); // this will set the task's id - which is why it isn't const.
	bool update_task(const Task &t);
	bool delete_task(int t_id);
	bool delete_task(Task &t); // same here. the task's id will be reset to -1 if successfully deleted.
	std::vector<Task> retrieve_active_tasks();

	Date retrieve_last_cleanup_date();
	bool update_last_cleanup_date(const Date d);
	std::vector<Task> retrieve_tasks_needing_update();

private:
	void log_exception(pqxx::sql_error &e);
	Task parse_task_from_row(pqxx::tuple row);

	std::string _connection_string;

};

#endif
