// This is the config file, where you can tweak around the parameters, constants, modes, and versions.

#pragma once

#define USING_NORMAL_BOARD // USING_NORMAL_BOARD or USING_LARGER_BOARD
#define TESTING // if not defined testing, then it is for playing purposes.

const enum status { BLANK, X, O, BANNED };
const enum mode { CVSC, CVSP, PVSC, PVSP, TVSC, GET_FROM_USER_INPUT };
const enum lang { ENG, CHI };

const int MAXK = 3, MAXN = MAXK * MAXK;
const bool PRINT_EVAL = true, USING_HEURISTICS = true, USING_CONSTANT_TIME = true;
const double uninitialized = 1e9, MAXSEC = 0.5; // Uninitialized is not necessary to change
const lang version = CHI; // Language version
static mode playing_mode = GET_FROM_USER_INPUT; // Playing mode is changeable
const int depths[10] = { 0, 0, 16, 11, 7, 6, 4, 3 };
const int MAXDEPTH = depths[MAXK];

#ifdef USING_NORMAL_BOARD
const double heur1 = 1.0 / 6, heur2 = 1.0 / 2, komi = 2; // Initial heuristics for 9 * 9 board (best by test).
#endif

#ifdef USING_LARGER_BOARD
const double komi = 0;
#endif
