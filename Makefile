CURR_DIG             = ./
PROG_PATH            = /usr/bin/
PROG_NAME            = rwdisk_stat
DOC                  = README
DOC_PATH             = /usr/share/rwdisk_stat/
SERVICE              = rwdisk_stat.service
SYSTEMD_SERVICE_PATH = /etc/systemd/system/
PGCFG                = pgsql.conf
PGCFG_PATH           = /etc/rwdisk_stat/

default: rwdisk_stat

rwdisk_stat: rwdisk_stat.c
	cc -g -l pq -o rwdisk_stat rwdisk_stat.c params.c pgsql.c

install: rwdisk_stat
	cp $(PROG_NAME) $(PROG_PATH)
	mkdir -p $(DOC_PATH)
	mkdir -p $(PGCFG_PATH)
	cp $(DOC) $(DOC_PATH)
	cp $(PGCFG) $(PGCFG_PATH)$(PGCFG)
	cp $(SERVICE) $(SYSTEMD_SERVICE_PATH)

uninstall:
	rm -f $(DOC_PATH)$(DOC)
	rm -f $(SYSTEMD_SERVICE_PATH)$(SERVICE)
	rm -f $(PROG_PATH)$(PROG_NAME)
	rmdir $(DOC_PATH)
	rmdir $(PGCFG_PATH)
