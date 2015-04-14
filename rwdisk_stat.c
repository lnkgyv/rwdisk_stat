#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pgsql/libpq-fe.h>

#define TRUE            1
#define MAX_PATH_LENGTH 30
#define SECTOR_SZ       512

#define check_ret_uni(ret, must, fn)  \
         if(ret == must){             \
           perror(#fn);               \
           exit(EXIT_FAILURE);        \
         }                            \

#define check_ret_nzero(ret, fn)      \
         if(ret){                     \
           perror(#fn);               \
           exit(EXIT_FAILURE);        \
         }                            \

#define check_ret_zero(ret, fn)       \
         if(!ret){                    \
           perror(#fn);               \
           exit(EXIT_FAILURE);        \
         }                            \

static void exit_nicely(PGconn *conn){
  PQfinish(conn);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
  long long  past[2];
  long long  curr[2];
  char       path[MAX_PATH_LENGTH] = "/sys/block/";
  char       query[255];
  FILE       *stat;
  time_t     curr_time;
  int        interval;
  char       ret;

  const char *conninfo;
  PGconn     *conn;
  PGresult   *res;

  conninfo = "dbname = monitoring user = watch password = watch";
  conn = PQconnectdb(conninfo);
  if(PQstatus(conn) != CONNECTION_OK){
    fprintf(stderr, "Connection to database failed: %s",
    PQerrorMessage(conn));
    exit_nicely(conn);
  }

  if(argc < 3){
    printf("Usage: %s <device_without_path> <interval_in_seconds>\n", argv[0]);
    printf("Example: %s sda 5\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if((strlen(path) + strlen("/stat") + strlen(argv[1])) > MAX_PATH_LENGTH - 1){
    printf("Path too long. Maybe something wrong?\n");
    exit(EXIT_FAILURE);
  }
  strcat(path, argv[1]);
  strcat(path, "/stat");

  ret = sscanf(argv[2], "%d", &interval);
  if(!ret || !interval){
    printf("Interval must be a digital >= 1\n");
    exit(EXIT_FAILURE);
  }

  stat = fopen(path, "r");
  check_ret_uni(stat, (FILE *)NULL, "fopen");

  /* disable buffering */
  ret = setvbuf(stat, (char *)NULL, _IONBF, 0);
  check_ret_nzero(ret, "setvbuff");

  ret = fscanf(stat, "%*lld %*lld %lld %*lld %*lld %*lld %lld", past, past + 1);
  check_ret_uni(stat, (FILE *)NULL, "fscanf");

  /* For developer needs */
  /*printf("Time         RKbytes        WKbytes\n");*/
  while(TRUE){
    sleep(interval);

    curr_time = time((time_t *)NULL);
    rewind(stat); /* seek to begin of file */
    ret = fscanf(stat, "%*lld %*lld %lld %*lld %*lld %*lld %lld", curr, curr + 1);
    check_ret_uni(stat, (FILE *)NULL, "fscanf");

    sprintf(query, "insert into results values(default, to_timestamp(%lld), %lld, %lld, 1)", time((time_t *)NULL), (curr[0] - past[0]) * SECTOR_SZ / 1024, (curr[1] - past[1]) * SECTOR_SZ / 1024);

    res = PQexec(conn, query);
    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      fprintf(stderr, "INSERT failed: %s", PQerrorMessage(conn));
      PQclear(res);
      exit_nicely(conn);
    }

    /* For developer needs */
    /* printf("%lld   %lld              %lld\n", time((time_t *)NULL), (curr[0] - past[0]) * SECTOR_SZ / 1024, (curr[1] - past[1]) * SECTOR_SZ / 1024); */
    past[0] = curr[0];
    past[1] = curr[1];
  }

  ret = fclose(stat);
  check_ret_uni(stat, (FILE *)EOF, "fclose");

  exit(EXIT_SUCCESS);
}
