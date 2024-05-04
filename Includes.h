// This is the includes.h file that includes everything basic.

#pragma once
#define NOMINMAX
#include <unordered_map>
#include <windows.h>
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <string>
#include <time.h>
#include <queue>
#include <tuple>

#include "config.h"

#define assert(x, y) if (!(x)) { throw(y); }

static status memo[MAXN][MAXN]; // please change memo only using the change_status function of class STTT.
static std::mt19937_64 rng(time(0) & 0x114514 | 0x1919810 ^ 5418);
static int blank[MAXN], abcnt = 0;
static const int center = (MAXK % 2 == 1 ? MAXN / 2 : MAXN / 2 - MAXK / 2 - 1);

#ifdef USING_NORMAL_BOARD
static const std::vector<std::tuple<int, int, int>> tttlines = { std::make_tuple(0, 1, 2), std::make_tuple(6, 7, 8), std::make_tuple(0, 3, 6), std::make_tuple(2, 5, 8), std::make_tuple(0, 4, 8), std::make_tuple(1, 4, 7), std::make_tuple(2, 4, 6), std::make_tuple(3, 4, 5) };
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
		status get_status() const { return SudokuTicTacToe::get_status(grid, num); }
		void change_status(status st) { SudokuTicTacToe::change_status(grid, num, st); }
		point get_rev() { return point(num, grid); }
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
	static inline status get_status(int grid, int num)
	{
		return memo[grid][num];
	}
	static inline void change_status(int grid, int num, status st)
	{
		int tot = 0;
		if (memo[grid][num] == BLANK) tot--;
		if (st == BLANK) tot++;
		memo[grid][num] = st;
		blank[grid] += tot;
	}
	static inline std::string tostr(double n)
	{
		std::string res, res2;
		int k = (int)n; n -= k;
		while (k != 0) res += (k % 10 + '0'), k /= 10;
		std::reverse(res.begin(), res.end());
		while (n != 0) n *= 10, res2 += ((int)n + '0'), n = n - (int)n;
		return res + '.' + res2;
	}
	static inline char status_to_char(status st)
	{
		if (st == X) return 'X';
		if (st == O) return 'O';
		if (st == BLANK) return ' ';
		return '*';
	}
	static inline bool check_grid(int grid)
	{
		return blank[grid] != 0;
	}
	static inline point get_double(int grid)
	{
		return point(grid, grid);
	}
	static inline std::string get_board_fen()
	{
		std::string res;
		for (int i = 0; i < MAXN; i++)
		{
			for (int j = 0; j < MAXN; j++)
			{
				res += (get_status(i, j) + '0');
			}
		}
		return res;
	}
	static inline size_t get_board_hash()
	{
		return std::hash<std::string>{}(get_board_fen());
	}
	static inline int get_depth()
	{
		if (USING_CONSTANT_TIME)
		{
			// More scientific way of calculating the right depth
			double tot = 0, cnt = 0;
			for (int i = 0; i < MAXN; i++) tot += blank[i], cnt += blank[i] * blank[i];
			if (tot <= 1 || cnt == tot) return 1;
			return std::min((int)tot, (int)round((log(10) * 13 + log(MAXSEC)) / log(cnt / tot)) - 4);
		}
		else return MAXDEPTH;
	}
	static inline std::vector<int> get_grid_point_in_order(bool is_x, int grid_num)
	{
		std::vector<int> ans1, ans2, ord;
		int mid = center; ord.push_back(mid);
		for (int i = 0; i < MAXN; i++) if (i != mid) ord.push_back(i);
		status st = is_x ? X : O;
		for (int i : ord)
		{
			if (get_status(grid_num, i) != BLANK || !check_grid(i)) continue;
			bool marked = false;
			for (auto& j : tttlines_with_num[i])
			{
#ifdef USING_NORMAL_BOARD
				if (get_status(grid_num, j.first) == st && get_status(grid_num, j.second) == st)
				{
					ans1.push_back(i);
					marked = true;
					break;
				}
#endif
#ifdef USING_LARGER_BOARD
				bool t = true;
				for (auto& k : j)
					if (get_status(grid_num, k) != st)
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
				int a = get_status(i, std::get<0>(j)), b = get_status(i, std::get<1>(j)), c = get_status(i, std::get<2>(j));
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
					if (get_status(i, k) == BANNED || get_status(i, k) == notst)
					{
						tot = 0;
						break;
					}
					tot += (get_status(i, k) == st);
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
				cnt += (get_status(i, std::get<0>(j)) == st && get_status(i, std::get<1>(j)) == st && get_status(i, std::get<2>(j)) == st);
#endif
#ifdef USING_LARGER_BOARD
			{
				bool t = true;
				for (auto& k : j)
					if (get_status(i, k) != st)
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
		for (int i = 0; i < MAXN; i++) blank[i] = MAXN;
#ifdef USING_NORMAL_BOARD
		for (int i = 0; i < MAXN; i++)
			for (auto& j : tttlines)
				if (std::get<0>(j) == i || std::get<1>(j) == i || std::get<2>(j) == i)
				{
					if (std::get<0>(j) == i) tttlines_with_num[i].push_back(std::make_pair(std::get<1>(j), std::get<2>(j)));
					if (std::get<1>(j) == i) tttlines_with_num[i].push_back(std::make_pair(std::get<0>(j), std::get<2>(j)));
					if (std::get<2>(j) == i) tttlines_with_num[i].push_back(std::make_pair(std::get<0>(j), std::get<1>(j)));
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
	static inline void print_board(point last_move = point(-1, -1))
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
				if (last_move.grid == p.grid && last_move.num == p.num)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
					printf("%c", status_to_char(p.get_status()));
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				}
				else printf("%c", status_to_char(p.get_status()));
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
		init();
		std::string aa[MAXN] = {
			"         ",
			"XX XX XX ",
			" X  X  X ",
			"OOOOOO   ",
			"X*OOXOXX ",
			"OOOOOO X ",
			"   OOO   ",
			"XX *XOXX ",
			" * OXO X "
		};
		for (int i = 0; i < MAXN; i++) for (int j = 0; j < MAXN; j++)
		{
			int x = coord(i, j).to_point().grid, y = coord(i, j).to_point().num;
			if (aa[i][j] == 'O') memo[x][y] = O;
			if (aa[i][j] == 'X') memo[x][y] = X;
			if (aa[i][j] == '*') memo[x][y] = BANNED;
			if (aa[i][j] == ' ') memo[x][y] = BLANK;
		}
		for (int i = 0; i < MAXN; i++)
		{
			int cnt = 0;
			for (int j = 0; j < MAXN; j++)
			{
				if (memo[i][j] == BLANK) cnt++;
			}
			blank[i] = cnt;
		}
		print_board();
	}
	static inline std::pair<int, double> abdfs(int depth, bool is_x, int grid, double alpha = -uninitialized, double beta = uninitialized)
	{
		std::vector<int> equals;
		abcnt++;
		double uninit = is_x ? -uninitialized : uninitialized;
		double best_diff = uninit;
		int best_move;
		for (int& i : get_grid_point_in_order(is_x, grid))
		{
			change_status(grid, i, is_x ? X : O);
			std::vector<point> changed; std::queue<int> q;
			if (!check_grid(grid)) q.push(grid);
			if (blank[4] < 0) __debugbreak();
			while (!q.empty())
			{
				int _grid = q.front(); q.pop();
				for (int j = 0; j < MAXN; j++)
					if (j != _grid && get_status(j, _grid) == BLANK)
					{
						change_status(j, _grid, BANNED);
						changed.push_back(point(j, _grid));
						if (!check_grid(j))
							q.push(j);
					}
			}
			double diff = (depth == 1 ? (USING_HEURISTICS ? get_heuristic_point_diff() : get_point_diff()) : abdfs(depth - 1, !is_x, i, alpha, beta).second);
			for (point& j : changed)
				j.change_status(BLANK);
			change_status(grid, i, BLANK);
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
		if (std::count(equals.begin(), equals.end(), center) > 0) best_move = center;
		else {
			best_move = equals[0];
			best_move = equals[rng() % (int)equals.size()];
		}
		return std::make_pair(best_move, best_diff);
	}
	static inline std::pair<int, double> tgabdfs(int grid)
	{
		double best_heur = uninitialized; int best_move;
		for (int i = 0; i < MAXN; i++)
		{
			if (get_status(grid, i) != BLANK) continue;
			change_status(grid, i, O);
			change_status(i, grid, X);
		}
		for (int i = 0; i < MAXN; i++)
		{
			if (get_status(grid, i) != O) continue;
			change_status(i, grid, BANNED);
			double heur = abdfs(get_depth() - 1, true, i).second;
			if (heur < best_heur) best_move = i, best_heur = heur;
			change_status(i, grid, X);
		}
		change_status(grid, best_move, BLANK);
		change_status(best_move, grid, BLANK);
		return std::make_pair(best_move, best_heur);
	}
	static inline void game(std::pair<int, double>(*move_func)(bool, int), int first_grid = center)
	{
		int move_num = 0;
		int now_grid = first_grid;
		bool is_x = true;
		while (check_grid(now_grid))
		{
			move_num++;
			std::pair<int, double> ret = move_func(is_x, now_grid);
			int move = ret.first;
			change_status(now_grid, move, is_x ? X : O);
			if (!check_grid(now_grid))
				dfs_grid(now_grid);
			point(now_grid, move).print();
			print_board(point(now_grid, move));
			now_grid = move;
			is_x = !is_x;
			std::cout << count_point(true) << " : " << count_point(false) << "+" + tostr(komi) << std::endl << std::endl;
			if (PRINT_EVAL && ret.second != uninitialized) std::cout << "Computer Eval: " << ret.second - komi << std::endl;
			std::cout << "Computer Depth: " << get_depth() << std::endl << std::endl;
		}
		double diff = get_point_diff() - komi;
		if (diff > 0) std::cout << "X Wins!!!!" << std::endl;
		else if (diff < 0) std::cout << "O Wins!!!!" << std::endl;
		else std::cout << "What a TIE!!!!" << std::endl;
		std::cout << "Game ended in " << move_num << " moves.\n";
	}
	static inline void dfs_grid(int grid)
	{
		for (int i = 0; i < MAXN; i++)
			if (i != grid && get_status(i, grid) == BLANK)
			{
				change_status(i, grid, BANNED);
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
		std::cout << "Which mode do you want to play in? Type CVSC, CVSP, PVSC, PVSP, or TVSC below.\n";
		if (version == CHI) std::cout << "你想要玩什么模式？请输入 CVSC, CVSP, PVSC, PVSP, 或者 TVSC。P代表人, C代表机, T代表反身策略。\n";
		std::cin >> input;
		while (input != "CVSC" && input != "CVSP" && input != "PVSC" && input != "PVSP" && input != "TVSC")
		{
			std::cout << "Input error! Please input CVSC, CVSP, PVSC, PVSP, or TVSC again below.\n";
			if (version == CHI) std::cout << "输入错误！请再次输入 CVSC, CVSP, PVSC, PVSP, 或者 TVSC。\n";
			std::cin >> input;
		}
		if (input == "CVSC") playing_mode = CVSC;
		else if (input == "CVSP") playing_mode = CVSP;
		else if (input == "PVSC") playing_mode = PVSC;
		else if (input == "PVSP") playing_mode = PVSP;
		else if (input == "TVSC") playing_mode = TVSC;
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
		static inline int tiger_move(int now_grid)
		{
			long long t = clock();
			if (get_status(now_grid, center) == BLANK)
			{
#ifndef TESTING
				Sleep(std::max(0ll, 1000 + t - clock()));
#endif
				return center;
			}
			bool isbanned = false, full = true;
			for (int i = 0; i < MAXN; i++)
				if (get_status(now_grid, i) == BANNED)
				{
					isbanned = true;
					break;
				}
			if (isbanned && get_status(now_grid, now_grid) == BLANK)
			{
#ifndef TESTING
				Sleep(std::max(0ll, 1000 + t - clock()));
#endif
				return now_grid;
			}
			for (int i = 0; i < MAXN; i++)
			{
				isbanned = false; full = true;
				for (int j = 0; j < MAXN; j++)
				{
					if (get_status(i, j) == BANNED) isbanned = true;
					if (get_status(i, j) == BLANK) full = false;
				}
				if (isbanned && !full)
				{
#ifndef TESTING
					Sleep(std::max(0ll, 1000 + t - clock()));
#endif
					return i;
				}
			}
#ifndef TESTING
			Sleep(std::max(0ll, 1000 + t - clock()));
#endif
			return 0;
		}
		static inline std::pair<int, double> computer_move(bool is_x, int now_grid)
		{
			int t = clock(); std::pair<int, double> ret;
#ifdef TESTING
			if (playing_mode == TVSC) ret = tgabdfs(now_grid);
			else ret = abdfs(get_depth(), is_x, now_grid);
#else
			ret = abdfs(get_depth(), is_x, now_grid);
			Sleep(std::max(0ll, 1000ll + t - clock()));
#endif
			return ret;
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
		static inline std::pair<int, double> tvsc_move(bool is_x, int now_grid)
		{
			if (is_x) return std::make_pair(tiger_move(now_grid), uninitialized);
			else return computer_move(is_x, now_grid);
		}
		static inline std::pair<int, double> auto_move(bool is_x, int now_grid)
		{
			if (playing_mode == CVSC) return cvsc_move(is_x, now_grid);
			if (playing_mode == CVSP) return cvsp_move(is_x, now_grid);
			if (playing_mode == PVSC) return pvsc_move(is_x, now_grid);
			if (playing_mode == PVSP) return pvsp_move(is_x, now_grid);
			if (playing_mode == TVSC) return tvsc_move(is_x, now_grid);
		}
	};
};

#define STTT SudokuTicTacToe
