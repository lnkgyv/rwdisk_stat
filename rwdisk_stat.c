#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "rwdisk_stat.h"
#include "pgsql.h"
#include "params.h"

int main(int argc, char *argv[]){
  long long  past[2];
  long long  curr[2];
  char       devpath[MAX_PATH_LENGTH] = "";
  char       cfgpath[MAX_PATH_LENGTH] = "";
  char       query[MAX_QUERY_LENGTH] = "";
  FILE       *stat;
  time_t     curr_time;
  int        interval;
  char       Daemon = 0;
  char       ret;

  const char *conninfo;
  PGconn     *conn;

  deploy_arguments(argc, argv, devpath, cfgpath, &interval, &Daemon);
  /* For developer needs */
  /*printf("Arguments:\n\tdevpath=%s \n\tcfgpath=%s \n\tinterval=%d\n",devpath, cfgpath, interval);*/

  if(Daemon){
    ret = daemon(NOCHDIR, NOCLOSE);
    check_ret_uni(ret, -1, "daemon");
  }

  conn = connect_to_db(cfgpath);

  stat = fopen(devpath, "r");
  check_ret_uni(stat, (FILE *)NULL, "fopen statfile");

  /* disable buffering needed for seek with buffering*/
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

    sprintf(query, "insert into results values(default, to_timestamp(%lld), %lld, %lld, %lld)", 
                   time((time_t *)NULL), (curr[0] - past[0]) * SECTOR_SZ / 1024, (curr[1] - past[1]) * SECTOR_SZ / 1024, PLUGIN_ID);

    insert_to_db(conn, query);

    /* For developer needs */
    /* printf("%lld   %lld              %lld\n", time((time_t *)NULL), (curr[0] - past[0]) * SECTOR_SZ / 1024, (curr[1] - past[1]) * SECTOR_SZ / 1024); */

    past[0] = curr[0];
    past[1] = curr[1];
  }

  PQfinish(conn);

  ret = fclose(stat);
  check_ret_uni(stat, (FILE *)EOF, "fclose");

  exit(EXIT_SUCCESS);
}
