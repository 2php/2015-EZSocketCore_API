CC=gcc

all: EX1 EX2 EX3

ex1: EZSocketCore.o EZUserdef.o EZWeb.o Example1_Timeserver.o
	$(CC) EZSocketCore.o EZUserdef.o EZWeb.o Example1_Timeserver.o -o Timeserver
ex2: EZSocketCore.o EZUserdef.o EZWeb.o Example2_Timeclient.o
	$(CC) EZSocketCore.o EZUserdef.o EZWeb.o Example2_Timeclient.o -o Timeclient
ex3: EZSocketCore.o EZUserdef.o EZWeb.o Example3_Webserver.o
	$(CC) EZSocketCore.o EZUserdef.o EZWeb.o Example3_Webserver.o -o Webserver

EZSocketCore.o: EZSocketCore.c
	$(CC) EZSocketCore.c -c
EZUserdef.o: EZUserdef.c
	$(CC) EZUserdef.c -c
EZWeb.o: EZWeb.c
	$(CC) EZWeb.c -c
Example1_Timeserver.o: Example1_Timeserver.c
	$(CC) Example1_Timeserver.c -c
Example2_Timeclient.o: Example2_Timeclient.c
	$(CC) Example2_Timeclient.c -c
Example3_Webserver.o: Example3_Webserver.c
	$(CC) Example3_Webserver.c -c	
	
clean:
	rm -rf *.o Timeserver Timeclient Webserver
