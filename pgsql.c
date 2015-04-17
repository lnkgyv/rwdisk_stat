#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "pgsql.h"
#include "rwdisk_stat.h"
#include <pgsql/libpq-fe.h>

PGconn *connect_to_db(char *cfgpath){
  char buf[LINE_MAX];
  char conninfo[MAX_CONNECTION_STRING] = "";
  FILE *cfgfile;
  PGconn *conn;

  cfgfile = fopen(cfgpath, "r");
  check_ret_uni(cfgfile, (FILE *)NULL, "fopen");

  /* get connect string */
  while(fgets(buf, LINE_MAX, cfgfile)){
    if(!rindex(buf, '#')){
      /* check connection string legth */
      if(strlen(strncat(conninfo, buf, strlen(buf) - 1)) > MAX_CONNECTION_STRING){
        fprintf(stderr, "Connection string is too long. Maybe something wrong...\n");
        exit(EXIT_FAILURE);
      }else{
        strcat(conninfo, " ");
      }
    }
    else{
      continue; /* skip comment string */
    }
  }

  /* For developer needs */
  /* printf("connection string: %s\n", conninfo); */

  conn = PQconnectdb(conninfo);
  if(PQstatus(conn) != CONNECTION_OK){
    fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
    exit_nicely(conn);
  }

  return conn;
}

void insert_to_db(PGconn *conn, char *query){
  PGresult *res;

  res = PQexec(conn, query);
  if(PQresultStatus(res) != PGRES_COMMAND_OK){
    fprintf(stderr, "INSERT failed: %s", PQerrorMessage(conn));
    PQclear(res);
    exit_nicely(conn);
  }

  PQclear(res);
}

