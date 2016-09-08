test:
	make build;
	make run;

build:
	gcc -std=c99 -c main.c -I/usr/local/include
	gcc main.o -o sine-wave -lm -L/usr/local/lib -lSDL2;

run:
	./sine-wave

clean:
	-rm sine-wave;
	-rm *.o;
