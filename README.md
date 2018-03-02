# Crosswords-Game
## Introduction
### What is this game ?
It is a game I developed for a school project. There is a grid of hidden words and the purpose of the game is to find the different hidden words. There are two modes: one where the computer can play and another one where you can play. There is a file containing all the hidden words of the grid. The game is over when you have find all the words in this file.

There is no graphic interface, everything happens in the command.

When you launch the game, this appears:
> Welcome ! Write "1" to make the computer play, write "2" to play yourselves, write "3" to quit.

For instance, if you choose to make the computer play, you will get:

![Alt text](crosswords.jpg?raw=true "Crosswords game")

If you choose to play yourselves, the list of word you have to find is written. When you find a word, you have to press "1", "2", ... depending on where you saw it (from left to right, from right to left, ...), you have to write the number of the line and column where the word begins and the number of the word in the list.

When you have find all the words in the list, you're done ! You can start again if you want.

### Code
This code was written in C. There are four different files:
1. Prog.c : where all the functions are written
2. Prog.h : where all the prototypes are written
3. main.c : where the main program is written
4. makefile : a makefile to compile the program

There are also four grids for you to play.

### How to execute the game
To execute my game, you can write "make" in your console to compile it and then you can write "./Jeu Grille1.txt Liste1.txt" in your console in order to play with the first grid. If you want to play with the 2nd or 3rd grid you just have to change the number of the grid.

### To-do list
- Graphic interface.


Enjoy !
