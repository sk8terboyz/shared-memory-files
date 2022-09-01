all: filesender filereceiver

filesender: shm_filesender.c
	gcc -o filesender shm_filesender.c -lrt

filereceiver: shm_filereceiver.c
	gcc -o filereceiver shm_filereceiver.c -lrt
