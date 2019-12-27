#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cstdlib>

using namespace std;

//������ز���
#define MAX 101101
int sudo[2][10][10]; //���ڴ���������һ����������
int flag = 0; //�н��־��0Ϊ�޽⣬1Ϊһ�⣬2Ϊ���

//��������
inline int read(); //�����Ż�
inline void Putsudo(int); //���ĳһ���õ�����
void Input(); //��������
void Output(); //�������
void Makelink(); //����link
bool Check(int, int, int, int, int[10][10], int[10][10], int[10][10]); //�ж����λ�����ܷ���num�����
inline void Fillblock(int, int, int, int, int[10][10], int[10], int[10][10], int[10], int[10][10], int[10]); //��ָ��������������ռλ
void Preprocess(int[10][10], int[10], int[10][10], int[10], int[10][10], int[10], int&); //Ԥ��������

//����ṹ�����DLX
struct DLX
{
	//����DLX�ṹ����������ɲ���
	int n, m, cnt;//����Ķ�ά�������������
	int s[MAX];//ÿ�еĽ���� 
	int h[MAX];//ÿ�е�ͷ��� 
	int ansk[MAX];//�洢��
	int l[MAX], r[MAX], u[MAX], d[MAX], row[MAX], col[MAX];//ÿ��������ң����£��У�����Ϣ 

	//��ʼ������������ÿ�е�ͷ���
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
		r[m] = 0;//m�ұ���0 
		l[0] = m;//0�����m 
		memset(h, -1, sizeof(h));
		memset(s, 0, sizeof(s));
		cnt = m + 1;//��ʼʱ��m����㣨0���͸���ͷ��㣩 
	}

	//��R��C�н����µĵ�
	void link(int R, int C)
	{
		s[C]++;
		row[cnt] = R;
		col[cnt] = C;
		u[cnt] = C;
		d[cnt] = d[C];
		u[d[C]] = cnt;
		d[C] = cnt;

		//������û�б�ĵ�,��ѵ�һ������ĵ���Ϊ���е���ͷ��� 
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

	//ɾ��c�к�c�����е����
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

	//�ָ�c�к�c�����е����
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

	//�������
	void dance(int deep)
	{
		if (r[0] == 0)
		{
			int x, y, v;
			for (int i = 0; i < deep; i++)
			{
				x = (ansk[i] - 1) / 9 / 9;
				y = (ansk[i] - 1) / 9 % 9;
				v = (ansk[i]) % 9;//������Ϣת���ɶ�Ӧ�ĵ��ֵ 
				if (v == 0)
					v = 9;//��ֹ9�ı���%9==0 
				sudo[flag][x][y] = v;
			}

			//���������еĽ⣬�ҵ��ڶ�����ʱ���ǰ������
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
		//�ҵ������ٵ���
		for (int i = r[0]; i != 0; i = r[i])
			if (s[i] < s[c])
				c = i;
		remove(c);

		for (int i = d[c]; i != c; i = d[i])
		{
			ansk[deep] = row[i];
			for (int j = r[i]; j != i; j = r[j])
				remove(col[j]);
			dance(deep + 1);//��һ��
			for (int j = l[i]; j != i; j = l[j])
				resume(col[j]);
		}
		resume(c);
		return;
	}
}dlx;

//������
int main()
{
	//��ʼ��dlx�Ĳ���
	dlx.init(729, 324);

	//�������ݲ������ݽ���Ԥ����
	Input();

	//����link
	Makelink();

	//��ʼ�������
	dlx.dance(0);

	//�����
	Output();

	return 0;
}

//�����Ż�
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

//���ĳһ���õ�����
inline void Putsudo(int num)
{
	for (int i = 0; i <= 8; i++)
	{
		for (int j = 0; j <= 8; j++)
			printf("%d ", sudo[num][i][j]);
		printf("\n");
	}
}

//�������ݲ�����Ԥ����
void Input()
{
	//����һЩ��������״̬�ı���
	int col[10][10] = { {0} }; //�洢ÿ�����������ֵ������0��ʾ���ã�1��ʾ������
	int row[10][10] = { {0} }; //�洢ÿ�����
	int pal[10][10] = { {0} }; //�洢ÿ�����
	int col_n[10] = { 0 }; //ÿ������������
	int row_n[10] = { 0 }; //ÿ������������
	int pal_n[10] = { 0 }; //ÿ������������
	int cnt = 0; //�������
	bool legal = true; //�Ϸ��Ա���

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

			//���㹬��
			int pal_num = i / 3 * 3 + j / 3 + 1;

			//�жϺϷ���
			if (!Check(i, j, pal_num, k, col, row, pal))
				legal = false;

			//ռλ
			Fillblock(i, j, pal_num, k, col, col_n, row, row_n, pal, pal_n);
		}

	//�жϺϷ���
	if (!legal)
	{
		printf("No solution\n");
		exit(0);
	}

	//Ԥ��������
	Preprocess(col, col_n, row, row_n, pal, pal_n, cnt);

	//���Ԥ����ʱ�Ѿ������ˣ�������Ⲣ����
	if (cnt == 0) 
	{
		flag++;
		Output();
		exit(0);
	}
}

//�����Ľ��
void Output()
{
	//�н��־Ϊ2��˵���ж�⣬Ϊ1˵����Ψһ�⣬Ϊ0˵���޽�
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

//����link
void Makelink()
{
	for (int i = 0; i <= 8; i++)
		for (int j = 0; j <= 8; j++)
			for (int k = 1; k <= 9; k++)
			{
				if (sudo[flag][i][j] != k && sudo[flag][i][j] != 0)//�Ѿ���õĵ�Ͳ��ÿ����� 
					continue;
				int o = i * 9 * 9 + j * 9 + k;
				dlx.link(o, i * 9 + j + 1);
				dlx.link(o, i * 9 + 81 + k);
				dlx.link(o, j * 9 + 81 * 2 + k);
				dlx.link(o, 81 * 3 + (i / 3 * 3 + j / 3) * 9 + k);//�ѵ��Ӧ���У����ϣ� 
			}
}

//�ж����λ�����ܷ���num�����
bool Check(int i, int j, int pal_num, int num, int col[10][10], int row[10][10], int pal[10][10]) 
{
	if (col[j][num] == 1 || row[i][num] == 1 || pal[pal_num][num] == 1)
		return false;
	return true;
}

//��ָ��λ��������
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

//Ԥ��������
void Preprocess(int col[10][10], int col_n[10], int row[10][10], int row_n[10], int pal[10][10], int pal_n[10], int &cnt)
{
	//׼����һЩ����
	int po_ans[10][10][10]; //ÿ��λ���ϵĺ�ѡ��
	int po_ans_n[10][10] = { {0} }; //ÿ��λ���ϵĺ�ѡ����
	int po_row_n[10][10] = { {0} }; //ÿ��ÿ�����ĺ�ѡ����
	int po_row[10][10][10]; //ÿ��ÿ�����ĺ�ѡ�⣬�洢�����к�
	int po_col_n[10][10] = { {0} }; //��Ӧ�������
	int po_col[10][10][10]; //ÿ��ÿ�����ĺ�ѡ�⣬�洢�����к�

	int filled = 0; //��¼�Ѿ����˶�����
	//���Ӻ�ѡ������������Щֻ��һ��ѡ��ĸ���
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

	//�к�ѡ����ÿ���бض�����1-9�����֣����ڻ�û����������֣�����ÿһ�������������ֻ��һ�����ӿ������֮
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

	//�к�ѡ����ÿ�б���1-9��ԭ�����к�ѡ��ͬ
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

	if (filled == 0) //�������û���Ż����
		return;

	while (true) //��������������ȥΪֹ
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