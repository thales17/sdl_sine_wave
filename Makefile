test:
	make build;
	make run;

build:
	gcc -std=c99 -c main.c
	gcc main.o -o sine-wave -lm -lSDL2;

run:
	./sine-wave

clean:
	-rm sine-wave;
	-rm *.o;
