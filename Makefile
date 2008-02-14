CC = g++

all:
	$(CC) main.cpp loadworld.cpp collision.cpp font.cpp -o test -lSDL -lglut

clean:
	@echo Cleaning up...
	@rm test
	@echo Done.

