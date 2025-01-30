
main:
	clang ./src/Snake.c ./src/Apple.c ./src/Shape.c ./src/Init.c ./src/Limit.c ./src/Player.c ./src/Main.c -o ./build/snake -lncurses
	./build/snake

main-debug:
	clang ./src/Snake.c ./src/Apple.c ./src/Shape.c ./src/Init.c ./src/Limit.c ./src/Player.c ./src/Main.c -o ./build/snake -lncurses -g
	gdb --tui ./build/snake

snake-test:
	clang ./src/Snake.c ./src/Shape.c ./test/SnakeTest.c -o ./build/snake-test -lcunit
	./build/snake-test

apple-test:
	clang ./src/Apple.c ./src/Shape.c ./test/AppleTest.c -o ./build/apple-test -lcunit
	./build/apple-test

shape-test:
	clang ./src/Shape.c ./test/ShapeTest.c -o ./build/shape-test -lcunit
	./build/shape-test

