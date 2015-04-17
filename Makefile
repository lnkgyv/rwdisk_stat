all: rwdisk_stat
rwdisk_stat: rwdisk_stat.c
	cc -g -l pq -o rwdisk_stat rwdisk_stat.c params.c pgsql.c

