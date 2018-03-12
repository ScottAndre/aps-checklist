CREATE TABLE tasks (
	id                  SERIAL PRIMARY KEY,
	task                TEXT NOT NULL,
	date                DATE NOT NULL,
	recurrence          INTEGER NOT NULL,
	recurrence_interval INTEGER,
	recurrence_period   VARCHAR(7),
	persistent          BOOLEAN NOT NULL,
	complete            BOOLEAN NOT NULL
);
GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE tasks TO checklist;
ALTER TABLE tasks OWNER TO checklist;

CREATE TABLE control_data (
	control_name        VARCHAR(255) PRIMARY KEY,
	control_value       VARCHAR(255)
);
GRANT SELECT, UPDATE ON TABLE control_data TO checklist;
ALTER TABLE control_data OWNER TO checklist;
INSERT INTO control_data (control_name, control_value) VALUES ('last_cleanup_date', NULL);
