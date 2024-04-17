This is the code for:

Sudoku Tic-Tac-Toe Calculator & Simulator 3.5 English Version
Copyright by RandomGuy1520

Game Rules:

Behold are the rules of Sudoku Tic-Tac-Toe. It is Sudoku since it uses a Sudoku board. It is also Tic-Tac-Toe since its score-counting is tic-tac-toe rules.
At first, there will be a 9 * 9 blank board. The blank board consists of 9 (3 * 3) grids, and each grid consists of 9 squares. Remember, a grid is nine squares.
Player X and player O take turns placing 'X's and 'O's in one of the squares on the board. Player X goes first. The first move must be played in the center grid.
After the first move, we look at the square's position in the grid that the last player played, including the square.
Whatever position is the square in the larger grid, the move playing now must be in the grid with the same position as compared to the whole board.
For example, if the n-th move is played in the center of the board, then the n+1 move must be played in 9 of the center grid.
For another example, if the n-th move is played in the right-top corner square of any grid, then the n+1 move must be played in the right-top grid of the board.
Banned squares: If a grid is full (every 9 of its squares is placed by X or O or is banned), then every square leading to this grid shall be banned.
Any player cannot place banned squares at any time later. For example, if the center square is full, then every center of other grids will be banned.
When the game ends, we count all tic-tac-toe scores (every 3-in-a-row) in every grid. Note that 3-in-a-rows across grids do not count. The player with more scores wins.

More information:

It is a variant of tic-tac-toe, or more precisely, a variant of a tic-tac-toe variant named Ultimate Tic-Tac-Toe.
The only difference between these two games is that when counting scores, Ultimate Tic-Tac-Toe only focuses on grids won, while Sudoku Tic-Tac-Toe focuses on all tic-tac-toe scores in every grid.

In my code, you can:

1. Change modes, from Player vs Player to Computer vs Computer, and also Player vs Computer and Computer vs Player.
2. Change computer evaluation & search depth, from 1 to 12.
3. Play Sudoku Tic-Tac-Toe freely!

To use the code in my directory, you can clone it, or just copy the code.
