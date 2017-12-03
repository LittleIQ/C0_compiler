/*C0zzb.h-����*/
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
#define norw 8      //�ؼ��ֵĸ���  word����
#define al 10
#define txmax 100    //���ֱ�����
#define nmax 14
#define cxmax 200
#define levmax 3
#define amax 2047   //��ַ�Ͻ�
#define stacksize 500


/*���ų���*/
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

//���ֱ��е�����
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
int err;   //���������
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
int cx;   //��ǰcode��ָ��
char line[81];
enum symbol sym;
enum symbol ssym[256];   //���ַ��ķ���ֵ
char word[norw][al];
enum symbol wsym[norw];
char mnemonic[fctnum][5];   //���������ָ������
bool declbegsys[symnum];
bool statbegsys[symnum];
bool facbegsys[symnum];
int cc, ll;      //getchʹ�õļ�������cc��ʾ��ǰ�ַ�(ch)��λ��
char a[al + 1];
char id[al + 1];
int num;     //��ǰnumber
instruction code[100];
int cx1[255];         //��¼�����г�ʼ��table��û���ҵ��ĺ���
int wikind;   //while:1 if:2 else:3
int cx2[255];    //����JPC��code�е�λ��,��������Ϊ���ж��jpc��תʱ�����while��if��  Ҳ��������jmp��
int cx3;   //��¼�õ�cal�ĵط�
int bcount;   //���ڽ���{}ƥ��
int lbracenum[255];  //��¼Ŀǰ�ǵڼ�����{���� ��С����
int rbracenum[255];  //��¼Ŀǰ�ǵڼ�����}���� �Ӵ���С����{������ʼ�ݼ���
int l, r;  //�ֱ���Ϊ{ }�ı��
int varnum;   //ͳ��ÿ�����̱�������
int tablenum;   //��¼table��ǰ��λ��
char idfun[255][al + 1];  //��¼�¸տ�ʼû���ҵ��ĺ���
int ii;   //��¼�տ�ʼû���ҵ��ĺ����ĸ���
bool hasfun;   //�Ƿ��к������� �жϴ�ʱ��table�������Ƿ���ڸú���
int ifcx[255], ifendcx[255];  //��¼if��ص�λ��
int elsecx;
int kk[255];   //��ǳ����в�ͬλ�õ�if while
int kc;  //kk������±�


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