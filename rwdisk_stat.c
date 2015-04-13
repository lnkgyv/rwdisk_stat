#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

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

int main(int argc, char *argv[]){
  FILE *stat;
  char path[MAX_PATH_LENGTH] = "/sys/block/";
  char ret;
  long long past[2], curr[2];
  time_t curr_time;
  int interval;

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

  printf("Time         RKbytes        WKbytes\n");
  while(TRUE){
    sleep(interval);
    rewind(stat); /* seek to begin of file */
    ret = fscanf(stat, "%*lld %*lld %lld %*lld %*lld %*lld %lld", curr, curr + 1);
    check_ret_uni(stat, (FILE *)NULL, "fscanf");
    printf("%lld   %lld              %lld\n", time((time_t *)NULL), (curr[0] - past[0]) * SECTOR_SZ / 1024, (curr[1] - past[1]) * SECTOR_SZ / 1024);
    past[0] = curr[0];
    past[1] = curr[1];
  }

  ret = fclose(stat);
  check_ret_uni(stat, (FILE *)EOF, "fclose");

  exit(EXIT_SUCCESS);
}
