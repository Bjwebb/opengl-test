CC = g++

all:
	$(CC) main.cpp loadworld.cpp collision.cpp font.cpp -o test -lSDL -lglut -g

clean:
	@echo Cleaning up...
	@rm test
	@echo Done.

