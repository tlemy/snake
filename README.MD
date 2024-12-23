# SNAKE
## Context
This project is a text based user interface application with Ncurses simulating the game 'Snake'.<br />
Every time the snake eats an apple, it grows by one square.<br />
If the snake touches itself or the borders, it's game over.<br />
Then, pressing the 'ENTER' key restarts a new game.<br />
To move the snake: 'w' to go up, 's' to go down, 'a' to go left and 'd' to go right.<br />
'Control C' exits the game.<br />
The score can be seen in the upper left corner.<br />
Have fun!<br />

## Prerequisites
* Have 'Ncurses' installed
* Have 'clang' installed
* Have 'make' installed
* (This application was developed and tested on a Linux distribution) 

## Compile and Run
`$ sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev`<br />
`$ mkdir build`<br />
`$ make shape-test`<br />
`$ make apple-test`<br />
`$ make snake-test`<br />
`$ make main`<br />
