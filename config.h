// This is the config file, where you can tweak around the parameters, constants, modes, and versions.

#pragma once

#define USING_NORMAL_BOARD
#define assert(x, y) if (!(x)) { throw(y); }

const enum status { BLANK, X, O, BANNED };
const enum mode { CVSC, CVSP, PVSC, PVSP, GET_FROM_USER_INPUT };
const enum lang { ENG, CHI };

const int MAXN = 9, MAXK = 3, MAXDEPTH = 10; // MAXDEPTH is changeable to control the depth of computer calculation of the board.
const bool PRINT_EVAL = true, USING_HEURISTICS = true;
const double uninitialized = 1e9;
const lang version = CHI;
mode playing_mode = GET_FROM_USER_INPUT; // Playing mode is changeable

#ifdef USING_NORMAL_BOARD
const double heur1 = 1.0 / 6.5, heur2 = 1.0 / 2;
#endif
