#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "params.h"

void deploy_arguments(int argc, char *argv[], char *devpath, char *cfgpath, int *interval, char *Daemon){
  int         aflag = 0, /* general flag */
              dflag = 1, /* device flag */
              iflag = 1; /* interval flag */
  int         c;         /* cli key changer */
  char        path[MAX_PATH_LENGTH] = "/sys/block/";
  extern int  optind, optopt;
  extern char *optarg;

  while((c = getopt(argc, argv, ":c:d:i:hD")) != -1){
    switch(c){
      case 'c':
               strcpy(cfgpath, optarg);
               break;
      case 'd':
               strcpy(devpath, optarg);
               dflag--;
               break;
      case 'i':
               *interval = atoi(optarg);
               if(*interval < 1){
                  fprintf(stderr, "Interval must be a digital >= 1\n");
                  exit(EXIT_FAILURE);
               }
               iflag--;
               break;
      case 'h':
               call_help();
               exit(EXIT_SUCCESS);
               break;
      case 'D':
               (*Daemon)++;
               break;
      case ':':
               fprintf(stderr, "Option -%c requires an operand\n", optopt);
               aflag++;
               break;

      case '?':
               fprintf(stderr, "Unrecognized option: -%c\n", optopt);
               aflag++;
    } /* switch */
  } /* while */

    /* post getopt() checker */
  if(aflag){
    call_help();
    exit(EXIT_FAILURE);
  }

  if(dflag){
    fprintf(stderr, "device not set\n");
    exit(EXIT_SUCCESS);
  }

  if(iflag){
    fprintf(stderr, "interval not set\n");
    exit(EXIT_SUCCESS);
  }

  /* control cfgpath */
  if(!strcmp(cfgpath,""))
     strcpy(cfgpath, DEF_CFG_PATH);

  /* set statistic path */
  rindex(devpath, '/') == NULL?devpath:strcpy(devpath, rindex(devpath, '/') + 1);
  /* For developer needs */
  /* printf("%s\n", devpath); */
  if((strlen(path) + strlen("/stat") + strlen(devpath)) > MAX_PATH_LENGTH - 1){
    fprintf(stderr, "Path too long. Maybe something wrong?\n");
    exit(EXIT_FAILURE);
  }
  strcat(path, devpath);
  strcat(path, "/stat");
  strcpy(devpath, path);

}

