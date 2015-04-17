#ifndef RWDISK_STAT_H
#define RWDISK_STAT_H

#define TRUE             1
#define PLUGIN_ID        1 /* I think about it later */
#define SECTOR_SZ        512

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


#endif /* RWDISK_STAT_H */
