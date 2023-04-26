# **Tetris Game**

This is a simple implementation of the classic game Tetris using C++ and the Windows API. The game consists of falling blocks that the player must rotate and move to fit into a well-defined board. The goal is to complete horizontal lines of blocks without gaps to score points and clear the board.

## **Prerequisites**

- Windows operating system
- Visual Studio or another C++ compiler

## **Getting Started**

1. Clone or download the repository.
2. Open the project in Visual Studio or another C++ compiler.
3. Build and run the program.

## **How to Play**

- Use the right and left arrow keys to move the current falling block horizontally.
- Use the up arrow key to rotate the current falling block clockwise.
- Use the down arrow key to move the current falling block faster.
- Use the 'Z' key to rotate the current falling block counterclockwise.

## **Game Logic**

The program creates a playfield with a boundary and a well-defined size using a dynamically allocated array. It then generates seven tetrominoes with different shapes and colors using a string class for wide characters. The tetrominoes are stored as a 1D matrix but are rotated using a 2D matrix rotation formula.

The main game loop checks for player input and handles the game's logic. It keeps track of the current falling block's position, rotation, and speed, checks if the block fits into the playfield, and updates the score accordingly. If the block hits the bottom or overlaps with another block, the program generates a new block and continues the game.

## **Acknowledgments**

This program was inspired by the classic game Tetris, invented by Alexey Pajitnov in 1984. It uses the Windows API and the C++ language to create a simple but entertaining game.
