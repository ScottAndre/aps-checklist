CREATE TABLE tasks (
	id                  SERIAL PRIMARY KEY,
	task                TEXT NOT NULL,
	date                DATE NOT NULL,
	recurrence          CHAR(1) NOT NULL,
	recurrence_interval SMALLINT,
	recurrence_period   VARCHAR(7),
	persistent          BOOLEAN NOT NULL,
	complete            BOOLEAN NOT NULL
);
GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE tasks TO checklist;
ALTER TABLE tasks OWNER TO checklist;
