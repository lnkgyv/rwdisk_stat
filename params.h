#ifndef PARAMS_H
#define PARAMS_H

#define MAX_PATH_LENGTH  30
#define MAX_QUERY_LENGTH 255
#define DEF_CFG_PATH     "/etc/adm/rwdisk_stat.conf"

#define call_help()                                                                                                        \
    fprintf(stderr, "\nUsage: %s [OPTION] ... \n", argv[0]);                                                               \
    fprintf(stderr, "-c        Database connect parameters ('by default-if no set' equal /etc/adm/rwdisk_stat.conf) \n");  \
    fprintf(stderr, "-d        Disk device (/dev/sdX, /dev/hdX) \n");                                                      \
    fprintf(stderr, "-i        Interval (in seconds but not '0')\n");                                                      \
    fprintf(stderr, "-D        Deamonize                        \n");                                                      \
    fprintf(stderr, "-h        Print help. This message')\n\n");                                                           \
    fprintf(stderr, "Example:\n %s -d /dev/sda -i 5\n\n", argv[0]);                                                        \

/* deploy argumants for main*/
void deploy_arguments(int argc, char *argv[], char *devpath, char *cfgpath, int *interval);

#endif /* PARAMS_H */