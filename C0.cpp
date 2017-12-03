#include "C0.h"
#include<iostream>
#include<fstream>
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
using namespace std;


/*初始化*//*init*/
void init()
{
	bcount = 0;
	varnum = 0;   //
	tablenum = 0;
	wikind = 0;
	hasfun = false;
	l = r = 0;
	kc = 0;
	int i;
	/*设置单字符符号*/
	for (i = 0;i <= 255;i++)
	{
		ssym[i] = nul;
	}
	ssym['+'] = plussym;
	ssym['-'] = minussym;
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym[','] = comma;
	ssym[';'] = semicolon;
	ssym['{'] = lbrace;
	ssym['}'] = rbrace;

	//***************************************************

	/*设置保留字名字，按照字母顺序，折半查找*/
	strcpy(&word[0][0], "else");
	strcpy(&word[1][0], "if");
	strcpy(&word[2][0], "int");
	strcpy(&word[3][0], "printf");
	strcpy(&word[4][0], "return");
	strcpy(&word[5][0], "scanf");
	strcpy(&word[6][0], "void");
	strcpy(&word[7][0], "while");

	/*设置保留字符号*/
	wsym[0] = elsesym;
	wsym[1] = ifsym;
	wsym[2] = intsym;
	wsym[3] = printfsym;
	wsym[4] = returnsym;
	wsym[5] = scanfsym;
	wsym[6] = voidsym;
	wsym[7] = whilesym;

	/*设置指令名称*/
	strcpy(&(mnemonic[LIT][0]), "LIT");
	strcpy(&(mnemonic[LOD][0]), "LOD");
	strcpy(&(mnemonic[STO][0]), "STO");
	strcpy(&(mnemonic[CAL][0]), "CAL");
	strcpy(&(mnemonic[INT][0]), "INT");
	strcpy(&(mnemonic[JMP][0]), "JMP");
	strcpy(&(mnemonic[JPC][0]), "JPC");
	strcpy(&(mnemonic[ADD][0]), "ADD");
	strcpy(&(mnemonic[SUB][0]), "SUB");
	strcpy(&(mnemonic[MUL][0]), "MUL");
	strcpy(&(mnemonic[DIV][0]), "DIV");
	strcpy(&(mnemonic[RED][0]), "RED");
	strcpy(&(mnemonic[WRT][0]), "WRT");
	strcpy(&(mnemonic[RET][0]), "RET");

	/*设置符号集*/
	for (i = 0;i<symnum;i++)
	{
		declbegsys[i] = false;
		statbegsys[i] = false;
		facbegsys[i] = false;
	}

	/*设置声明开始符号集*/
	declbegsys[voidsym] = true;
	declbegsys[intsym] = true;
	declbegsys[lbrace] = true;


	/*设置语句开始符号集*/
	statbegsys[ifsym] = true;
	statbegsys[whilesym] = true;
	statbegsys[voidsym] = true;
	statbegsys[intsym] = true;
	statbegsys[scanfsym] = true;
	statbegsys[printfsym] = true;

	/*设置因子开始符号集*/
	facbegsys[ident] = true;
	facbegsys[number] = true;
	facbegsys[lparen] = true;
}

//在名字表中加入一项
/*enter*/
void enter(enum object k, int* ptx, int lev, int* pdx)      //这里指针用后变量可以在下次使用时依赖上次的值
{

	(*ptx)++;
	strcpy(table[(*ptx)].name, id);
	table[(*ptx)].kind = k;
	switch (k)
	{
	case variable:     //变量
		table[(*ptx)].level = lev;
		table[(*ptx)].adr = (*pdx);
		(*pdx)++;
		varnum++;
		break;
	case void_C0:          //过程
		table[(*ptx)].level = lev;
		table[(*ptx)].adr = cx;        //过程地址==当前指针的值
		table[(*ptx)].size = 0;    //初始默认为0
		break;
	}
}

/*用数组实现集合的集合的集合运算*/
/*inset*/
int inset(int e, bool *s)     //判断一个符号常量是否在s中  定义成bool便于循环处理
{
	return s[e];
}


/*
输出代码清单
*/
/*listcode*/
void listcode(int cx0)
{
	int i;
	if (listswitch&&err == 0)
	{
		printf("虚拟机代码code:\n");
		for (i = cx0;i<cx;i++)
		{
			//code[i].f代表指针常量   而mnemonic里面装着对应常量的值
			printf("(%d) %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
			fprintf(fa, "(%d) %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
		}
	}
}

/*
漏掉空格，读取一个字符
每次读一行，存入line缓冲区，line被getsym取空后再读一行
被函数getsym调用
*/
/*getch*/
int getch()
{
	if (cc == ll)     //cc=ll 即line取完后在读一行
	{

		ll = 0;
		cc = 0;
		ch = ' ';
		while (ch != 10)          //回车
		{
			if (EOF == fscanf(fin, "%c", &ch))
			{
				line[ll] = 0;
				break;
			}
			printf("%c", ch);
			fprintf(fa1, "%c", ch);         //将源代码逐个字符往fa1文件里面放
			line[ll] = ch;
			ll++;
		}
	}
	ch = line[cc];
	cc++;
	return 0;
}

/*
词法分析，获取一个符号
*/
/*getsym*/
int getsym()
{
	int i, j, k;
	while (ch == ' ' || ch == 10 || ch == 9)      /*忽略空格、换行和TAB*/
	{
		getchdo;      //取字符
	}
	if (ch >= 'a' && ch <= 'z')
	{                                      /*名字或保留字以a.z开头*/
		k = 0;
		do {
			if (k<al)
			{
				a[k] = ch;
				k++;
			}
			getchdo;
		} while (ch >= 'a'&&ch <= 'z' || ch >= '0'&&ch <= '9');
		a[k] = 0;         //将后面多余的字符清除掉
		strcpy(id, a);
		i = 0;
		j = norw - 1;
		do {                                    /*搜索当前符号是否为保留字*/
			k = (i + j) / 2;
			if (strcmp(id, word[k]) <= 0)
			{
				j = k - 1;
			}
			if (strcmp(id, word[k]) >= 0)
			{
				i = k + 1;
			}
		} while (i <= j);
		if (i - 1 > j)
		{
			sym = wsym[k];
		}
		else
		{
			sym = ident;       //搜索失败，为名字或数字
		}
	}
	else
	{
		if (ch >= '0' && ch <= '9')
		{
			k = 0;
			num = 0;
			sym = number;
			do {
				num = 10 * num + ch - '0';
				k++;
				getchdo;
			} while (ch >= '0'&&ch <= '9');
			k--;
			if (k>nmax)
			{
				error(15);    //超出数字允许的最大长度
			}
		}
		else
		{
				if (ch == '=')
				{
					sym = becomes;
					getchdo;
				}
				else
				{
					sym = nul;
				}


			if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')'
				|| ch == ',' /*|| ch == '#'*/ || ch == ';' || ch == '{' || ch == '}')
			{
				sym = ssym[ch];
				getchdo;

			}
		}
	}

	if (sym == lbrace)
	{
		bcount++;
		if (wikind == 2)
		{
			lbracenum[++l] = 1;
			++r;
			kk[kc++] = 2;
		}
		if (wikind == 1)
		{
			kk[kc++] = 1;
		}
		if (wikind == 3)
		{
			kk[kc++] = 3;
		}
	}
	else if (sym == rbrace)
	{
		bcount--;
		if (wikind == 2)
		{
			rbracenum[l--] = 1;
		}
		kc--;
	}

	return 0;
}

//编译程序主体
int block(int lev, int tx, bool *fsys)
{
	int i;
	int dx;
	int tx0;
	bool nxtlev[symnum];        //后跟符号集
	gendo(JMP, 0, 0);             //默认跳到1，需要修改为主函数地址
loop:dx = 3;                    //遇到一个新的函数时跳转到此
	tx0 = tx;            //记录本层名字的初始位置
	do {
		if (sym == lbrace)    //{
			getsymdo;
		if (sym == rbrace)    //}
			getsymdo;
		if (sym == intsym)      //收到int符号
		{
			getsymdo;        //再取一个符号,即名字字符。
			if (sym == ident)
			{
				vardeclarationdo(&tx, lev, &dx);
				while (sym == comma)
				{
					getsymdo;
					vardeclarationdo(&tx, lev, &dx);
				}
				if (sym == semicolon)
				{
					getsymdo;
				}
				else
				{
					error(3);       //缺分号
				}
			}
			else
			{
				error(4);   //不是变量
			}
		}
		while (sym == voidsym)     //收到过程声明符号，开始处理过程
		{
			getsymdo;
			if (sym == ident)
			{
				if (strcmp(id, "main") == 0)
					code[0].a = cx;             //修改第1条指令的a值  跳转到main函数处
				enter(void_C0, &tx, lev, &dx);    //添加过程名void_c0

				int j = 0;
				if (hasfun)
				{
					for (int k = 0;k<255;k++)
					{
						j = position(idfun[k], tx);   //每个函数入名字表后，在名字表里面查找前面没有找到的过程名
						if (j != 0)
						{
							code[cx1[k] - 1].a = table[j].adr;  //找到后修改之前默认的cal的a（=0） 修改为正确的a值
							j = 0;
						}
					}
					hasfun = false;
				}
				getsymdo;
			}
			else
			{
				error(5);      //不能作为函数的名字
			}
			if (sym == lparen)
			{
				getsymdo;
			}
			else
			{
				error(1);       //过程名后应跟（，没有左括号
			}
			if (sym == rparen)
			{
				getsymdo;
			}
			else
			{
				error(2);       //左括号后应跟），丢失右括号
			}

			if (sym == semicolon)
			{
				getsymdo;
			}
		}
	} while (inset(sym, declbegsys));  //直到没有声明符号  sym符号是否在声明开始符号集declbegsys里面

	gendo(INT, 0, dx);      //生成分配内存代码

	while (inset(sym, statbegsys) || sym == semicolon || sym == rbrace || sym == ident)       //一直进行语句处理，直到所有语句处理完
	{
		if (sym == semicolon || sym == rbrace)
		{
			getsymdo;
			/*当函数里面嵌套{}时，不包括最外面函数体的{},遇到}后，跳转到{之前*/
			if ((sym == rbrace) && bcount>0)
			{
				wikind = kk[kc];   //根据kk数组及其kc下标确定当前在那一层{}里面
				if (wikind == 1)   //while  }结束一层时
				{
					gendo(JMP, 0, cx2[bcount + 1] - 1);    //通过之前记录的cx2数组的值来生成JMP指令
					code[cx2[bcount + 1]].a = cx;    //修改之前不确定的JPC应该跳转的位置    bcount+1指上面对于的{部分
				}
				if (wikind == 2)   // if }结束一层时
				{
					if ((lbracenum[r] == 1) && (rbracenum[r] == 1))   //{与}匹配时
					{
						elsecx = cx;   //else结束的地方
									   //gendo(JMP,0,0);    //通过之前记录的cx2数组的值来生成JMP指令
						code[ifcx[l + 1]].a = cx;    //修改之前不确定的JPC应该跳转的位置
						r--;
					}
				}
				if (wikind == 3)  //else
				{
					code[elsecx].a = cx;    //修改JMP 详见语句处理函数else
					wikind = 2;  //不可能有多个else 将wikind类型改为if类型  即可修改最外层的if的jpc跳转

				}
			}


		}
		if (bcount == 0)
		{
			table[++tablenum].size = varnum + 3;         //本过程的大小 前3个 加变量个数
			tablenum += varnum;    //table表当前位置，方便修改函数的地址
			varnum = 0;


			getsymdo;
			if (sym == voidsym)
			{
				gendo(RET, 0, 0);     //一个函数结束时，生成一个RET 0 0
				goto loop;
			}
		}
		/*else
		{
		error(1);
		}*/
		statementdo(nxtlev, &tx, lev);    //语句处理
	}
	gendo(RET, 0, 0);

	memset(nxtlev, 0, sizeof(bool)* symnum);
	if (tableswitch&&err == 0)        //输出名字表
	{
		printf("\ntable名字表:\n");
		if (tx0 + 1>tx)
		{
			printf("NULL\n");
		}
		for (i = 1;i <= tx;i++)    //tx  table长度
		{
			switch (table[i].kind)
			{
			case variable:
				printf(" %d int %4s ", i, table[i].name);
				printf(" lev=%d  addr=%d\n", table[i].level, table[i].adr);
				//输出名字表
				fprintf(fas, " %d var %4s ", i, table[i].name);
				fprintf(fas, " lev=%d  addr=%d\n", table[i].level, table[i].adr);
				break;
			case void_C0:
				printf(" %d void %4s", i, table[i].name);
				printf(" lev=%d  addr=%d  size=%d\n", table[i].level, table[i].adr, table[i].size);
				//输出名字表
				fprintf(fas, " %d void %s ", i, table[i].name);
				fprintf(fas, " lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
				break;
			}
		}
		printf("\n");
	}
	listcode(0);            //输出虚拟机代码
	return 0;
}
//生成虚拟机代码
int gen(enum fct x, int y, int z)
{
	if (cx >= cxmax)
	{
		printf("Program too long");
		return -1;
	}
	code[cx].f = x;
	code[cx].l = y;
	code[cx].a = z;
	cx++;
	return 0;
}
int position(char* idt, int tx)
{
	int i;
	strcpy(table[0].name, idt);     //方便结束下面的循环
	i = tx;
	while (strcmp(table[i].name, idt) != 0)
	{
		i--;
	}
	return i;
}
int factor(bool* fsys, int* ptx, int lev)   //因子处理
{
	int i;
	bool nxtlev[symnum];
	while (inset(sym, facbegsys))    //循环直到不是因子开始符号
	{
		if (sym == ident)
		{
			i = position(id, *ptx);
			if (i == 0)
			{
				error(6);   //不存在该标识符
			}
			else
			{
				switch (table[i].kind)
				{
				case variable:
					gendo(LOD, table[i].level, table[i].adr);
					break;
				case void_C0:
					error(7);   //不能作为过程
					break;
				}
			}
			getsymdo;
		}
		else
		{
			if (sym == number)
			{
				if (num>amax)
				{
					error(8);    //当前数字超过了规定上限
					num = 0;
				}
				gendo(LIT, 0, num);
				getsymdo;
			}
			else
			{
				if (sym == lparen)
				{
					getsymdo;
					expressiondo(nxtlev, ptx, lev);
					if (sym == rparen)
					{
						getsymdo;
					}
					else
					{
						error(1);
					}
				}
			}
		}
	}
	return 0;
}
int term(bool* fsys, int* ptx, int lev)   //项处理
{
	enum symbol mulop;
	bool nxtlev[symnum];
	factordo(nxtlev, ptx, lev);
	while (sym == times || sym == slash)
	{
		mulop = sym;   //保存操作符
		getsymdo;
		factordo(nxtlev, ptx, lev);
		if (mulop == times)
		{
			gendo(MUL, 0, 0);    //乘法
		}
		else
		{
			gendo(DIV, 0, 0);    //除法
		}
	}
	return 0;
}

//表达式处理
int expression(bool* fsys, int* ptx, int lev)
{
	enum symbol addop;
	bool nxtlev[symnum];
	if (sym == plussym || sym == minussym)
	{
		addop = sym;    //保存开头的正负号
		getsymdo;
		termdo(nxtlev, ptx, lev);
	}
	else     //此时表达式被看作项的加减
	{
		termdo(nxtlev, ptx, lev);
	}
	while (sym == plussym || sym == minussym)
	{
		addop = sym;
		getsymdo;
		termdo(nxtlev, ptx, lev);
		if (addop == plussym)
		{
			gendo(ADD, 0, 0);   //生成加法指令
		}
		else
		{
			gendo(SUB, 0, 0);   //生成减法指令
		}
	}
	return 0;
}
//语句处理
int statement(bool* fsys, int* ptx, int lev)
{
	int i;
	bool nxtlev[symnum];
	if (sym == ident)    //标识符
	{
		i = position(id, *ptx);
		if (i == 0)    //可能是变量或者函数调用时的函数名 (初始未找到被调用函数)
		{
			hasfun = true;            //设置hasfun为true，说明该函数调用时在table表里面没找到
			strcpy(idfun[ii], id);
			getsymdo;
			if (sym == semicolon)
				error(6);         //变量未找到
			else
			{
				if (sym != lparen)   //标识符后面为（时，说明是函数调用
					error(1);    //缺少左括号（
				else
					getsymdo;
				if (sym != rparen)
					error(2);   //缺少右括号）
				else
					getsymdo;
				if (sym != semicolon)
					error(3);  //缺少分号;
				else         //函数调用不出错
				{
					gen(CAL, 0, 0);
					cx1[ii] = cx;
					ii++;
				}
			}
		}
		else
		{
			if (table[i].kind != variable&&table[i].kind != void_C0)
			{
				error(9);       //语句格式错误,开头为变量或者是函数名
				i = 0;
			}
			else if (table[i].kind == variable)
			{
				getsymdo;
				if (sym == becomes)
				{
					getsymdo;
				}
				else
				{
					error(10);           //没有检测到赋值符号
				}
				expressiondo(nxtlev, ptx, lev);          //表达式处理
				if (i != 0)
				{
					gendo(STO, table[i].level, table[i].adr);
				}
			}
			else if (table[i].kind == void_C0)     //有函数调用时
			{
				getsymdo;

				if (sym != lparen)   //匹配(
					error(1);    //缺少(括号
				else
					getsymdo;
				if (sym != rparen)   //匹配)
					error(2);    //缺少)括号
				else
					getsymdo;
				//	cx3=cx;       //记下cal的位置
				gendo(CAL, 0, table[i].adr);
				getsymdo;
			}
		}
	}
	else
	{
		if (sym == scanfsym)
		{
			getsymdo;
			if (sym != lparen)
			{
				error(1);   //(
			}
			else
			{
				getsymdo;
				if (sym == ident)
				{
					i = position(id, *ptx);
				}
				else
				{
					i = 0;
				}
				if (i == 0)
				{
					error(6);    //标识符未声明
				}
				else
				{
					gendo(RED, 0, 0);
					gendo(STO, lev - table[i].level, table[i].adr);
				}
				getsymdo;
			}
			if (sym != rparen)
			{
				error(2);   //)
				while (!inset(sym, fsys))
				{
					getsymdo;
				}
			}
			else
			{
				getsymdo;
			}
		}
		else
		{
			if (sym == printfsym)
			{
				getsymdo;
				if (sym == lparen)
				{
					getsymdo;
					expressiondo(nxtlev, ptx, lev);
					gendo(WRT, 0, 0);
					if (sym != rparen)
					{
						error(2);   //)
					}
					else
					{
						getsymdo;
					}
					if (sym != semicolon)
						error(3);    //缺少分号
				}
			}
			else
			{
				if (sym == void_C0)        //调用函数
				{
					getsymdo;
					if (sym != ident)
					{
						error(11);         //不是标识符
					}
					else
					{
						i = position(id, *ptx);
						if (i == 0)
						{
							//					error(1);
						}
						else
						{
							if (table[i].kind == void_C0)
							{
								gendo(CAL, 0, table[i].adr);
							}
							else
							{
								error(1);
							}
						}
						getsymdo;
					}
				}
				else
				{
					if (sym == ifsym)
					{

						wikind = 2;
						getsymdo;

						conditiondo(nxtlev, ptx, lev);

						ifcx[l] = cx;
						gendo(JPC, 0, 0);   //if(条件)  条件为0时调转到else或if尾
						if (sym == lbrace)
							getsymdo;
						if (sym == rbrace)
							getsymdo;
						statementdo(fsys, ptx, lev);
					}
					else if (sym == elsesym)
					{
						gendo(JMP, 0, 0);    //通过之前记录的cx2数组的值来生成JMP指令

						code[ifcx[l + 1]].a = cx;   //修改if中初始jpc的值
						wikind = 3;
						//	gendo(JMP,0,0);        //if的结尾加入JMP跳转，让目标码执行时跳过else
						getsymdo;

						if (sym == lbrace)
							getsymdo;
						if (sym == rbrace)
							getsymdo;
						conditiondo(nxtlev, ptx, lev);
						statementdo(fsys, ptx, lev);
					}
					else
					{

						if (sym == whilesym)
						{
							wikind = 1;
							getsymdo;

							conditiondo(nxtlev, ptx, lev);        //
							cx2[bcount] = cx;                //记下JPC在code中的位置
							gendo(JPC, 0, 0);        //由于刚开始不知道jpc调转的位置，初始标记为0
							getsymdo;
							statementdo(fsys, ptx, lev);

						}

					}
				}
			}
		}
	}
	return 0;
}
//条件处理
int condition(bool* fsys, int* ptx, int lev)
{
	expressiondo(fsys, ptx, lev);
	return 0;
}

//出错处理
int vardeclaration(int *ptx, int lev, int *pdx)  //变量声明处理
{
	if (sym == ident)    //这里指变量
	{
		enter(variable, ptx, lev, pdx);  //填写名字表
		getsymdo;
	}
	else		    	//不是变量时出错
	{
		error(1);
	}
	return 0;
}

//测试当前符号是否合法
//s1 需要的符号   s2 若不是我们需要的符号，则需要一个补救用的集合
//n 错误号
int test(bool *s1, bool *s2, int n)
{
	if (!inset(sym, s1))
	{
		error(n);
		//当检测不通过时，不停地获取符号，直到它属于需要的集合或补救的集合
		while ((!inset(sym, s1)) && (!inset(sym, s2)))
		{
			getsymdo;
		}
	}
	return 0;
}


/*-error*/
void error(int n)
{
	char errors[100];
	switch (n)
	{
	case 1:strcpy(errors, "缺少左括号");
		break;
	case 2:strcpy(errors, "缺少右括号");
		break;
	case 3:strcpy(errors, "缺少分号");
		break;
	case 4:strcpy(errors, "不是变量");
		break;
	case 5:strcpy(errors, "不能作为函数的名字");
		break;
	case 6:strcpy(errors, "该标识符未声明");
		break;
	case 7:strcpy(errors, "不能作为过程名");
		break;
	case 8:strcpy(errors, "当前数字超过了规定上限");
		break;
	case 9:strcpy(errors, "语句格式错误,开头应为变量或者是函数名");
		break;
	case 10:strcpy(errors, "缺少赋值符号");
		break;
	case 11:strcpy(errors, "不是标识符");
		break;
	case 12:strcpy(errors, "不是变量");
		break;
	case 13:strcpy(errors, "缺少左大括号");
		break;
	case 14:strcpy(errors, "缺少右大括号");
		break;
	case 15:strcpy(errors, "超出数字允许的最大长度");
		break;
	}
	err++;
	printf("%s\n", errors);
}
//解释程序部分
int base(int l, int b)
{
	int b1;
	b1 = b;
	return b1;
}
/*-interpret*/
void interpret()
{
	int p, b, t;
	struct instruction i;
	int s[stacksize];
	t = 0;
	b = 0;
	p = 0;
	s[0] = s[1] = s[2] = 0;
	do
	{
		i = code[p];
		p++;
		switch (i.f)
		{
		case LIT:
			s[t] = i.a;
			t++;
			break;
		case LOD:             //取相对当前过程的数据基地址为a的内存的值到栈顶
			s[t] = s[base(i.l, b) + i.a];
			t++;
			break;
		case STO:          //栈顶的值存到相对当前过程的数据基地址为a的内存
			t--;
			s[base(i.l, b) + i.a] = s[t];
			break;
		case CAL:                    //***被调用的函数暂不支持带参数和返回值***
			s[t] = base(i.l, b);        //当前基址入栈
			s[t + 1] = b;                  //本过程基址入栈  DL    (和s[t]一样)
			s[t + 2] = p;                  //当前指令地址入栈 RA(返回地址)  在判断程序是否结束时起作用
			b = t;            //新基址
			p = i.a;

			break;
		case INT:
			t = t + i.a;
			break;
		case JMP:
			p = i.a;
			break;
		case JPC:
			t--;
			if (s[t] == 0)
			{
				p = i.a;
			}
			break;
			//**********
		case ADD:
			t--;
			s[t - 1] = s[t - 1] + s[t];
			break;
		case SUB:
			t--;
			s[t - 1] = s[t - 1] - s[t];
			break;
		case MUL:
			t--;
			s[t - 1] = s[t - 1] * s[t];
			break;
		case DIV:
			t--;
			if (s[t - 1] != 0)
				s[t - 1] = s[t - 1] / s[t];
			else
				cout << "input error" << endl;
			break;
		case RED:
			cout << "input:";
			cin >> s[t];
			t++;
			break;
		case WRT:
			cout << "result:" << s[t - 1] << endl;
			fprintf(fa2, "%d\n", s[t - 1]);
			t--;
			break;
		case RET:
			t = b;
			p = s[t + 2];       //由t找到调用函数前的p值     要返回的地址为0时程序结束
			b = s[t + 1];
			break;

		}
	} while (p != 0);
}
/*-translate*/
void translate(char *s2, int ll, int aa, int i)  //处理读取的文本内容
{
	if (strcmp(s2, "LIT") == 0)
	{
		code[i].f = (fct)0;
	}
	else if (strcmp(s2, "LOD") == 0)
	{
		code[i].f = (fct)1;
	}
	else if (strcmp(s2, "STO") == 0)
	{
		code[i].f = (fct)2;
	}
	else if (strcmp(s2, "CAL") == 0)
	{
		code[i].f = (fct)3;
	}
	else if (strcmp(s2, "INT") == 0)
	{
		code[i].f = (fct)4;
	}
	else if (strcmp(s2, "JMP") == 0)
	{
		code[i].f = (fct)5;
	}
	else if (strcmp(s2, "JPC") == 0)
	{
		code[i].f = (fct)6;
	}
	else if (strcmp(s2, "ADD") == 0)
	{
		code[i].f = (fct)7;
	}
	else if (strcmp(s2, "SUB") == 0)
	{
		code[i].f = (fct)8;
	}
	else if (strcmp(s2, "MUL") == 0)
	{
		code[i].f = (fct)9;
	}
	else if (strcmp(s2, "DIV") == 0)
	{
		code[i].f = (fct)10;
	}
	else if (strcmp(s2, "RED") == 0)
	{
		code[i].f = (fct)11;
	}
	else if (strcmp(s2, "WRT") == 0)
	{
		code[i].f = (fct)12;
	}
	else if (strcmp(s2, "RET") == 0)
	{
		code[i].f = (fct)13;
	}
	code[i].l = ll;
	code[i].a = aa;
}
/*-intercall*/
void intercall()
{

	cout << "*****解释:*****" << endl;
	ifstream in("fa.tmp", ios::in);
	if (!in)
	{
		cerr << "文件不能打开！";
		return;
	}
	char s1[10], s2[10];
	int ll, aa;
	int i = 0;     //当前指令地址
	while (in >> s1 >> s2 >> ll >> aa)
	{
		translate(s2, ll, aa, i);
		i++;
	}
	interpret();
}
/*-main()*/
int main()
{
	bool nxtlev[symnum];
	cout << "放入C0程序:" << endl;
	cin >> fname;
	fin = fopen(fname, "r");
	//	fin=fopen("7.txt","r");
	fin = fopen(fname, "r");
	if (fin)
	{
		cout << "是否打印源代码?(Y/N)" << endl;
		cin >> fname;
		listswitch = (fname[0] == 'y' || fname[0] == 'Y');
		//	listswitch=true;
		cout << "是否打印符号表?(Y/N)" << endl;
		cin >> fname;
		tableswitch = (fname[0] == 'y' || fname[0] == 'Y');
		//	tableswitch=true;
		fa1 = fopen("fa1.tmp", "w");
		fprintf(fa1, "打印C0程序?");
		fprintf(fa1, "%s\n", fname);
		init();
		err = 0;
		cc = cx = ll = 0;
		ch = ' ';
		if (-1 != getsym())
		{
			fa = fopen("fa.tmp", "w");   //存放目标代码
			fas = fopen("fas.tmp", "w");   //存放名字表

			if (-1 == block(0, 0, nxtlev))    //调用编译程序    出错
			{
				fclose(fa);
				fclose(fa1);
				fclose(fas);
				fclose(fin);
				printf("\n");
				return 0;
			}
			fclose(fa);
			fclose(fa1);
			fclose(fas);
			if (err == 0)   //没有错误
			{
				fa2 = fopen("fa2.tmp", "w");
				intercall();    //调用解释程序
				fclose(fa2);
			}
			else
			{
				cout << "Errors in c0 program" << endl;
			}
		}
		fclose(fin);
	}
	else
	{
		cout << "can not open the file" << endl;
	}
    system("pause");
	return 0;
}