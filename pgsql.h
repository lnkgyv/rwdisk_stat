#ifndef PGSQL_H
#define PGSQL_H

#include <stdlib.h>
#include <pgsql/libpq-fe.h>

/* Big part of this from official documentation examples.
   Maybe I something lose, but it's work :)
   Link: http://www.postgresql.org/docs/9.1/static/libpq.html */

#define MAX_CONNECTION_STRING     100

#define exit_nicely(conn)         \
             PQfinish(conn);      \
             exit(EXIT_FAILURE);  \

/* get arguments from cfgfile and connect to db */
PGconn *connect_to_db(char *cfgpath);

/* put data to db */
void insert_to_db(PGconn *conn, char *query);

#endif /* PGSQL_H */
