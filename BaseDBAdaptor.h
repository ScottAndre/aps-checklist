/* Checklist
 * base_db_adaptor.h
 *
 * Created by Scott Andre
 *
 * The abstract base class for database adaptors. Specifies only an interface for adaptor behavior.
 */

#ifndef CHECKLIST_BASE_DB_ADAPTOR
#define CHECKLIST_BASE_DB_ADAPTOR

#include <vector>

#include "Task.h"

class DBAdaptor {
public:
	virtual bool save_task(Task &t) = 0;
	virtual bool insert_task(Task &t) = 0;
	virtual bool update_task(const Task &t) = 0;
	virtual bool delete_task(Task &t) = 0;
	virtual std::vector<Task> retrieve_active_tasks() = 0;
};

#endif
