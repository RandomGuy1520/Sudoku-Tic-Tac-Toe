// Sudoku Tic-Tac-Toe Calculator & Simulator 3.5
// Copyright by Regis Zhou ZJX

/*
Behold are the rules of Sudoku Tic-Tac-Toe. It is Sudoku since it uses a Sudoku board. It is also Tic-Tac-Toe since its score-counting is tic-tac-toe rules.
At first, there will be a 9 * 9 blank board. The blank board consists of 9 (3 * 3) grids, and each grid consists of 9 squares. Remember, a grid is nine squares.
Player X and player O take turns placing 'X's and 'O's in one of the squares on the board. Player X goes first. The first move must be played in the center grid.
After the first move, we look at the position of the square the last player played in the grid that includes it.
Whatever position is the square in the larger grid, the move playing now must be in the grid with the same position as compared to the whole board.
For example, if the n-th move is played in the center of the board, then the n+1 move must be played in 9 of the center grid.
For another example, if the n-th move is played in the right-top corner square of any grid, then the n+1 move must be played in the right-top grid of the board.
Banned squares: If a grid is full (every 9 of its squares is placed by X or O or is banned), then every square leading to this grid shall be banned.
Banned squares cannot be placed by any player at any time later. For example, if the center square is full, then every center of other grids will be banned.
When the game ends, we count all tic-tac-toe scores (every 3-in-a-row) in every grid. Note that 3-in-a-rows across grids do not count. The player with more scores wins.
*/

#include <windows.h>
#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <time.h>
#include <tuple>
using namespace std;

enum status { BLANK, X, O, BANNED };
enum mode { CVSC, CVSP, PVSC, PVSP };

const vector<tuple<int, int, int>> tttlines = { {0, 1, 2}, {6, 7, 8}, {0, 3, 6}, {2, 5, 8}, {0, 4, 8}, {1, 4, 7}, {2, 4, 6}, {3, 4, 5} };
const int MAXN = 9, MAXDEPTH = 10; // MAXDEPTH is changeable to control the depth of computer calculation of the board.
const double heur1 = 1.0 / 7, heur2 = 1.0 / 2, uninitialized = 1e9;
const bool PRINT_EVAL = true, USING_HEURISTICS = true;
const mode playing_mode = PVSC; // Playing mode is changeable

vector<pair<int, int>> tttlines_with_num[10];
status memo[MAXN][MAXN];
mt19937_64 rng(time(0) & 0x114514 | 0x1919810 ^ 5418);
int abcnt = 0;

namespace SudokuTicTacToe
{
	struct point
	{
		int grid, num;
		point(int grid, int num) : grid(grid), num(num) {}
		point() : grid(-1), num(-1) {}
		void print() const
		{
			int m_x = grid / 3, m_y = grid % 3, g_x = num / 3, g_y = num % 3;
			cout << 3 * m_x + g_x + 1 << " " << 3 * m_y + g_y + 1 << endl;
		}
		status get_status() const { return memo[grid][num]; }
		void change_status(status st) { memo[grid][num] = st; }
	};
	struct coord
	{
		int x, y;
		coord(int x, int y) : x(x), y(y) {}
		coord() : x(-1), y(-1) {}
		point to_point() const { int m_x = x / 3, m_y = y / 3, g_x = x % 3, g_y = y % 3; return point(3 * m_x + m_y, 3 * g_x + g_y); }
		void print() const { cout << x + 1 << " " << y + 1 << endl; }
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
	static inline vector<int> get_grid_point_in_order(bool is_x, int grid_num)
	{
		vector<int> ans1, ans2, ord = { 4, 0, 1, 2, 3, 5, 6, 7, 8 };
		status st = is_x ? X : O;
		for (int i : ord)
		{
			bool marked = false;
			for (auto& j : tttlines_with_num[i])
			{
				if (memo[grid_num][j.first] == st && memo[grid_num][j.second] == st)
				{
					ans1.push_back(i);
					marked = true;
					break;
				}
			}
			if (!marked) ans2.push_back(i);
		}
		for (int i : ans2) ans1.push_back(i);
		return ans1;
	}
	static inline void print_memo()
	{
		cout << "\n  1 2 3 4 5 6 7 8 9";
		printf("\n");
		for (int i = 0; i < 9; i++)
		{
			cout << i + 1 << " ";
			for (int j = 0; j < 9; j++)
			{
				coord c(i, j); point p = c.to_point();
				printf("%c", status_to_char(memo[p.grid][p.num]));
				if (j % 3 == 2) printf("|");
				else printf(" ");
			}
			printf("\n");
			if (i % 3 == 2)
				cout << "  - - - - - - - - -\n";
		}
		cout << endl;
	}
	static inline void init()
	{
		for (int i = 0; i < MAXN; i++)
			for (int j = 0; j < MAXN; j++)
				memo[i][j] = BLANK;
		for (int i = 0; i < 9; i++)
			for (auto j : tttlines)
				if (get<0>(j) == i || get<1>(j) == i || get<2>(j) == i)
				{
					if (get<0>(j) == i) tttlines_with_num[i].push_back(make_pair(get<1>(j), get<2>(j)));
					if (get<1>(j) == i) tttlines_with_num[i].push_back(make_pair(get<0>(j), get<2>(j)));
					if (get<2>(j) == i) tttlines_with_num[i].push_back(make_pair(get<0>(j), get<1>(j)));
				}
	}
	static inline void __debuginit()
	{
		string aa[9] = {
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
		for (int i = 0; i < 9; i++) for (int j = 0; j < 9; j++) {
			int x = coord(i, j).to_point().grid, y = coord(i, j).to_point().num;
			if (aa[i][j] == 'O') memo[x][y] = O;
			if (aa[i][j] == 'X') memo[x][y] = X;
			if (aa[i][j] == '*') memo[x][y] = BANNED;
			if (aa[i][j] == ' ') memo[x][y] = BLANK;
		}
		print_memo();
	}
	static inline double heuristic_count_point(bool is_x)
	{
		double cnt = 0;
		status st = is_x ? X : O, notst = is_x ? O : X;
		for (int i = 0; i < MAXN; i++)
		{
			for (auto j : tttlines)
			{
				int a = memo[i][get<0>(j)], b = memo[i][get<1>(j)], c = memo[i][get<2>(j)];
				int k = ((a == st) + (b == st) + (c == st));
				if (a == BANNED || b == BANNED || c == BANNED || a == notst || b == notst || c == notst) k = 0;
				if (k == 1) cnt += heur1;
				else if (k == 2) cnt += heur2;
				else if (k == 3) cnt += 1;
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
			for (auto j : tttlines)
				cnt += (memo[i][get<0>(j)] == st && memo[i][get<1>(j)] == st && memo[i][get<2>(j)] == st);
		}
		return cnt;
	}
	static inline int get_point_diff()
	{
		return count_point(true) - count_point(false);
	}
	static inline pair<int, double> abdfs(int depth, bool is_x, int grid, double alpha = -1e9, double beta = 1e9)
	{
		vector<int> equals;
		abcnt++;
		double best_diff = is_x ? -uninitialized : uninitialized;
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
						return make_pair(i, best_diff);
					alpha = max(alpha, best_diff);
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
						return make_pair(i, best_diff);
					beta = min(beta, best_diff);
				}
				equals.push_back(i);
			}
		}
		if (count(equals.begin(), equals.end(), 4) > 0) best_move = 4;
		else {
			best_move = equals[0];
			best_move = equals[rng() % (int)equals.size()];
		}
		return best_diff == (is_x ? -uninitialized : uninitialized) ? make_pair(-1, (double)get_point_diff()) : make_pair(best_move, best_diff);
	}
	static inline void dfs_grid(int grid)
	{
		for (int i = 0; i < 9; i++)
			if (i != grid && memo[i][grid] == BLANK)
			{
				memo[i][grid] = BANNED;
				if (!check_grid(i))
					dfs_grid(i);
			}
	}
	static inline void game(pair<int, double>(*move_func)(bool, int))
	{
		int move_num = 0;
		int now_grid = 4;
		bool is_x = true;
		while (check_grid(now_grid))
		{
			move_num++;
			pair<int, double> ret = move_func(is_x, now_grid);
			int move = ret.first;
			memo[now_grid][move] = (is_x ? X : O);
			if (!check_grid(now_grid))
				dfs_grid(now_grid);
			point(now_grid, move).print();
			now_grid = move;
			is_x = !is_x;
			print_memo();
			cout << count_point(true) << " : " << count_point(false) << endl << endl;
			if (PRINT_EVAL && ret.second != uninitialized) cout << "Computer Eval: " << ret.second << endl << endl;
		}
		int diff = get_point_diff();
		if (diff > 0) cout << "X Wins!!!!" << endl;
		else if (diff < 0) cout << "O Wins!!!!" << endl;
		else cout << "What a TIE!!!!" << endl;
		cout << "Game ended in " << move_num << " moves.\n";
	}
	static inline void print_result(int start_time)
	{
		printf("Finished calculating with depth %d, used %d ms time.\n", MAXDEPTH, clock() - start_time);
		cout << "Total Calculation Size: " << abcnt << endl;
	}
	static inline void greet_player()
	{
		if (playing_mode == CVSC) return;
		cout << "Welcome to Sudoku - Tic Tac Toe!\n欢迎来玩数独井字棋！\n\n"; Sleep(1000);
		cout << "I think you have already understood the rules of this game.\n你应该已经懂得怎么玩这玩意儿了。\n\n"; Sleep(1000);
		cout << "When you enter the coordinates, please enter the vertical one first and then the horizontal one.\n输入坐标时，先输入竖着的，再输横着的。\n\n"; Sleep(1000);
		cout << "The coordinates is as follows:\n坐标如下：\n\n";
		cout << "  1 2 3 4 5 6 7 8 9\n";
		for (int i = 1; i <= 9; i++)
		{
			cout << i << " ";
			for (int j = 1; j <= 9; j++)
			{
				cout << "X";
				if (j % 3 == 0) cout << "|";
				else cout << " ";
			}
			cout << endl;
			if (i % 3 == 0)
			{
				cout << "  - - - - - - - - -\n";
			}
		}
		cout << endl;
		Sleep(2000);
		if (playing_mode == CVSP) cout << "Computer goes first and you go second.\n电脑先，你后。\n\n";
		else if (playing_mode == PVSC) cout << "You go first and computer goes second.\n你先，电脑后。\n\n";
		else if (playing_mode == PVSP) cout << "You're playing in two-player mode.\n现在是双人模式。\n\n";
		cout << "Let's start!\n我们开始吧！\n\n"; Sleep(1000);
	}
	class Moves
	{
	public:
		static inline int human_move(int now_grid)
		{
			int x, y;
			cout << "Your Move:\n\nEnter coordinates:\n输入坐标：\n";
			cin >> x >> y;
			coord c(x - 1, y - 1); point p = c.to_point();
			while (p.grid != now_grid || p.get_status() != BLANK)
			{
				cout << "\nInvalid input! Try again:\n输入错误！请重新输入：\n";
				cin >> x >> y;
				c.x = x - 1, c.y = y - 1;
				p = c.to_point();
			}
			return p.num;
		}
		static inline pair<int, double> computer_move(bool is_x, int now_grid)
		{
			return abdfs(MAXDEPTH, is_x, now_grid);
		}
		static inline pair<int, double> cvsc_move(bool is_x, int now_grid)
		{
			cout << "Computer's Move:\n\n";
			return computer_move(is_x, now_grid);
		}
		static inline pair<int, double> cvsp_move(bool is_x, int now_grid)
		{
			if (is_x) return cout << "Computer's Move:\n\n", abdfs(MAXDEPTH, is_x, now_grid);
			else return make_pair(human_move(now_grid), uninitialized);
		}
		static inline pair<int, double> pvsc_move(bool is_x, int now_grid)
		{
			if (!is_x) return cout << "Computer's Move:\n\n", abdfs(MAXDEPTH, is_x, now_grid);
			else return make_pair(human_move(now_grid), uninitialized);
		}
		static inline pair<int, double> pvsp_move(bool is_x, int now_grid)
		{
			return make_pair(human_move(now_grid), uninitialized);
		}
		static inline pair<int, double> auto_move(bool is_x, int now_grid)
		{
			if (playing_mode == CVSC) return cvsc_move(is_x, now_grid);
			if (playing_mode == CVSP) return cvsp_move(is_x, now_grid);
			if (playing_mode == PVSC) return pvsc_move(is_x, now_grid);
			if (playing_mode == PVSP) return pvsp_move(is_x, now_grid);
		}
	};
};

signed main()
{
	using namespace SudokuTicTacToe;
	int start_time = clock();
	greet_player();
	init();
	game(Moves::auto_move);
	print_result(start_time);
}
