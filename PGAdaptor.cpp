/* Checklist
 * pg_adaptor.cpp
 *
 * Created by Scott Andre
 *
 * The Postgres adaptor for Checklist.
 */

#include <string>
#include <sstream>
#include <pqxx/pqxx>

#include "PGAdaptor.h"

#define TASK_INSERT_COLUMNS "task, date, recurrence, recurrence_interval, recurrence_period, persistent, complete"
#define TASK_INSERT_VALUES "$1, $2, $3, $4, $5, $6, $7"
#define TASK_UPDATE_VALUES "task = $1, date = $2, recurrence = $3, recurrence_interval = $4, recurrence_period = $5, persistent = $6, complete = $7"
#define TASK_UPDATE_ID_VALUE "id = $8"

PGAdaptor::PGAdaptor() {
	std::stringstream conn_stream;
	conn_stream << "hostaddr=127.0.0.1" << " dbname=" << DATABASE << " user=" << DB_USER << " password=" << DB_PASS;
	_connection_string = conn_stream.str();
}

PGAdaptor::~PGAdaptor() {
	// nothing to do
}

bool PGAdaptor::save_task(Task &t) {
	if(t.exists_in_database()) {
		return this->update_task(t);
	}
	else {
		return this->insert_task(t);
	}
}

bool PGAdaptor::insert_task(Task &t) {
	try {
		pqxx::connection connection(_connection_string);

		std::stringstream insert_stream;
		insert_stream << "INSERT INTO " << DB_TASKS_TABLE << " (" << TASK_INSERT_COLUMNS <<
		                 ") VALUES (" << TASK_INSERT_VALUES << ") RETURNING id;";
		connection.prepare("insert", insert_stream.str());

		pqxx::work transaction(connection, "InsertNewTask");
		Task_core core = t.core();
		pqxx::result new_id = transaction.prepared("insert")(core.task)(Date::to_db_representation(core.date))(core.recurrence)(core.recurrence_interval)(core.recurrence_period)(core.persistent)(core.complete).exec();
		transaction.commit(); // TODO: get last insert ID and set t.id to that
		return true;
	}
	catch(pqxx::sql_error &e) { // also what exceptions to catch?
		log_exception(e);
		return false;
	}
}

bool PGAdaptor::update_task(const Task &t) {
	try {
		pqxx::connection connection(_connection_string);

		std::stringstream update_stream;
		update_stream << "UPDATE " << DB_TASKS_TABLE << " SET " << TASK_UPDATE_VALUES <<
		                 " WHERE " << TASK_UPDATE_ID_VALUE << ";";
		connection.prepare("update", update_stream.str());

		pqxx::work transaction(connection, "UpdateTask");
		Task_core core = t.core();
		transaction.prepared("update")(core.task)(Date::to_db_representation(core.date))(core.recurrence)(core.recurrence_interval)(core.recurrence_period)(core.persistent)(core.complete)(core.id).exec();
		transaction.commit();
		return true;
	}
	catch(pqxx::sql_error &e) {
		log_exception(e);
		return false;
	}
}

bool PGAdaptor::delete_task(const Task &t) {
	try {
		pqxx::connection connection(_connection_string);

		std::stringstream delete_stream;
		delete_stream << "DELETE FROM " << DB_TASKS_TABLE << " WHERE id = $1;";
		connection.prepare("delete", delete_stream.str());

		pqxx::work transaction(connection, "DeleteTask");
		Task_core core = t.core();
		transaction.prepared("delete")(core.id).exec();
		transaction.commit();
		return true;
	}
	catch(pqxx::sql_error &e) {
		log_exception(e);
		return false;
	}
}

std::vector<Task> PGAdaptor::retrieve_daily_tasks() {
	std::vector<Task> tasks_container;

	try {
		pqxx::connection connection(_connection_string);

		std::stringstream retrieve_stream;
		retrieve_stream << "SELECT * FROM " << DB_TASKS_TABLE << ";"; // TODO: only retrieve tasks for the current day
		connection.prepare("retrieve", retrieve_stream.str());

		pqxx::work transaction(connection, "RetrieveTasks");
		pqxx::result tasks = transaction.prepared("retrieve").exec();

		for(auto it = tasks.begin(); it != tasks.end(); ++it) {
			Task_core c;
			std::string d;

			pqxx::tuple row = *it;
			row["id"] >> c.id;
			row["task"] >> c.task;
			row["date"] >> d;
			row["recurrence"] >> c.recurrence;
			row["recurrence_interval"] >> c.recurrence_interval;
			row["recurrence_period"] >> c.recurrence_period;
			row["persistent"] >> c.persistent;
			row["complete"] >> c.complete;

			c.date = Date::from_db_representation(d).get_raw_time();
			
			Task t = Task::reconstruct(c);
			tasks_container.push_back(t);
		}

		transaction.commit();
	}
	catch(pqxx::sql_error &e) {
		log_exception(e);
	}

	return tasks_container;
}

// Private methods

void PGAdaptor::log_exception(pqxx::sql_error &e) {
	std::cerr << "A" << e.what() << " error occurred during the following query: " << e.query() << std::endl;
}
