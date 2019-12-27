#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cstdlib>

using namespace std;

//设置相关参数
#define MAX 101101
int sudo[2][10][10]; //用于储存数独的一（两）个解
int flag = 0; //有解标志，0为无解，1为一解，2为多解

//函数声明
inline int read(); //读入优化
inline void Putsudo(int); //输出某一组解好的数独
void Input(); //输入数据
void Output(); //输出数据
void Makelink(); //创建link
bool Check(int, int, int, int, int[10][10], int[10][10], int[10][10]); //判断这个位置上能否填num这个数
inline void Fillblock(int, int, int, int, int[10][10], int[10], int[10][10], int[10], int[10][10], int[10]); //在指定格子上填数并占位
void Preprocess(int[10][10], int[10], int[10][10], int[10], int[10][10], int[10], int&); //预处理数独

//定义结构体变量DLX
struct DLX
{
	//定义DLX结构体变量的若干参数
	int n, m, cnt;//矩阵的二维参数，点的总数
	int s[MAX];//每列的结点数 
	int h[MAX];//每行的头结点 
	int ansk[MAX];//存储答案
	int l[MAX], r[MAX], u[MAX], d[MAX], row[MAX], col[MAX];//每个点的左，右，上下，行，列信息 

	//初始化函数，生成每列的头结点
	void init(int _n, int _m)
	{
		n = _n;
		m = _m;
		for (int i = 0; i <= m; i++)
		{
			r[i] = i + 1;
			l[i] = i - 1;
			u[i] = d[i] = i;
		}
		r[m] = 0;//m右边是0 
		l[0] = m;//0左边是m 
		memset(h, -1, sizeof(h));
		memset(s, 0, sizeof(s));
		cnt = m + 1;//开始时有m个结点（0结点和各列头结点） 
	}

	//在R行C列建立新的点
	void link(int R, int C)
	{
		s[C]++;
		row[cnt] = R;
		col[cnt] = C;
		u[cnt] = C;
		d[cnt] = d[C];
		u[d[C]] = cnt;
		d[C] = cnt;

		//若该行没有别的点,则把第一个加入的点作为该行的行头结点 
		if (h[R] < 0)
			h[R] = r[cnt] = l[cnt] = cnt;
		else
		{
			r[cnt] = h[R];
			l[cnt] = l[h[R]];
			r[l[h[R]]] = cnt;
			l[h[R]] = cnt;
		}
		cnt++;
	}

	//删除c列和c列上有点的行
	void remove(int c)
	{
		r[l[c]] = r[c];
		l[r[c]] = l[c];
		for (int i = d[c]; i != c; i = d[i])
		{
			for (int j = r[i]; j != i; j = r[j])
			{
				u[d[j]] = u[j];
				d[u[j]] = d[j];
				s[col[j]]--;
			}
		}
	}

	//恢复c列和c列上有点的行
	void resume(int c)
	{
		for (int i = u[c]; i != c; i = u[i])
		{
			for (int j = l[i]; j != i; j = l[j])
			{
				u[d[j]] = j;
				d[u[j]] = j;
				s[col[j]]++;
			}
		}
		r[l[c]] = c;
		l[r[c]] = c;
	}

	//求解数独
	void dance(int deep)
	{
		if (r[0] == 0)
		{
			int x, y, v;
			for (int i = 0; i < deep; i++)
			{
				x = (ansk[i] - 1) / 9 / 9;
				y = (ansk[i] - 1) / 9 % 9;
				v = (ansk[i]) % 9;//把行信息转换成对应的点的值 
				if (v == 0)
					v = 9;//防止9的倍数%9==0 
				sudo[flag][x][y] = v;
			}

			//遍历完所有的解，找到第二个解时输出前两个解
			if (flag == 0)
				flag++;
			else
			{
				flag++;
				Output();
				exit(0);
			}
			return;
		}

		int c = r[0];
		//找到点最少的列
		for (int i = r[0]; i != 0; i = r[i])
			if (s[i] < s[c])
				c = i;
		remove(c);

		for (int i = d[c]; i != c; i = d[i])
		{
			ansk[deep] = row[i];
			for (int j = r[i]; j != i; j = r[j])
				remove(col[j]);
			dance(deep + 1);//下一步
			for (int j = l[i]; j != i; j = l[j])
				resume(col[j]);
		}
		resume(c);
		return;
	}
}dlx;

//主函数
int main()
{
	//初始化dlx的参数
	dlx.init(729, 324);

	//读入数据并对数据进行预处理
	Input();

	//创建link
	Makelink();

	//开始求解数独
	dlx.dance(0);

	//输出解
	Output();

	return 0;
}

//读入优化
inline int read()
{
	char ch = ' ';
	while (!isgraph(ch))
		ch = getchar();
	if (ch == '-')
		return 0;
	else
		return ch - '0';
}

//输出某一组解好的数独
inline void Putsudo(int num)
{
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
			printf("%d ", sudo[num][i][j]);
		printf("\n");
	}
}

//读入数据并进行预处理
void Input()
{
	//设置一些描述数独状态的变量
	int col[10][10] = { {0} }; //存储每列上已填数字的情况，0表示可用，1表示不可用
	int row[10][10] = { {0} }; //存储每行情况
	int pal[10][10] = { {0} }; //存储每宫情况
	int col_n[10] = { 0 }; //每列已填数字数
	int row_n[10] = { 0 }; //每行已填数字数
	int pal_n[10] = { 0 }; //每宫已填数字数
	int cnt = 0; //待填方格数
	bool legal = true; //合法性变量

	for (int i = 0; i <= 8; i++)
		for (int j = 0; j <= 8; j++)
		{
			int k = read();
			if (!k)
			{
				cnt++;
				sudo[flag][i][j] = k;
				continue;
			}

			//计算宫号
			int pal_num = i / 3 * 3 + j / 3 + 1;

			//判断合法性
			if (!Check(i, j, pal_num, k, col, row, pal))
				legal = false;

			//占位
			Fillblock(i, j, pal_num, k, col, col_n, row, row_n, pal, pal_n);
		}

	//判断合法性
	if (!legal)
	{
		printf("No solution\n");
		exit(0);
	}

	//预处理数独
	Preprocess(col, col_n, row, row_n, pal, pal_n, cnt);

	//如果预处理时已经填满了，就输出解并结束
	if (cnt == 0) 
	{
		flag++;
		Output();
		exit(0);
	}
}

//输出解的结果
void Output()
{
	//有解标志为2，说明有多解，为1说明有唯一解，为0说明无解
	if (flag == 2)
	{
		printf("Multiple solutions\n");
		Putsudo(0);
		printf("\n");
		Putsudo(1);
	}
	else if (flag == 1)
	{
		printf("OK\n");
		Putsudo(0);
	}
	else
		printf("No solution\n");
}

//创建link
void Makelink()
{
	for (int i = 0; i <= 8; i++)
		for (int j = 0; j <= 8; j++)
			for (int k = 1; k <= 9; k++)
			{
				if (sudo[flag][i][j] != k && sudo[flag][i][j] != 0)//已经填好的点就不用考虑了 
					continue;
				int o = i * 9 * 9 + j * 9 + k;
				dlx.link(o, i * 9 + j + 1);
				dlx.link(o, i * 9 + 81 + k);
				dlx.link(o, j * 9 + 81 * 2 + k);
				dlx.link(o, 81 * 3 + (i / 3 * 3 + j / 3) * 9 + k);//把点对应成行（集合） 
			}
}

//判断这个位置上能否填num这个数
bool Check(int i, int j, int pal_num, int num, int col[10][10], int row[10][10], int pal[10][10]) 
{
	if (col[j][num] == 1 || row[i][num] == 1 || pal[pal_num][num] == 1)
		return false;
	return true;
}

//在指定位置上填数
inline void Fillblock(int i, int j, int pal_num, int num, int col[10][10], int col_n[10], int row[10][10], int row_n[10], int pal[10][10], int pal_n[10])
{
	sudo[flag][i][j] = num;
	col[j][num] = 1;
	col_n[j]++;
	row[i][num] = 1;
	row_n[i]++;
	pal[pal_num][num] = 1;
	pal_n[pal_num]++;
}

//预处理数独
void Preprocess(int col[10][10], int col_n[10], int row[10][10], int row_n[10], int pal[10][10], int pal_n[10], int &cnt)
{
	//准备好一些参数
	int po_ans[10][10][10]; //每个位置上的候选解
	int po_ans_n[10][10] = { {0} }; //每个位置上的候选解数
	int po_row_n[10][10] = { {0} }; //每行每个数的候选解数
	int po_row[10][10][10]; //每行每个数的候选解，存储的是列号
	int po_col_n[10][10] = { {0} }; //相应的列情况
	int po_col[10][10][10]; //每列每个数的候选解，存储的是行号

	int filled = 0; //记录已经填了多少数
	//格子候选法：先填满那些只有一个选择的格子
	for (int i = 0; i <= 8; i++)
		for (int j = 0; j <= 8; j++)
		{
			if (sudo[flag][i][j] == 0)
			{
				int pal_num = i / 3 * 3 + j / 3 + 1;
				for (int k = 1; k <= 9; k++)
				{
					if (Check(i, j, pal_num, k, col, row, pal))
					{
						po_ans_n[i][j]++;
						po_ans[i][j][po_ans_n[i][j]] = k;
					}
				}
				if (po_ans_n[i][j] == 1)
				{
					Fillblock(i, j, pal_num, po_ans[i][j][1], col, col_n, row, row_n, pal, pal_n);
					filled++;
					cnt--;
				}
				else if (po_ans_n[i][j] == 0)
				{
					printf("No solution\n");
					exit(0);
				}
			}
		}

	//行候选法：每个行必定含有1-9的数字，对于还没有填入的数字，搜索每一个格，如果该数字只有一个格子可以填，填之
	for (int i = 0; i <= 8; i++)
		for (int k = 1; k <= 9; k++)
		{
			if (row[i][k] == 0)
			{
				for (int j = 0; j <= 8; j++)
				{
					int pal_num = i / 3 * 3 + j / 3 + 1;
					if (sudo[flag][i][j] == 0 && Check(i, j, pal_num, k, col, row, pal))
					{
						po_row_n[i][k]++;
						po_row[i][k][po_row_n[i][k]] = j;
					}
				}
				if (po_row_n[i][k] == 1)
				{
					Fillblock(i, po_row[i][k][1], i / 3 * 3 + po_row[i][k][1] / 3 + 1, k, col, col_n, row, row_n, pal, pal_n);
					filled++;
					cnt--;
				}
				else if (po_row_n[i][k] == 0)
				{
					printf("No solution\n");
					exit(0);
				}
			}
		}

	//列候选法：每列必有1-9，原理与行候选相同
	for (int j = 0; j <= 8; j++)
		for (int k = 1; k <= 9; k++)
		{
			if (col[j][k] == 0)
			{
				for (int i = 0; i <= 8; i++)
				{
					int pal_num = i / 3 * 3 + j / 3 + 1;
					if (sudo[flag][i][j] == 0 && Check(i, j, pal_num, k, col, row, pal))
					{
						po_col_n[j][k]++;
						po_col[j][k][po_col_n[j][k]] = i;
					}
				}
				if (po_col_n[j][k] == 1)
				{
					Fillblock(po_col[j][k][1], j, po_col[j][k][1] / 3 * 3 + j / 3 + 1, k, col, col_n, row, row_n, pal, pal_n);
					filled++;
					cnt--;
				}
				else if (po_col_n[j][k] == 0)
				{
					printf("No solution\n");
					exit(0);
				}
			}
		}

	if (filled == 0) //无数可填，没有优化余地
		return;

	while (true) //迭代填数，填到填不下去为止
	{
		int filled_2 = 0;

		for (int i = 0; i <= 8; i++)
			for (int j = 0; j <= 8; j++)
			{
				if (sudo[flag][i][j] == 0)
				{
					int pal_num = i / 3 * 3 + j / 3 + 1;
					for (int k = 1; k <= po_ans_n[i][j]; k++)
					{
						if (!Check(i, j, pal_num, po_ans[i][j][k], col, row, pal))
						{
							swap(po_ans[i][j][k], po_ans[i][j][po_ans_n[i][j]]);
							po_ans_n[i][j]--;
							k--;
						}
					}
					if (po_ans_n[i][j] == 1)
					{
						Fillblock(i, j, pal_num, po_ans[i][j][1], col, col_n, row, row_n, pal, pal_n);
						filled_2++;
						cnt--;
					}
					else if (po_ans_n[i][j] == 0)
					{
						printf("No solution\n");
						exit(0);
					}
				}
			}

		for (int i = 0; i <= 8; i++)
			for (int k = 1; k <= 9; k++)
			{
				if (row[i][k] == 0)
				{
					for (int b = 1; b <= po_row_n[i][k]; b++)
					{
						int j = po_row[i][k][b];
						int pal_num = i / 3 * 3 + j / 3 + 1;
						if (!(sudo[flag][i][j] == 0 && Check(i, j, pal_num, k, col, row, pal)))
						{
							swap(po_row[i][k][b], po_row[i][k][po_row_n[i][k]]);
							po_row_n[i][k]--;
							b--;
						}
					}
					if (po_row_n[i][k] == 1)
					{
						Fillblock(i, po_row[i][k][1], i / 3 * 3 + po_row[i][k][1] / 3 + 1, k, col, col_n, row, row_n, pal, pal_n);
						filled_2++;
						cnt--;
					}
					else if (po_row_n[i][k] == 0)
					{
						printf("No solution\n");
						exit(0);
					}
				}
			}

		for (int j = 0; j <= 8; j++)
			for (int k = 1; k <= 9; k++)
			{
				if (col[j][k] == 0)
				{
					for (int b = 1; b <= po_col_n[j][k]; b++)
					{
						int i = po_col[j][k][b];
						int pal_num = i / 3 * 3 + j / 3 + 1;
						if (!(sudo[flag][i][j] == 0 && Check(i, j, pal_num, k, col, row, pal)))
						{
							swap(po_col[j][k][b], po_col[j][k][po_col_n[j][k]]);
							po_col_n[j][k]--;
							b--;
						}
					}
					if (po_col_n[j][k] == 1)
					{
						Fillblock(po_col[j][k][1], j, po_col[j][k][1] / 3 * 3 + j / 3 + 1, k, col, col_n, row, row_n, pal, pal_n);
						filled_2++;
						cnt--;
					}
					else if (po_col_n[j][k] == 0)
					{
						printf("No solution\n");
						exit(0);
					}
				}
			}

		if (filled_2 == 0)
			return;
	}
}