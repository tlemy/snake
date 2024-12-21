
main:
	clang ./src/Snake.c ./src/Shape.c ./src/Main.c -o ./build/snake -lncurses
	./build/snake

snake-test:
	clang ./src/Snake.c ./src/Shape.c ./test/SnakeTest.c -o ./build/snake-test -lcunit
	./build/snake-test

shape-test:
	clang ./src/Shape.c ./test/ShapeTest.c -o ./build/shape-test -lcunit
	./build/shape-test

