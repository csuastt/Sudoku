/*********** Headers ****************/ 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;

/************* Defines **************/
#define NO_SOLUTION 0
#define ONE_SOLUTION 1
#define MANY_SOLUTION -1
//Used in the Solve Function.
#define HEAD 0
#define COLUMNS 324
//Used in Dancing Link.

/*********** Struct: Board ***********/
/* Use this struct to save a Sudoku board. */
struct Board
{
	int board[9][9]; //the board
	int filled;      //How many blocks are filled.
	Board()          //Initialization
	{
		memset(board,0,sizeof(board));
		filled=0;
	}
	void read()     //Read a board from input. Use '-' to represent an empty block. Separated by a single space.
	{
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++)
			{
				char x;
				cin>>x;
				if(x=='-')
					board[i][j]=0;
				else
				{
					board[i][j]=x-'0';
					filled++;
				}
			}
	}
	void print()const //Output a board. Use '-' to represent an empty block. Separated by a single space.
	{
		for(int i=0;i<9;i++,puts(""))
			for(int j=0;j<9;j++)
				printf("%c ",board[i][j]?board[i][j]+'0':'-');
	}
	const int* operator[](const int &r) const//(DONE)Redefine [] for convenience. PLEASE DO NOT MODIFY THIS!!
	{
		return board[r];
	}
	bool check(const int &x,const int &y,const int &val)const //Check whether we can put val on position(x,y)
	{
		for(int i=0;i<9;i++)
			if(board[x][i]==val||board[i][y]==val)
				return false;
		int xx=x/3*3,yy=y/3*3;
		for(int i=0;i<3;i++)
			for(int j=0;j<3;j++)
				if(board[xx+i][yy+j]==val)
					return false;
		return true;
	}
	void putNumber(const int &x,const int &y,const int &val) //Modify position (x,y) from empty to val
	{
		board[x][y]=val;
		filled++;
	}
	void eraseNumber(const int &x,const int &y) //Modify position (x,y) to empty
	{
		board[x][y]=0;
		filled--;
	}
	/********************** Note **************************/
	//Please use putNumber and eraseNumber to modify board./
	//        DO NOT MODIFY THE BOARD DIRECTLY!!           /
	/******************************************************/
};

/********* Global Variables *********/
vector<Board> g_ans; //Use to save answers
/********************************************* Note ************************************************/
//If you want to define other global variable, please use prefix "g_" to prevent same variable name./
/***************************************************************************************************/

/*********** Struct Block ***********/
//Used to save information of one block.
struct Block
{
	int x,y;
	int val;
	Block(const int &_x,const int &_y,const int &_val):x(_x),y(_y),val(_val){}
	Block():x(0),y(0),val(0){}
};

/******** Struct DancingLinkX *******/
//This struct is used to implement the dancing link X algorithm.
//Column 1-81:   Position (x,y) has a number. (x*9+y)+1
//Column 82-162: Line i has number x.         (i*9+x-1)+82
//Column 163-243:Column i has number x.       (i*9+x-1)+163
//Column 244-324:Block i has number x.        (i*9+x-1)+244 
struct DancingLinkX
{
	int l[5000],r[5000],u[5000],d[5000];
	int row[5000],col[5000];
	int rows;
	int nodecnt;
	Block rowBlock[5000];
	DancingLinkX()
	{
		memset(l,0,sizeof(l));
		memset(r,0,sizeof(r));
		memset(u,0,sizeof(u));
		memset(d,0,sizeof(d));
		nodecnt=0;
		u[0]=0;
		d[0]=0;
		r[0]=1;
		l[0]=COLUMNS;
		for(int i=1;i<=COLUMNS;i++)
		{
			u[i]=i;
			d[i]=i;
			l[i]=i-1;
			r[i]=i+1;
			row[i]=0;
		}
		r[COLUMNS]=0;
		rows=0;
		nodecnt=COLUMNS+1;
	}
	int getNewNode()
	{
		return nodecnt++;
	}
	void appendLine(const vector<int> &idx,const int &x,const int &y,const int &val)
	{
		rows++;
		rowBlock[rows]=Block(x,y,val);
		for(int i=0;i<idx.size();i++)
		{
			int cur=getNewNode();
			if(i==0)
			{
				l[cur]=cur;
				r[cur]=cur;
			}
			else
			{
				l[cur]=cur-1;
				r[cur]=r[cur-1];
				l[r[cur-1]]=cur;
				r[cur-1]=cur;
			}
			d[cur]=idx[i];
			u[cur]=u[idx[i]];
			d[u[idx[i]]]=cur;
			u[idx[i]]=cur;
			row[cur]=rows;
			col[cur]=idx[i];
		}
	}
	void removeColumn(const int &column)
	{
		l[r[column]]=l[column];
		r[l[column]]=r[column];
		for(int i=d[column];i!=column;i=d[i])
			for(int j=r[i];j!=i;j=r[j])
			{
				u[d[j]]=u[j];
				d[u[j]]=d[j];
			}
	}
	void recoverColumn(const int &column)
	{
		l[r[column]]=column;
		r[l[column]]=column;
		for(int i=u[column];i!=column;i=u[i])
			for(int j=r[i];j!=i;j=r[j])
			{
				u[d[j]]=j;
				d[u[j]]=j;
			}
	}
	int curAns[200];
	void dance(int level,const bool &allAnsFlag)
	{
		if(!allAnsFlag&&g_ans.size()>=2)
			return;
		int curColumn=r[HEAD];
		if(curColumn==HEAD)
		{
			Board ans;
			for(int i=1;i<level;i++)
			{
				Block cur=rowBlock[curAns[i]];
				ans.putNumber(cur.x,cur.y,cur.val);
			}
			g_ans.push_back(ans);
			return;
		}
		removeColumn(curColumn);
		for(int i=d[curColumn];i!=curColumn;i=d[i])
		{
			curAns[level]=row[i];
			for(int j=r[i];j!=i;j=r[j])
				removeColumn(col[j]);
			dance(level+1,allAnsFlag);
			for(int j=l[i];j!=i;j=l[j])
				recoverColumn(col[j]);
		}
		recoverColumn(curColumn);
	}
};

/******** Function Predefine ********/
int Rand(); //Generate a random positive integer
/******************************************* Note ***********************************************/
//Please use this Rand() instead of rand() directly, for rand() in Windows is easy to get a loop!/
/************************************************************************************************/

void Search(const Board &board,const bool &allAnsFlag);
//Search for answers.
//If ansAnsFlag==true, you need to find all the answers, otherwise you only need to judge this Sudoku has 0, 1 or more than 1 answer.
//Save the answers in the vector g_ans.

int Solve(const Board &board,const bool &allAnsFlag);
//Use to prepare for the search. Main Function and Generate Function will use this instead of Search.
//If ansAnsFlag==true, you need to return the quantity of answers.
//Otherwise return NO_SOLUTION, ONE_SOLUTION or MANY_SOLUTION

int SolveDLX(const Board &board,const bool &allAnsFlag);
//Use Dancing Link X to solve a Sudoku.

Board GenerateFull();//Use to Generate a Full Sudoku board.

Board GenerateSudoku(const Board &full,const int &solCnt);
//If solCnt!=-1, generate a Sudoku with MORE THAN solCnt solution(s). (Maybe we'll change it to exact solCnt solution(s).TBD)
//If solCnt==-1, please generate a Sudoku with ONLY ONE solution.
//The Sudoku board returned should not contain more than 40 numbers.

/********** Function Rand ***********/
int Rand()
{
	if(RAND_MAX==32767) //Windows
		return rand()<<15|rand();
	return rand();      //Otherwise
}

/****** Function GenerateFull *******/
Board GenerateFull()
{
	ReGenerate:;
	Board ret;
	for(int r=0;r<9;r++)
	{
		vector<int> v;
		for(int i=1;i<=9;i++)
			v.push_back(i);
		random_shuffle(v.begin(),v.end());
		queue<int> q;
		for(int i=0;i<9;i++)
			q.push(v[i]);
		for(int i=0;i<9;i++)
		{
			int now=q.front();
			int cnt=0;
			while(!ret.check(r,i,now))
			{
				cnt++;
				if(cnt>q.size())
					goto ReGenerate;
				q.pop();
				q.push(now);
				now=q.front();
			}
			q.pop();
			ret.putNumber(r,i,now);
		}
	}
	return ret;
}

/***** Function GenerateSudoku ******/
Board GenerateSudoku(const Board &full,const int &solCnt)
{
	bool checkflag[9][9];
	memset(checkflag,0,sizeof(checkflag));
	Board ret=full;
	while(true)
	{
		int cnt=0;
		memset(checkflag,0,sizeof(checkflag));
		ReErase:;
		if(cnt>=ret.filled)
			break;
		int x=Rand()%9;
		int y=Rand()%9;
		while(ret[x][y]==0||checkflag[x][y])
		{
			x=Rand()%9;
			y=Rand()%9;
		}
		checkflag[x][y]=true;
		int save=ret[x][y];
		ret.eraseNumber(x,y);
		if(solCnt==-1)
		{
			if(Solve(ret,false)==-1)
			{
				ret.putNumber(x,y,save);
				cnt++;
				goto ReErase;
			}
		}
		else
		{
			if(Solve(ret,true)>solCnt)
				return ret;
		}
	}
	return ret;
}

/********* Function Solve ***********/
int Solve(const Board &board,const bool &allAnsFlag)
{
	Board tmp=board;
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			if(board[i][j]!=0)
			{
				tmp.eraseNumber(i,j);
				if(!tmp.check(i,j,board[i][j]))
					return NO_SOLUTION;
				tmp.putNumber(i,j,board[i][j]);
			}
	g_ans.clear();
	Search(board,allAnsFlag);
	if(allAnsFlag)
		return g_ans.size();
	else
	{
		if(g_ans.size()==0)
			return NO_SOLUTION;
		else if(g_ans.size()==1)
			return ONE_SOLUTION;
		else
			return MANY_SOLUTION;
	}
}

/********* Function Search **********/
void Search(const Board &board,const bool &allAnsFlag)
{
	if(!allAnsFlag&&g_ans.size()>=2)
		return;
	if(board.filled==81)
	{
		g_ans.push_back(board);
		return;
	}
	int mn=10,mnx=0,mny=0;
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			if(board[i][j]==0)
			{
				int cnt=0;
				for(int k=1;k<=9;k++)
					if(board.check(i,j,k))
						cnt++;
				if(cnt<mn)
				{
					mn=cnt;
					mnx=i;
					mny=j;
				}
			}
	if(mn==10)
		return;
	Board next=board;
	for(int i=1;i<=9;i++)
		if(board.check(mnx,mny,i))
		{
			next.putNumber(mnx,mny,i);
			Search(next,allAnsFlag);
			next.eraseNumber(mnx,mny);
		}
}

/********* Function SolveDLX ********/
int SolveDLX(const Board &board,const bool &allAnsFlag)
{
	g_ans.clear();
	DancingLinkX solver;
	vector<int> cur;
	cur.resize(4,0);
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
		{
			cur[0]=i*9+j+1;
			if(board[i][j])
			{
				cur[1]=i*9+board[i][j]-1+82;
				cur[2]=j*9+board[i][j]-1+163;
				int block=i/3*3+j/3;
				cur[3]=block*9+board[i][j]-1+244;
				solver.appendLine(cur,i,j,board[i][j]);
			}
			else
			{
				for(int k=1;k<=9;k++)
					if(board.check(i,j,k))
					{
						cur[1]=i*9+k-1+82;
						cur[2]=j*9+k-1+163;
						int block=i/3*3+j/3;
						cur[3]=block*9+k-1+244;
						solver.appendLine(cur,i,j,k);
					}
			}
		}
	solver.dance(1,allAnsFlag);
	if(allAnsFlag)
		return g_ans.size();
	else
	{
		if(g_ans.size()==0)
			return NO_SOLUTION;
		else if(g_ans.size()==1)
			return ONE_SOLUTION;
		else
			return MANY_SOLUTION;
	}
}
/*********** Main function **********/
//Have 4 Input Modes.
//Mode 1:Input a single number 1.
//Output a Sudoku with ONLY ONE solution.
//The Sudoku board should not contain more than 40 numbers.
//Mode 2:Input a number 2, following which is a Sudoku board.
//If there is no solution for this Sudoku, output "No Solution"(without quote).
//If there is ONLY ONE solution, output "OK"(without quote) and then output the solution.
//If there is MORE THAN ONE solution, output "Multiple Solutions"(without quote) and then output 2 possible solutions separated by an empty line.
//Mode 3: Give a file name.
//Solve the input from the given file.
//Output to "output.txt"(without quote).
//Mode 4: Give a file name and a command "-force"(without quote).
//Solve the input from the given file BY THE BRUTE FORCE ALGORITHM.
//Output to "output.txt"(without quote).
int main(int argv,const char **argc)
{
	bool forceFlag=true;
	if(argv!=1)
	{
		freopen(argc[1],"r",stdin);
		freopen("output.txt","w",stdout);
		if(argv>=3&&strcmp(argc[2],"-force")==0)
			forceFlag=true;
	}
	int op;
	scanf("%d",&op);
	if(op==1)
	{
		Board full=GenerateFull();
		Board question=GenerateSudoku(full,-1);
		question.print();
	}
	else
	{
		Board question;
		question.read();
		if(forceFlag)
			Solve(question,false);
		else
			SolveDLX(question,false);
		if(g_ans.size()==0)
			return puts("No_solution"),0;
		else if(g_ans.size()>1)
		{
			puts("Multiple_solutions");
			for(int i=0;i<g_ans.size();i++)
			{
				g_ans[i].print();
				puts("");
			}
		}
		else
		{
			puts("OK");
			g_ans[0].print();
		}
	}
	return 0;
}

