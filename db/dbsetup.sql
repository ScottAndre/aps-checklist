DROP DATABASE IF EXISTS checklist;
DROP ROLE IF EXISTS checklist;

CREATE ROLE checklist WITH LOGIN ENCRYPTED PASSWORD 'checklist';
CREATE DATABASE checklist WITH OWNER checklist;
