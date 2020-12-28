# tui-checkers
A somewhat mediocre, but fully functional, checkers engine with a sloppy TUI on top of it. 

#Usage
Run the `checkers` binary to run the game. To make a move, input the move you would like to make as `(startX, startY), (endX, endY)`, where the bottom left corner is `(0, 0)` and the top left corner is `(7, 7)`. Red goes first, and white goes second. Also, don't input it with the wrong format or the game will crash :)

#Building for Linux
- Clone into this repository
`git clone https://github.com/theultraman20/tui-checkers`
- Run make
`make`

#Issues
- The biggest issue with this program is that it uses an array of pointers (to pieces) to represent the board. I thought I would give it a try in hopes of making my code lighter, faster, and more readable, but unfortunately I ended up with the opposite. This had the consequence of making each function that has to manipulate the board itself require the pieces position as an argument, as you need to klnow the position of the piece in order to remove or move it on the board. This makes the code much more tedious and confusing then it should be. 

- The code for checking if a move is valid keeps track of the jumps needed to create that move and stores it on a jump stack inside the checkerboard object, for the piece moving function to use to remove jumped pieces. This means that a part of the code that is supposed to check if a move is possible actually modifys the object itself, and performs more then it's intended function. 

#Todo
- Handle a misformatted game move input properly
- Input moves using the arrow keys to select and move a piece instead of asking for coordinate input
- A better TUI, and options for ASCII or Unicode output
- Multiplayer (won't happen lol)
