
main:
	clang ./src/Snake.c ./src/Shape.c ./src/Main.c -o ./build/snake -lncurses
	./build/snake
