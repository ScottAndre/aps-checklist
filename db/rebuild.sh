#!/bin/bash
if [ `whoami` != postgres ]; then
	echo "Please ensure you are running this script as a user with sufficient database permissions."
fi

PSQL_OPTS="-e"

psql $PSQL_OPTS -f db/dbsetup.sql
psql $PSQL_OPTS --dbname=checklist -f db/schema.sql
