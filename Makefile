all: rwdisk_stat
rwdisk_stat: rwdisk_stat.c
	cc -g -o rwdisk_stat rwdisk_stat.c 

