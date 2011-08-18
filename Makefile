all:
	gcc -o taoc tclient.c
	gcc -o taos tserver.c

clean: 
	rm *.o *~