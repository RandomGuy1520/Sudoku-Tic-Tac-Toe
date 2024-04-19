#include "Includes.h"

signed main()
{
#ifdef USING_NORMAL_BOARD
	assert(MAXK == 3, "Board Size Error");
#endif
	int start_time = clock();
	SudokuTicTacToe::greet_player();
	SudokuTicTacToe::init();
	SudokuTicTacToe::game(SudokuTicTacToe::Moves::auto_move);
	SudokuTicTacToe::print_result(start_time);
}
