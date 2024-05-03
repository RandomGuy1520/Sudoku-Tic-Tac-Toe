#pragma once
#include "Includes.h"

void WaitForScreenClose()
{
	std::cout << std::endl << "Press Enter to Close Screen:" << std::endl;
	if (version == CHI) std::cout << "Çë°´ Enter ¼ü¹Ø±ÕÆÁÄ»£º" << std::endl;
	std::cin.get(); std::cin.get();
}

signed main()
{
#ifdef USING_NORMAL_BOARD
	assert(MAXK == 3, "Board Size Error");
#endif
	int start_time = clock();
	STTT::greet_player();
	STTT::init();
	STTT::game(STTT::Moves::auto_move);
	STTT::print_result(start_time);
	WaitForScreenClose();
}
