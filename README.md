# Menu-Program-Generator
A program to take in a menu as input from a text file, generate a general purpose binary tree from it and then generate a C file which makes the menu using switch statements for efficiency.
the input menu uses a tab to add a child. Menu items in the same level are siblings.

To run
```
gcc menugenerator.c -o prog
./prog < menu.dat > out.c
gcc out.c -o out
./a.out
```
You can now select sub menus by choosing 1,2,3... 0 to go back a level and an invalid entry stays in the same level
