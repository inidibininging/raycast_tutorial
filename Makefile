build:
	gcc -Wall -std=c99 ./src/*.c -o game -L/usr/local/include/SDL2 -lSDL2main -lSDL2

run:
	./game

clean:
	rm game
