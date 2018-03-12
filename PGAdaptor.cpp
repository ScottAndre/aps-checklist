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

Task PGAdaptor::retrieve_task(int t_id) {
	try {
		pqxx::connection connection(_connection_string);

		std::stringstream retrieve_stream;
		retrieve_stream << "SELECT * FROM " << DB_TASKS_TABLE << " WHERE id = $1;";
		connection.prepare("retrieve", retrieve_stream.str());

		pqxx::work transaction(connection, "RetrieveTask");
		pqxx::result ret_val = transaction.prepared("retrieve")(t_id).exec();
		auto ret_row = ret_val.front();
		Task task = parse_task_from_row(ret_row);
		transaction.commit();
		return task;
	}
	catch(pqxx::sql_error &e) {
		log_exception(e);
		Task fake("FAKE"); // I really don't know what to return in this case. Maybe just exit the program?
		return fake;
	}
}

bool PGAdaptor::save_task(Task &t) {
	if(t.exists_in_database()) {
		return update_task(t);
	}
	else {
		return insert_task(t);
	}
}

bool PGAdaptor::insert_task(Task &t) {
	try {
		pqxx::connection connection(_connection_string);

		std::stringstream insert_stream;
		insert_stream << "INSERT INTO " << DB_TASKS_TABLE << " (" << TASK_INSERT_COLUMNS <<
		                 ") VALUES (" << TASK_INSERT_VALUES << ") RETURNING id;";
		connection.prepare("insert", insert_stream.str());

		pqxx::work transaction(connection, "InsertTask");
		pqxx::result ret_val = transaction.prepared("insert")(t._task)(t._date.to_db_representation())((int)t._recurrence)(t._recurrence_interval)(Task::serialize_recurrence_period(t._recurrence_period))(t._persistent)(t._complete).exec();
		auto ret_row = ret_val.front(); // update the Task's ID so that future saves properly update the task rather than inserting it again
		int id;
		ret_row["id"] >> id;
		t._id = id;
		transaction.commit();
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
		transaction.prepared("update")(t._task)(t._date.to_db_representation())((int)t._recurrence)(t._recurrence_interval)(Task::serialize_recurrence_period(t._recurrence_period))(t._persistent)(t._complete)(t._id).exec();
		transaction.commit();
		return true;
	}
	catch(pqxx::sql_error &e) {
		log_exception(e);
		return false;
	}
}

bool PGAdaptor::delete_task(Task &t) {
	bool success = delete_task(t._id);
	if(success)
		t._id = -1;
	return success;
}

bool PGAdaptor::delete_task(int t_id) {
	try {
		pqxx::connection connection(_connection_string);

		std::stringstream delete_stream;
		delete_stream << "DELETE FROM " << DB_TASKS_TABLE << " WHERE id = $1;";
		connection.prepare("delete", delete_stream.str());

		pqxx::work transaction(connection, "DeleteTask");
		transaction.prepared("delete")(t_id).exec();
		transaction.commit();
		return true;
	}
	catch(pqxx::sql_error &e) {
		log_exception(e);
		return false;
	}
}

std::vector<Task> PGAdaptor::retrieve_active_tasks() {
	std::vector<Task> tasks_container;

	try {
		pqxx::connection connection(_connection_string);

		Date today;

		std::stringstream retrieve_stream;
		retrieve_stream << "SELECT * FROM " << DB_TASKS_TABLE << " WHERE date = $1 OR persistent = true ORDER BY id;";
		connection.prepare("retrieve", retrieve_stream.str());

		pqxx::work transaction(connection, "RetrieveTasks");
		pqxx::result tasks = transaction.prepared("retrieve")(today.to_db_representation()).exec();

		for(auto it = tasks.begin(); it != tasks.end(); ++it) {
			Task new_task = parse_task_from_row(*it);
			tasks_container.push_back(new_task);
		}

		transaction.commit();
	}
	catch(pqxx::sql_error &e) {
		log_exception(e);
	}

	return tasks_container;
}

Date PGAdaptor::retrieve_last_cleanup_date() {
	Date last_cleanup_date;

	try {
		pqxx::connection connection(_connection_string);

		std::stringstream get_cleanup_date_stream;
		get_cleanup_date_stream << "SELECT control_value FROM " << DB_CONTROL_TABLE << " WHERE control_name = '" << DB_CONTROL_CLEANUP_NAME << "';";
		connection.prepare("get_cleanup_date", get_cleanup_date_stream.str());

		pqxx::work transaction(connection, "RetrieveLastCleanupDate");
		pqxx::result res = transaction.prepared("get_cleanup_date").exec();

		std::string d;
		pqxx::tuple row = *(res.begin()); // there's only going to be one row
		row["control_value"] >> d;

		last_cleanup_date = Date::from_db_representation(d);

		transaction.commit();
	}
	catch(pqxx::sql_error &e) {
		log_exception(e);
	}

	return last_cleanup_date;
}

bool PGAdaptor::update_last_cleanup_date(const Date d) {
	try {
		pqxx::connection connection(_connection_string);

		std::stringstream set_cleanup_date_stream;
		set_cleanup_date_stream << "UPDATE " << DB_CONTROL_TABLE << " SET control_value = $1 WHERE control_name = '" << DB_CONTROL_CLEANUP_NAME << "';";
		connection.prepare("set_cleanup_date", set_cleanup_date_stream.str());

		pqxx::work transaction(connection, "UpdateLastCleanupDate");
		transaction.prepared("set_cleanup_date")(d.to_db_representation()).exec();
		transaction.commit();
		return true;
	}
	catch(pqxx::sql_error &e) {
		log_exception(e);
		return false;
	}
}

std::vector<Task> PGAdaptor::retrieve_tasks_needing_update() {
	std::vector<Task> tasks_container;

	try {
		pqxx::connection connection(_connection_string);

		Date today;

		std::stringstream retrieve_stream;
		retrieve_stream << "SELECT * FROM " << DB_TASKS_TABLE << " WHERE recurrence != " << (int)none << " AND persistent = false AND date < $1;";
		connection.prepare("retrieve", retrieve_stream.str());

		pqxx::work transaction(connection, "RetrieveTasksNeedingUpdate");
		pqxx::result tasks = transaction.prepared("retrieve")(today.to_db_representation()).exec();

		for(auto it = tasks.begin(); it != tasks.end(); ++it) {
			Task new_task = parse_task_from_row(*it);
			tasks_container.push_back(new_task);
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

// Helper methods

Task PGAdaptor::parse_task_from_row(pqxx::tuple row) {
	Task *new_task; // must be a pointer, otherwise it attempts to call default constructor, and Task doesn't have one
	std::string t;
	int r;
	int r_i;
	std::string r_p;
	std::string d;
	bool p;

	row["task"] >> t;
	row["date"] >> d;
	row["recurrence"] >> r;
	row["recurrence_interval"] >> r_i;
	row["recurrence_period"] >> r_p;
	row["persistent"] >> p;

	switch((Recurrence)r) {
		case intervallic:
			new_task = new Task(t, r_i, p, Date::from_db_representation(d)); break;
		case periodic:
			new_task = new Task(t, r_p, p, Date::from_db_representation(d)); break;
		default:
			new_task = new Task(t, p, Date::from_db_representation(d)); break;
	}

	Task reconstructed_task = *new_task;
	delete new_task;

	row["id"] >> reconstructed_task._id;
	row["complete"] >> reconstructed_task._complete;

	return reconstructed_task;
}
