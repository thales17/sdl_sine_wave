test:
	make build;
	make run;

build:
	gcc -std=c99 -c main.c
	gcc main.o -o sdl-starter -lm -lSDL2;

run:
	./sdl-starter

clean:
	-rm sdl-starter;
	-rm *.o;
