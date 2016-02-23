

all: dv

dv: dv.c
	gcc -o dv dv.c

install: dv
	sudo cp dv /usr/local/bin/

