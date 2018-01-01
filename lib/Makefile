all: libcsapp.a

libcsapp.a:
	gcc -I../include csapp.c  -lpthread -c -o csapp.o 
	ar rcs $@ csapp.o

clean:
	rm -rf libcsapp.a *.o
