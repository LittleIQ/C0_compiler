/*C0zzb.h-郭威*/
#include<iostream>
#include<fstream>
#include<stdio.h>

enum fct
{
	LIT, LOD, STO,
	CAL, INT, JMP,
	JPC, ADD, SUB,
	MUL, DIV, RED,
	WRT, RET,
};
struct instruction
{
	enum fct f;
	int l;
	int a;
};
#define norw 8      //关键字的个数  word长度
#define al 10
#define txmax 100    //名字表容量
#define nmax 14
#define cxmax 200
#define levmax 3
#define amax 2047   //地址上界
#define stacksize 500


/*符号常量*/
enum symbol
{
	nul, ident, number, plussym, minussym,
	times, slash, lparen, rparen, comma,
	semicolon, becomes, beginsym, ifsym, whilesym,
	writesym, readsym, dosym, callsym, varsym,
	lbrace, rbrace, intsym, voidsym, elsesym,
	scanfsym, printfsym, returnsym,
};
#define symnum 28
#define fctnum 14

//名字表中的类型
enum object
{
	variable, void_C0
};
struct tablestruct
{
	char name[al];
	enum object kind;
	int val;
	int level;
	int adr;
	int size;
};
struct tablestruct table[txmax];
int err;   //错误计数器
FILE *fas;
FILE *fa;
FILE *fa1;
FILE *fa2;
FILE *fin;
FILE *fout;
char fname[al];
bool listswitch;
bool tableswitch;
char ch;
int cx;   //当前code的指针
char line[81];
enum symbol sym;
enum symbol ssym[256];   //单字符的符号值
char word[norw][al];
enum symbol wsym[norw];
char mnemonic[fctnum][5];   //虚拟机代码指令名称
bool declbegsys[symnum];
bool statbegsys[symnum];
bool facbegsys[symnum];
int cc, ll;      //getch使用的计数器，cc表示当前字符(ch)的位置
char a[al + 1];
char id[al + 1];
int num;     //当前number
instruction code[100];
int cx1[255];         //记录程序中初始在table表没有找到的函数
int wikind;   //while:1 if:2 else:3
int cx2[255];    //记下JPC在code中的位置,用数组是为了有多层jpc跳转时（多层while、if）  也用于生成jmp用
int cx3;   //记录用到cal的地方
int bcount;   //用于进行{}匹配
int lbracenum[255];  //记录目前是第几个左{括号 从小往大
int rbracenum[255];  //记录目前是第几个左}括号 从大往小（从{个数开始递减）
int l, r;  //分别作为{ }的标记
int varnum;   //统计每个过程变量个数
int tablenum;   //记录table表当前的位置
char idfun[255][al + 1];  //记录下刚开始没有找到的函数
int ii;   //记录刚开始没有找到的函数的个数
bool hasfun;   //是否有函数调用 判断此时的table表里面是否存在该函数
int ifcx[255], ifendcx[255];  //记录if相关的位置
int elsecx;
int kk[255];   //标记程序中不同位置的if while
int kc;  //kk数组的下标


#define getsymdo if(-1==getsym()) return -1
#define getchdo if(-1==getch())return -1
#define testdo(a,b,c) if(-1==test(a,b,c))return -1
#define gendo(a,b,c) if(-1==gen(a,b,c))return -1
#define expressiondo(a,b,c) if(-1==expression(a,b,c))return -1
#define factordo(a,b,c) if(-1==factor(a,b,c))return -1
#define termdo(a,b,c) if(-1==term(a,b,c))return -1
#define conditiondo(a,b,c) if(-1==condition(a,b,c))return -1
#define statementdo(a,b,c) if(-1==statement(a,b,c))return -1
#define constdeclarationdo(a,b,c) if(-1==constdeclaration(a,b,c))return -1
#define vardeclarationdo(a,b,c) if(-1==vardeclaration(a,b,c))return -1

void init();
void enter(enum object k, int* ptx, int lev, int* pdx);
int inset(int e, bool *s);
/*int addset(bool *sr, bool *s1, bool *s2, int n);
int subset(bool *sr, bool *s1, bool *s2, int n);
int mulset(bool *sr, bool *s1, bool *s2, int n);*/
void listcode(int cx0);
int getch();
int getsym();
int block(int lev, int tx, bool *fsys);
int gen(enum fct x, int y, int z);
int position(char* idt, int tx);
int factor(bool* fsys, int* ptx, int lev);
int term(bool* fsys, int* ptx, int lev);
int expression(bool* fsys, int* ptx, int lev);
int statement(bool* fsys, int* ptx, int lev);
int condition(bool* fsys, int * ptx, int lev);
void error(int n);
int vardeclaration(int *ptx, int lev, int *pdx);
void interpret();
int base(int l, int b);
void translate(char *s2, int ll, int aa, int i);
void intercall();