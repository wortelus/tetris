# TETRIS by wortelus

This is 1st semester project for VŠB-TUO programming class.

Simple, yet fully functional game of TETRIS using pure C and SDL library.

## Building
This game accompanies CMakeLists.txt file for you to build this beautiful game on your system.
Built and developed on Linux.


    cmake .
    make

## Possible Errors
You can get font missing error, in that case you should fix its path in ```main.c``` before building.

Specifically, the ```/usr/share/fonts/truetype/freefont/FreeSerif.ttf``` is the default path in Linux.

---
by wortelus |
wortelus.eu