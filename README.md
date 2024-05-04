Copyright © by RandomGuy1520

This is the code for:

Sudoku Tic-Tac-Toe Calculator & Simulator 5.2 Version

In short: STTTCS 5.2 ver

Game Rules:

Behold are the rules of Sudoku Tic-Tac-Toe. It is Sudoku since it uses a Sudoku board. It is also Tic-Tac-Toe since its score-counting uses tic-tac-toe rules.
At first, there will be an N * N blank board, usually N = 9. Let K be sqrt(N). The blank board consists of N (K * K) grids, and each grid consists of N squares.
Player X and player O take turns placing 'X's and 'O's in one of the squares on the board. Player X goes first. The first move is placed freely.
If K is even, the center grid will be the left-top of the four centers. After the first move, the player is forced to play in a certain grid indicated by the last move as follows:
The relative position of the next grid to the whole board is equal to the relative position of the previous square to the grid that includes the previous square.
For example, if the m-th move is played in the center of the board, then the m+1 move must be played in N of the center grid.
For another example, if the n-th move is played in the right-top corner square of any grid, then the n+1 move must be played in the right-top grid of the board.
Banned squares: If a grid is full (every N of its squares is placed by X or O or is banned), then every square leading to this grid shall be banned.
Any player cannot place banned squares at any time later. For example, if the center square is full, then every center of other grids will be banned.
When the game ends, we count all tic-tac-toe scores (every K-in-a-row) in every grid. Note that K-in-a-rows across grids do not count. The player with more scores wins.

More information:

It is a variant of tic-tac-toe, or more precisely, a variant of a tic-tac-toe variant named Ultimate Tic-Tac-Toe.

Note:

This program's time complexity is O(C * N ^ (D + 4)) in CVSC mode, where D is the search depth, and C is the alpha-beta pruning constant, which is around 1e-5 to 4e-5.
A common strategy of the game is called the Tiger Strategy, where you can figure it out yourselves about what it is by using Tiger Strategy vs Computer mode.

In my code, you can:

1. Change modes, including Player vs Player, Computer vs Computer, Player vs Computer, Computer vs Player, or Tiger Strategy vs Computer. You can also let the user input the mode.
2. Change computer evaluation & search depth, from 1 to 12.
3. Play Sudoku Tic-Tac-Toe freely!

To use the code, you can clone it, or just copy the code.
