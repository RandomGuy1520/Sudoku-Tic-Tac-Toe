// Sudoku Tic-Tac-Toe Calculator & Simulator 3.5
// Copyright by Regis Zhou ZJX

/*

Rules:

Behold are the rules of Sudoku Tic-Tac-Toe. It is Sudoku since it uses a Sudoku board. It is also Tic-Tac-Toe since its score-counting is tic-tac-toe rules.
At first, there will be a N * N blank board, where usually N = 9. Let K be sqrt(N). The blank board consists of N (K * K) grids, and each grid consists of N squares.
Player X and player O take turns placing 'X's and 'O's in one of the squares on the board. Player X goes first. The first move must be played in the center grid.
If K is even, then the center grid will be the left-top one. After the first move, we look at the position of the square the last player played in the grid that includes it.
Whatever position is the square in the larger grid, the move playing now must be in the grid with the same position as compared to the whole board.
For example, if the m-th move is played in the center of the board, then the m+1 move must be played in N of the center grid.
For another example, if the n-th move is played in the right-top corner square of any grid, then the n+1 move must be played in the right-top grid of the board.
Banned squares: If a grid is full (every N of its squares is placed by X or O or is banned), then every square leading to this grid shall be banned.
Banned squares cannot be placed by any player at any time later. For example, if the center square is full, then every center of other grids will be banned.
When the game ends, we count all tic-tac-toe scores (every K-in-a-row) in every grid. Note that K-in-a-rows across grids do not count. The player with more scores wins.

*/

// Note: This program's time complexity is O(C * 9 ^ (D + 4)) in CVSC mode, where D is the search depth, and C is the alpha-beta pruning constant, which is around 1e-5 to 4e-5.

// This is the config file, where you can tweak around the parameters, constants, modes, and versions.

#pragma once

#define USING_NORMAL_BOARD
#define assert(x, y) if (!(x)) { throw(y); }

const enum status { BLANK, X, O, BANNED };
const enum mode { CVSC, CVSP, PVSC, PVSP };
const enum lang { ENG, CHI };

const int MAXN = 9, MAXK = 3, MAXDEPTH = 10; // MAXDEPTH is changeable to control the depth of computer calculation of the board.
const bool PRINT_EVAL = true, USING_HEURISTICS = true;
const double uninitialized = 1e9;
const mode playing_mode = CVSC; // Playing mode is changeable
const lang version = CHI;

#ifdef USING_NORMAL_BOARD
const double heur1 = 1.0 / 6.5, heur2 = 1.0 / 2;
#endif
