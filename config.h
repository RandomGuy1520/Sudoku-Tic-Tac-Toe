// This is the config file, where you can tweak around the parameters, constants, modes, and versions.

#pragma once

#define USING_NORMAL_BOARD // USING_NORMAL_BOARD or USING_LARGER_BOARD

const enum status { BLANK, X, O, BANNED };
const enum mode { CVSC, CVSP, PVSC, PVSP, GET_FROM_USER_INPUT };
const enum lang { ENG, CHI };

const int MAXK = 3, MAXN = MAXK * MAXK, MAXSEC = 6;
const bool PRINT_EVAL = true, USING_HEURISTICS = true;
const double uninitialized = 1e9; // Not necessary to change
const lang version = CHI; // Language version
mode playing_mode = GET_FROM_USER_INPUT; // Playing mode is changeable
const int depths[10] = { 0, 0, 16, 11, 7, 6, 4, 3 };
const int MAXDEPTH = depths[MAXK];

#ifdef USING_NORMAL_BOARD
const double heur1 = 1.0 / 6, heur2 = 1.0 / 2; // Initial heuristics for 9 * 9 board (best by test).
#endif
