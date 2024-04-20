// This is the includes.h file that includes everything basic.

#pragma once
#define NOMINMAX
#include <unordered_map>
#include <windows.h>
#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <string>
#include <time.h>
#include <tuple>

#include "config.h"

#define assert(x, y) if (!(x)) { throw(y); }

static status memo[MAXN][MAXN];
static std::mt19937_64 rng(time(0) & 0x114514 | 0x1919810 ^ 5418);
static int abcnt = 0;

#ifdef USING_NORMAL_BOARD
static const std::vector<std::tuple<int, int, int>> tttlines = { {0, 1, 2}, {6, 7, 8}, {0, 3, 6}, {2, 5, 8}, {0, 4, 8}, {1, 4, 7}, {2, 4, 6}, {3, 4, 5} };
static std::vector<std::pair<int, int>> tttlines_with_num[10];
#endif

#ifdef USING_LARGER_BOARD
static std::vector<std::vector<int>> tttlines;
static std::vector<std::vector<int>> tttlines_with_num[MAXN];
static double heur[MAXK + 1];
#endif

class SudokuTicTacToe
{
public:
	static const int center = (MAXK % 2 == 1 ? MAXN / 2 : MAXN / 2 - MAXK / 2 - 1);
	struct point
	{
		int grid, num;
		point(int grid, int num) : grid(grid), num(num) {}
		point() : grid(-1), num(-1) {}
		void print() const
		{
			int m_x = grid / MAXK, m_y = grid % MAXK, g_x = num / MAXK, g_y = num % MAXK;
			std::cout << MAXK * m_x + g_x + 1 << " " << MAXK * m_y + g_y + 1 << std::endl;
		}
		status get_status() const { return memo[grid][num]; }
		void change_status(status st) { memo[grid][num] = st; }
	};
	struct coord
	{
		int x, y;
		coord(int x, int y) : x(x), y(y) {}
		coord() : x(-1), y(-1) {}
		void print() const { std::cout << x + 1 << " " << y + 1 << std::endl; }
#ifdef USING_NORMAL_BOARD
		point to_point() { int m_x = x / MAXK, m_y = y / MAXK, g_x = x % MAXK, g_y = y % MAXK; return point(MAXK * m_x + m_y, MAXK * g_x + g_y); }
#endif
#ifdef USING_LARGER_BOARD
		point to_point() const { int m_x = x / MAXK, m_y = y / MAXK, g_x = x % MAXK, g_y = y % MAXK; return point(MAXK * m_x + m_y, MAXK * g_x + g_y); }
#endif
	};
	static inline char status_to_char(status st)
	{
		if (st == X) return 'X';
		if (st == O) return 'O';
		if (st == BLANK) return ' ';
		return '*';
	}
	static inline bool check_grid(int grid)
	{
		for (int i = 0; i < MAXN; i++)
		{
			if (memo[grid][i] == BLANK) return true;
		}
		return false;
	}
	static inline std::string get_board_fen()
	{
		std::string res;
		for (int i = 0; i < MAXN; i++)
		{
			for (int j = 0; j < MAXN; j++)
			{
				res += (memo[i][j] + '0');
			}
		}
		return res;
	}
	static inline size_t get_board_hash()
	{
		return std::hash<std::string>{}(get_board_fen());
	}
	static inline std::vector<int> get_grid_point_in_order(bool is_x, int grid_num)
	{
		std::vector<int> ans1, ans2, ord;
		int mid = center; ord.push_back(mid);
		for (int i = 0; i < MAXN; i++) if (i != mid) ord.push_back(i);
		status st = is_x ? X : O;
		for (int i : ord)
		{
			bool marked = false;
			for (auto& j : tttlines_with_num[i])
			{
#ifdef USING_NORMAL_BOARD
				if (memo[grid_num][j.first] == st && memo[grid_num][j.second] == st)
				{
					ans1.push_back(i);
					marked = true;
					break;
				}
#endif

#ifdef USING_LARGER_BOARD
				bool t = true;
				for (auto& k : j)
					if (memo[grid_num][k] != st)
					{
						t = false;
						break;
					}
				if (t)
				{
					ans1.push_back(i);
					marked = true;
					break;
				}
#endif
			}
			if (!marked) ans2.push_back(i);
		}
		for (int i : ans2) ans1.push_back(i);
		return ans1;
	}
	static inline double heuristic_count_point(bool is_x)
	{
		double cnt = 0;
		status st = is_x ? X : O, notst = is_x ? O : X;
		for (int i = 0; i < MAXN; i++)
		{
			for (auto& j : tttlines)
			{
#ifdef USING_NORMAL_BOARD
				int a = memo[i][get<0>(j)], b = memo[i][get<1>(j)], c = memo[i][get<2>(j)];
				int k = ((a == st) + (b == st) + (c == st));
				if (a == BANNED || b == BANNED || c == BANNED || a == notst || b == notst || c == notst) k = 0;
				if (k == 1) cnt += heur1;
				else if (k == 2) cnt += heur2;
				else if (k == 3) cnt += 1;
#endif
#ifdef USING_LARGER_BOARD
				int tot = 0;
				for (auto& k : j)
				{
					if (memo[i][k] == BANNED || memo[i][k] == notst)
					{
						tot = 0;
						break;
					}
					tot += (memo[i][k] == st);
				}
				cnt += heur[tot];
#endif
			}
		}
		return cnt;
	}
	static inline double get_heuristic_point_diff()
	{
		return heuristic_count_point(true) - heuristic_count_point(false);
	}
	static inline int count_point(bool is_x)
	{
		int cnt = 0;
		status st = is_x ? X : O;
		for (int i = 0; i < MAXN; i++)
		{
			for (auto& j : tttlines)
#ifdef USING_NORMAL_BOARD
				cnt += (memo[i][get<0>(j)] == st && memo[i][get<1>(j)] == st && memo[i][get<2>(j)] == st);
#endif
#ifdef USING_LARGER_BOARD
			{
				bool t = true;
				for (auto& k : j)
					if (memo[i][k] != st)
					{
						t = false;
						break;
					}
				cnt += t;
			}
#endif
		}
		return cnt;
	}
	static inline int get_point_diff()
	{
		return count_point(true) - count_point(false);
	}
	static inline void init()
	{
		for (int i = 0; i < MAXN; i++)
			for (int j = 0; j < MAXN; j++)
				memo[i][j] = BLANK;
#ifdef USING_NORMAL_BOARD
		for (int i = 0; i < MAXN; i++)
			for (auto& j : tttlines)
				if (get<0>(j) == i || get<1>(j) == i || get<2>(j) == i)
				{
					if (get<0>(j) == i) tttlines_with_num[i].push_back(std::make_pair(get<1>(j), get<2>(j)));
					if (get<1>(j) == i) tttlines_with_num[i].push_back(std::make_pair(get<0>(j), get<2>(j)));
					if (get<2>(j) == i) tttlines_with_num[i].push_back(std::make_pair(get<0>(j), get<1>(j)));
				}
#endif
#ifdef USING_LARGER_BOARD
		heur[MAXK] = 1.0;
		for (int i = MAXK - 1; i > 0; i--)
			heur[i] = heur[i + 1] / (MAXK - i + 1.1);
		for (int i = 0; i < MAXK; i++)
		{
			std::vector<int> vec1, vec2;
			for (int j = 0; j < MAXK; j++)
			{
				vec1.push_back(i + j * MAXK);
				vec2.push_back(i * MAXK + j);
			}
			tttlines.push_back(vec1);
			tttlines.push_back(vec2);
		}
		{
			std::vector<int> vec1, vec2;
			for (int i = 0; i < MAXK; i++) vec1.push_back(i * (MAXK + 1));
			for (int i = 1; i <= MAXK; i++) vec2.push_back(i * (MAXK - 1));
			tttlines.push_back(vec1); tttlines.push_back(vec2);
		}
		for (int i = 0; i < MAXN; i++)
			for (auto j : tttlines)
				for (int k : j)
					if (k == i)
					{
						std::vector<int> vec;
						for (int l : j) if (l != k) vec.push_back(l);
						tttlines_with_num[i].push_back(vec);
					}
#endif
	}
	static inline void print_board()
	{
		printf("\n  ");
		for (int i = 1; i <= MAXN; i++) printf("%d ", i % 10);
		printf("\n");
		for (int i = 0; i < MAXN; i++)
		{
			printf("%d ", (i + 1) % 10);
			for (int j = 0; j < MAXN; j++)
			{
				coord c(i, j); point p = c.to_point();
				printf("%c", status_to_char(memo[p.grid][p.num]));
				if (j % MAXK == MAXK - 1) printf("|");
				else printf(" ");
			}
			printf("\n");
			if (i % MAXK == MAXK - 1)
			{
				printf("  ");
				for (int j = 0; j < MAXN; j++) printf("- ");
				printf("\n");
			}
		}
		printf("\n");
	}
	static inline void __debuginit()
	{
		std::string aa[MAXN] = {
			"XXOXXXOOX",
			"OXOO*OOX*",
			"O XOOOOXO",
			"X*XOOOOOO",
			"XXOOXO*XX",
			"XXOOOOOXX",
			"OXXXX XXX",
			"OXXOXXXXX",
			"XXOOOOXOO"
		};
		for (int i = 0; i < MAXN; i++) for (int j = 0; j < MAXN; j++) {
			int x = coord(i, j).to_point().grid, y = coord(i, j).to_point().num;
			if (aa[i][j] == 'O') memo[x][y] = O;
			if (aa[i][j] == 'X') memo[x][y] = X;
			if (aa[i][j] == '*') memo[x][y] = BANNED;
			if (aa[i][j] == ' ') memo[x][y] = BLANK;
		}
		print_board();
	}
	static inline std::pair<int, double> abdfs(int depth, bool is_x, int grid, double alpha = -1e9, double beta = 1e9)
	{
		std::vector<int> equals;
		abcnt++;
		double uninit = is_x ? -uninitialized : uninitialized;
		double best_diff = uninit;
		int best_move;
		for (int& i : get_grid_point_in_order(is_x, grid))
		{
			if (memo[grid][i] != BLANK || !check_grid(i)) continue;
			memo[grid][i] = (is_x ? X : O);
			double diff = (depth == 1 ? (USING_HEURISTICS ? get_heuristic_point_diff() : get_point_diff()) : abdfs(depth - 1, !is_x, i, alpha, beta).second);
			memo[grid][i] = BLANK;
			if (is_x && diff >= best_diff)
			{
				if (diff > best_diff)
				{
					equals.clear();
					best_diff = diff;
					if (best_diff > beta)
						return std::make_pair(i, best_diff);
					alpha = std::max(alpha, best_diff);
				}
				equals.push_back(i);
			}
			if (!is_x && diff <= best_diff)
			{
				if (diff < best_diff)
				{
					equals.clear();
					best_diff = diff;
					if (best_diff < alpha)
						return std::make_pair(i, best_diff);
					beta = std::min(beta, best_diff);
				}
				equals.push_back(i);
			}
		}
		if (best_diff == uninit) return std::make_pair(-1, (double)get_point_diff());
		if (count(equals.begin(), equals.end(), center) > 0) best_move = center;
		else {
			best_move = equals[0];
			best_move = equals[rng() % (int)equals.size()];
		}
		return std::make_pair(best_move, best_diff);
	}
	static inline void game(std::pair<int, double>(*move_func)(bool, int))
	{
		int move_num = 0;
		int now_grid = center;
		bool is_x = true;
		while (check_grid(now_grid))
		{
			move_num++;
			std::pair<int, double> ret = move_func(is_x, now_grid);
			int move = ret.first;
			memo[now_grid][move] = (is_x ? X : O);
			if (!check_grid(now_grid))
				dfs_grid(now_grid);
			point(now_grid, move).print();
			now_grid = move;
			is_x = !is_x;
			print_board();
			std::cout << count_point(true) << " : " << count_point(false) << std::endl << std::endl;
			if (PRINT_EVAL && ret.second != uninitialized) std::cout << "Computer Eval: " << ret.second << std::endl << std::endl;
		}
		int diff = get_point_diff();
		if (diff > 0) std::cout << "X Wins!!!!" << std::endl;
		else if (diff < 0) std::cout << "O Wins!!!!" << std::endl;
		else std::cout << "What a TIE!!!!" << std::endl;
		std::cout << "Game ended in " << move_num << " moves.\n";
	}
	static inline void dfs_grid(int grid)
	{
		for (int i = 0; i < MAXN; i++)
			if (i != grid && memo[i][grid] == BLANK)
			{
				memo[i][grid] = BANNED;
				if (!check_grid(i))
					dfs_grid(i);
			}
	}
	static inline void print_result(int start_time)
	{
		printf("Finished calculating with depth %d, used %d ms time.\n", MAXDEPTH, clock() - start_time);
		std::cout << "Total Calculation Size: " << abcnt << " " << std::endl;
	}
	static inline void get_playing_mode()
	{
		if (playing_mode != GET_FROM_USER_INPUT) return;
		std::string input;
		std::cout << "Which mode do you want to play in? Type CVSC, CVSP, PVSC, or PVSP below.\n";
		if (version == CHI) std::cout << "你想要玩什么模式？请输入 CVSC, CVSP, PVSC, 或者 PVSP。P代表人，C代表机。\n";
		std::cin >> input;
		while (input != "CVSC" && input != "CVSP" && input != "PVSC" && input != "PVSP")
		{
			std::cout << "Input error! Please input CVSC, CVSP, PVSC, or PVSP again below.\n";
			if (version == CHI) std::cout << "输入错误！请再次输入 CVSC, CVSP, PVSC, 或者 PVSP。\n";
			std::cin >> input;
		}
		if (input == "CVSC") playing_mode = CVSC;
		else if (input == "CVSP") playing_mode = CVSP;
		else if (input == "PVSC") playing_mode = PVSC;
		else if (input == "PVSP") playing_mode = PVSP;
	}
	static inline void greet_player()
	{
		std::cout << "Welcome to Sudoku - Tic Tac Toe!\n"; if (version == CHI) std::cout << "欢迎来玩数独井字棋！\n"; std::cout << "\n"; Sleep(1000);
		std::cout << "I think you have already understood the rules of this game.\n"; if (version == CHI) std::cout << "你应该已经懂得怎么玩这玩意儿了。\n"; std::cout << "\n"; Sleep(1000);
		std::cout << "When you enter the coordinates, please enter the vertical one first and then the horizontal one.\n"; if (version == CHI) std::cout << "输入坐标时，先输入竖着的，再输横着的。\n"; std::cout << "\n"; Sleep(1000);
		std::cout << "The coordinates is as follows:\n"; if (version == CHI) std::cout << "坐标如下：\n"; std::cout << "\n";
		std::cout << "  ";
		for (int i = 1; i <= MAXN; i++) std::cout << i % 10 << " ";
		std::cout << std::endl;
		for (int i = 1; i <= MAXN; i++)
		{
			std::cout << i % 10 << " ";
			for (int j = 1; j <= MAXN; j++)
			{
				std::cout << "X";
				if (j % (int)sqrt(MAXN) == 0) std::cout << "|";
				else std::cout << " ";
			}
			std::cout << std::endl;
			if (i % MAXK == 0)
			{
				std::cout << " ";
				for (int j = 1; j <= MAXN; j++)
					std::cout << " -";
				std::cout << '\n';
			}
		}
		Sleep(2000);
		std::cout << std::endl;
#ifdef USING_LARGER_BOARD
		std::cout << "The coordinates appearing on the screen have been modded by 10. But when you type them, please type the original coordinates.\n";
		if (version == CHI) std::cout << "显示的坐标都被做了模10运算，但当你输入坐标时，请输入原来的坐标。\n";
		std::cout << std::endl;
		Sleep(1000);
#endif
		get_playing_mode();
		std::cout << std::endl;
		if (playing_mode == CVSC) { std::cout << "You're playing in computer mode.\n"; if (version == CHI) std::cout << "现在是电脑对战模式。\n"; }
		else if (playing_mode == CVSP) { std::cout << "Computer goes first and you go second.\n"; if (version == CHI) std::cout << "电脑先，你后。\n"; }
		else if (playing_mode == PVSC) { std::cout << "You go first and computer goes second.\n"; if (version == CHI) std::cout << "你先，电脑后。\n"; }
		else if (playing_mode == PVSP) { std::cout << "You're playing in two-player mode.\n"; if (version == CHI) std::cout << "现在是双人模式。\n"; }
		std::cout << "\nLet's start!\n"; if (version == CHI) std::cout << "我们开始吧！\n"; std::cout << "\n"; Sleep(1000);
	}
	class Moves
	{
	public:
		static inline int human_move(int now_grid)
		{
			int x, y;
			std::cout << "Your Move:\n\nEnter coordinates:\n"; if (version == CHI) std::cout << "输入坐标：\n";
			std::cin >> x >> y;
			coord c(x - 1, y - 1); point p = c.to_point();
			while (p.grid != now_grid || p.get_status() != BLANK)
			{
				if (std::cin.fail()) std::cin.clear(), std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\nInvalid input! Try again:\n"; if (version == CHI) std::cout << "输入错误！请重新输入：\n";
				std::cin >> x >> y;
				c.x = x - 1, c.y = y - 1;
				p = c.to_point();
			}
			return p.num;
		}
		static inline std::pair<int, double> computer_move(bool is_x, int now_grid)
		{
			return abdfs(MAXDEPTH, is_x, now_grid);
		}
		static inline std::pair<int, double> cvsc_move(bool is_x, int now_grid)
		{
			std::cout << "Computer's Move:\n\n";
			return computer_move(is_x, now_grid);
		}
		static inline std::pair<int, double> cvsp_move(bool is_x, int now_grid)
		{
			if (is_x) return std::cout << "Computer's Move:\n\n", abdfs(MAXDEPTH, is_x, now_grid);
			else return std::make_pair(human_move(now_grid), uninitialized);
		}
		static inline std::pair<int, double> pvsc_move(bool is_x, int now_grid)
		{
			if (!is_x) return std::cout << "Computer's Move:\n\n", abdfs(MAXDEPTH, is_x, now_grid);
			else return std::make_pair(human_move(now_grid), uninitialized);
		}
		static inline std::pair<int, double> pvsp_move(bool is_x, int now_grid)
		{
			return std::make_pair(human_move(now_grid), uninitialized);
		}
		static inline std::pair<int, double> auto_move(bool is_x, int now_grid)
		{
			if (playing_mode == CVSC) return cvsc_move(is_x, now_grid);
			if (playing_mode == CVSP) return cvsp_move(is_x, now_grid);
			if (playing_mode == PVSC) return pvsc_move(is_x, now_grid);
			if (playing_mode == PVSP) return pvsp_move(is_x, now_grid);
		}
	};
};
