
main:
	clang ./src/modules/Snake.c ./src/modules/Apple.c ./src/modules/Shape.c ./src/modules/GameMap.c ./src/modules/Player.c ./src/modules/Key.c ./src/Main.c -o ./build/main -lncurses -I./include
	./build/main

main-debug:
	clang ./src/modules/Snake.c ./src/modules/Apple.c ./src/modules/Shape.c ./src/modules/GameMap.c ./src/modules/Player.c ./src/modules/Key.c ./src/Main.c -o ./build/main -lncurses -I./include -g
	gdb --tui ./build/main

player-test:
	clang ./src/modules/Player.c ./src/modules/Snake.c ./src/modules/Shape.c ./test/PlayerTest.c -o ./build/player-test -lncurses -lcunit -I./include
	./build/player-test

snake-test:
	clang ./src/modules/Snake.c ./src/modules/Shape.c ./test/SnakeTest.c -o ./build/snake-test -lncurses -lcunit -I./include
	./build/snake-test

apple-test:
	clang ./src/modules/Apple.c ./src/modules/Shape.c ./test/AppleTest.c -o ./build/apple-test -lncurses -lcunit -I./include
	./build/apple-test

shape-test:
	clang ./src/modules/Shape.c ./test/ShapeTest.c -o ./build/shape-test -lncurses -lcunit -I./include
	./build/shape-test

