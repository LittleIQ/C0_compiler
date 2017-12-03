#include "C0.h"
#include<iostream>
#include<fstream>
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
using namespace std;


/*��ʼ��*//*init*/
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
	/*���õ��ַ�����*/
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

	/*���ñ��������֣�������ĸ˳���۰����*/
	strcpy(&word[0][0], "else");
	strcpy(&word[1][0], "if");
	strcpy(&word[2][0], "int");
	strcpy(&word[3][0], "printf");
	strcpy(&word[4][0], "return");
	strcpy(&word[5][0], "scanf");
	strcpy(&word[6][0], "void");
	strcpy(&word[7][0], "while");

	/*���ñ����ַ���*/
	wsym[0] = elsesym;
	wsym[1] = ifsym;
	wsym[2] = intsym;
	wsym[3] = printfsym;
	wsym[4] = returnsym;
	wsym[5] = scanfsym;
	wsym[6] = voidsym;
	wsym[7] = whilesym;

	/*����ָ������*/
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

	/*���÷��ż�*/
	for (i = 0;i<symnum;i++)
	{
		declbegsys[i] = false;
		statbegsys[i] = false;
		facbegsys[i] = false;
	}

	/*����������ʼ���ż�*/
	declbegsys[voidsym] = true;
	declbegsys[intsym] = true;
	declbegsys[lbrace] = true;


	/*������俪ʼ���ż�*/
	statbegsys[ifsym] = true;
	statbegsys[whilesym] = true;
	statbegsys[voidsym] = true;
	statbegsys[intsym] = true;
	statbegsys[scanfsym] = true;
	statbegsys[printfsym] = true;

	/*�������ӿ�ʼ���ż�*/
	facbegsys[ident] = true;
	facbegsys[number] = true;
	facbegsys[lparen] = true;
}

//�����ֱ��м���һ��
/*enter*/
void enter(enum object k, int* ptx, int lev, int* pdx)      //����ָ���ú�����������´�ʹ��ʱ�����ϴε�ֵ
{

	(*ptx)++;
	strcpy(table[(*ptx)].name, id);
	table[(*ptx)].kind = k;
	switch (k)
	{
	case variable:     //����
		table[(*ptx)].level = lev;
		table[(*ptx)].adr = (*pdx);
		(*pdx)++;
		varnum++;
		break;
	case void_C0:          //����
		table[(*ptx)].level = lev;
		table[(*ptx)].adr = cx;        //���̵�ַ==��ǰָ���ֵ
		table[(*ptx)].size = 0;    //��ʼĬ��Ϊ0
		break;
	}
}

/*������ʵ�ּ��ϵļ��ϵļ�������*/
/*inset*/
int inset(int e, bool *s)     //�ж�һ�����ų����Ƿ���s��  �����bool����ѭ������
{
	return s[e];
}


/*
��������嵥
*/
/*listcode*/
void listcode(int cx0)
{
	int i;
	if (listswitch&&err == 0)
	{
		printf("���������code:\n");
		for (i = cx0;i<cx;i++)
		{
			//code[i].f����ָ�볣��   ��mnemonic����װ�Ŷ�Ӧ������ֵ
			printf("(%d) %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
			fprintf(fa, "(%d) %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
		}
	}
}

/*
©���ո񣬶�ȡһ���ַ�
ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ��
������getsym����
*/
/*getch*/
int getch()
{
	if (cc == ll)     //cc=ll ��lineȡ����ڶ�һ��
	{

		ll = 0;
		cc = 0;
		ch = ' ';
		while (ch != 10)          //�س�
		{
			if (EOF == fscanf(fin, "%c", &ch))
			{
				line[ll] = 0;
				break;
			}
			printf("%c", ch);
			fprintf(fa1, "%c", ch);         //��Դ��������ַ���fa1�ļ������
			line[ll] = ch;
			ll++;
		}
	}
	ch = line[cc];
	cc++;
	return 0;
}

/*
�ʷ���������ȡһ������
*/
/*getsym*/
int getsym()
{
	int i, j, k;
	while (ch == ' ' || ch == 10 || ch == 9)      /*���Կո񡢻��к�TAB*/
	{
		getchdo;      //ȡ�ַ�
	}
	if (ch >= 'a' && ch <= 'z')
	{                                      /*���ֻ�������a.z��ͷ*/
		k = 0;
		do {
			if (k<al)
			{
				a[k] = ch;
				k++;
			}
			getchdo;
		} while (ch >= 'a'&&ch <= 'z' || ch >= '0'&&ch <= '9');
		a[k] = 0;         //�����������ַ������
		strcpy(id, a);
		i = 0;
		j = norw - 1;
		do {                                    /*������ǰ�����Ƿ�Ϊ������*/
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
			sym = ident;       //����ʧ�ܣ�Ϊ���ֻ�����
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
				error(15);    //���������������󳤶�
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

//�����������
int block(int lev, int tx, bool *fsys)
{
	int i;
	int dx;
	int tx0;
	bool nxtlev[symnum];        //������ż�
	gendo(JMP, 0, 0);             //Ĭ������1����Ҫ�޸�Ϊ��������ַ
loop:dx = 3;                    //����һ���µĺ���ʱ��ת����
	tx0 = tx;            //��¼�������ֵĳ�ʼλ��
	do {
		if (sym == lbrace)    //{
			getsymdo;
		if (sym == rbrace)    //}
			getsymdo;
		if (sym == intsym)      //�յ�int����
		{
			getsymdo;        //��ȡһ������,�������ַ���
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
					error(3);       //ȱ�ֺ�
				}
			}
			else
			{
				error(4);   //���Ǳ���
			}
		}
		while (sym == voidsym)     //�յ������������ţ���ʼ�������
		{
			getsymdo;
			if (sym == ident)
			{
				if (strcmp(id, "main") == 0)
					code[0].a = cx;             //�޸ĵ�1��ָ���aֵ  ��ת��main������
				enter(void_C0, &tx, lev, &dx);    //��ӹ�����void_c0

				int j = 0;
				if (hasfun)
				{
					for (int k = 0;k<255;k++)
					{
						j = position(idfun[k], tx);   //ÿ�����������ֱ�������ֱ��������ǰ��û���ҵ��Ĺ�����
						if (j != 0)
						{
							code[cx1[k] - 1].a = table[j].adr;  //�ҵ����޸�֮ǰĬ�ϵ�cal��a��=0�� �޸�Ϊ��ȷ��aֵ
							j = 0;
						}
					}
					hasfun = false;
				}
				getsymdo;
			}
			else
			{
				error(5);      //������Ϊ����������
			}
			if (sym == lparen)
			{
				getsymdo;
			}
			else
			{
				error(1);       //��������Ӧ������û��������
			}
			if (sym == rparen)
			{
				getsymdo;
			}
			else
			{
				error(2);       //�����ź�Ӧ��������ʧ������
			}

			if (sym == semicolon)
			{
				getsymdo;
			}
		}
	} while (inset(sym, declbegsys));  //ֱ��û����������  sym�����Ƿ���������ʼ���ż�declbegsys����

	gendo(INT, 0, dx);      //���ɷ����ڴ����

	while (inset(sym, statbegsys) || sym == semicolon || sym == rbrace || sym == ident)       //һֱ������䴦��ֱ��������䴦����
	{
		if (sym == semicolon || sym == rbrace)
		{
			getsymdo;
			/*����������Ƕ��{}ʱ�������������溯�����{},����}����ת��{֮ǰ*/
			if ((sym == rbrace) && bcount>0)
			{
				wikind = kk[kc];   //����kk���鼰��kc�±�ȷ����ǰ����һ��{}����
				if (wikind == 1)   //while  }����һ��ʱ
				{
					gendo(JMP, 0, cx2[bcount + 1] - 1);    //ͨ��֮ǰ��¼��cx2�����ֵ������JMPָ��
					code[cx2[bcount + 1]].a = cx;    //�޸�֮ǰ��ȷ����JPCӦ����ת��λ��    bcount+1ָ������ڵ�{����
				}
				if (wikind == 2)   // if }����һ��ʱ
				{
					if ((lbracenum[r] == 1) && (rbracenum[r] == 1))   //{��}ƥ��ʱ
					{
						elsecx = cx;   //else�����ĵط�
									   //gendo(JMP,0,0);    //ͨ��֮ǰ��¼��cx2�����ֵ������JMPָ��
						code[ifcx[l + 1]].a = cx;    //�޸�֮ǰ��ȷ����JPCӦ����ת��λ��
						r--;
					}
				}
				if (wikind == 3)  //else
				{
					code[elsecx].a = cx;    //�޸�JMP �����䴦����else
					wikind = 2;  //�������ж��else ��wikind���͸�Ϊif����  �����޸�������if��jpc��ת

				}
			}


		}
		if (bcount == 0)
		{
			table[++tablenum].size = varnum + 3;         //�����̵Ĵ�С ǰ3�� �ӱ�������
			tablenum += varnum;    //table��ǰλ�ã������޸ĺ����ĵ�ַ
			varnum = 0;


			getsymdo;
			if (sym == voidsym)
			{
				gendo(RET, 0, 0);     //һ����������ʱ������һ��RET 0 0
				goto loop;
			}
		}
		/*else
		{
		error(1);
		}*/
		statementdo(nxtlev, &tx, lev);    //��䴦��
	}
	gendo(RET, 0, 0);

	memset(nxtlev, 0, sizeof(bool)* symnum);
	if (tableswitch&&err == 0)        //������ֱ�
	{
		printf("\ntable���ֱ�:\n");
		if (tx0 + 1>tx)
		{
			printf("NULL\n");
		}
		for (i = 1;i <= tx;i++)    //tx  table����
		{
			switch (table[i].kind)
			{
			case variable:
				printf(" %d int %4s ", i, table[i].name);
				printf(" lev=%d  addr=%d\n", table[i].level, table[i].adr);
				//������ֱ�
				fprintf(fas, " %d var %4s ", i, table[i].name);
				fprintf(fas, " lev=%d  addr=%d\n", table[i].level, table[i].adr);
				break;
			case void_C0:
				printf(" %d void %4s", i, table[i].name);
				printf(" lev=%d  addr=%d  size=%d\n", table[i].level, table[i].adr, table[i].size);
				//������ֱ�
				fprintf(fas, " %d void %s ", i, table[i].name);
				fprintf(fas, " lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
				break;
			}
		}
		printf("\n");
	}
	listcode(0);            //������������
	return 0;
}
//�������������
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
	strcpy(table[0].name, idt);     //������������ѭ��
	i = tx;
	while (strcmp(table[i].name, idt) != 0)
	{
		i--;
	}
	return i;
}
int factor(bool* fsys, int* ptx, int lev)   //���Ӵ���
{
	int i;
	bool nxtlev[symnum];
	while (inset(sym, facbegsys))    //ѭ��ֱ���������ӿ�ʼ����
	{
		if (sym == ident)
		{
			i = position(id, *ptx);
			if (i == 0)
			{
				error(6);   //�����ڸñ�ʶ��
			}
			else
			{
				switch (table[i].kind)
				{
				case variable:
					gendo(LOD, table[i].level, table[i].adr);
					break;
				case void_C0:
					error(7);   //������Ϊ����
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
					error(8);    //��ǰ���ֳ����˹涨����
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
int term(bool* fsys, int* ptx, int lev)   //���
{
	enum symbol mulop;
	bool nxtlev[symnum];
	factordo(nxtlev, ptx, lev);
	while (sym == times || sym == slash)
	{
		mulop = sym;   //���������
		getsymdo;
		factordo(nxtlev, ptx, lev);
		if (mulop == times)
		{
			gendo(MUL, 0, 0);    //�˷�
		}
		else
		{
			gendo(DIV, 0, 0);    //����
		}
	}
	return 0;
}

//���ʽ����
int expression(bool* fsys, int* ptx, int lev)
{
	enum symbol addop;
	bool nxtlev[symnum];
	if (sym == plussym || sym == minussym)
	{
		addop = sym;    //���濪ͷ��������
		getsymdo;
		termdo(nxtlev, ptx, lev);
	}
	else     //��ʱ���ʽ��������ļӼ�
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
			gendo(ADD, 0, 0);   //���ɼӷ�ָ��
		}
		else
		{
			gendo(SUB, 0, 0);   //���ɼ���ָ��
		}
	}
	return 0;
}
//��䴦��
int statement(bool* fsys, int* ptx, int lev)
{
	int i;
	bool nxtlev[symnum];
	if (sym == ident)    //��ʶ��
	{
		i = position(id, *ptx);
		if (i == 0)    //�����Ǳ������ߺ�������ʱ�ĺ����� (��ʼδ�ҵ������ú���)
		{
			hasfun = true;            //����hasfunΪtrue��˵���ú�������ʱ��table������û�ҵ�
			strcpy(idfun[ii], id);
			getsymdo;
			if (sym == semicolon)
				error(6);         //����δ�ҵ�
			else
			{
				if (sym != lparen)   //��ʶ������Ϊ��ʱ��˵���Ǻ�������
					error(1);    //ȱ�������ţ�
				else
					getsymdo;
				if (sym != rparen)
					error(2);   //ȱ�������ţ�
				else
					getsymdo;
				if (sym != semicolon)
					error(3);  //ȱ�ٷֺ�;
				else         //�������ò�����
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
				error(9);       //����ʽ����,��ͷΪ���������Ǻ�����
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
					error(10);           //û�м�⵽��ֵ����
				}
				expressiondo(nxtlev, ptx, lev);          //���ʽ����
				if (i != 0)
				{
					gendo(STO, table[i].level, table[i].adr);
				}
			}
			else if (table[i].kind == void_C0)     //�к�������ʱ
			{
				getsymdo;

				if (sym != lparen)   //ƥ��(
					error(1);    //ȱ��(����
				else
					getsymdo;
				if (sym != rparen)   //ƥ��)
					error(2);    //ȱ��)����
				else
					getsymdo;
				//	cx3=cx;       //����cal��λ��
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
					error(6);    //��ʶ��δ����
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
						error(3);    //ȱ�ٷֺ�
				}
			}
			else
			{
				if (sym == void_C0)        //���ú���
				{
					getsymdo;
					if (sym != ident)
					{
						error(11);         //���Ǳ�ʶ��
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
						gendo(JPC, 0, 0);   //if(����)  ����Ϊ0ʱ��ת��else��ifβ
						if (sym == lbrace)
							getsymdo;
						if (sym == rbrace)
							getsymdo;
						statementdo(fsys, ptx, lev);
					}
					else if (sym == elsesym)
					{
						gendo(JMP, 0, 0);    //ͨ��֮ǰ��¼��cx2�����ֵ������JMPָ��

						code[ifcx[l + 1]].a = cx;   //�޸�if�г�ʼjpc��ֵ
						wikind = 3;
						//	gendo(JMP,0,0);        //if�Ľ�β����JMP��ת����Ŀ����ִ��ʱ����else
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
							cx2[bcount] = cx;                //����JPC��code�е�λ��
							gendo(JPC, 0, 0);        //���ڸտ�ʼ��֪��jpc��ת��λ�ã���ʼ���Ϊ0
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
//��������
int condition(bool* fsys, int* ptx, int lev)
{
	expressiondo(fsys, ptx, lev);
	return 0;
}

//������
int vardeclaration(int *ptx, int lev, int *pdx)  //������������
{
	if (sym == ident)    //����ָ����
	{
		enter(variable, ptx, lev, pdx);  //��д���ֱ�
		getsymdo;
	}
	else		    	//���Ǳ���ʱ����
	{
		error(1);
	}
	return 0;
}

//���Ե�ǰ�����Ƿ�Ϸ�
//s1 ��Ҫ�ķ���   s2 ������������Ҫ�ķ��ţ�����Ҫһ�������õļ���
//n �����
int test(bool *s1, bool *s2, int n)
{
	if (!inset(sym, s1))
	{
		error(n);
		//����ⲻͨ��ʱ����ͣ�ػ�ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ���
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
	case 1:strcpy(errors, "ȱ��������");
		break;
	case 2:strcpy(errors, "ȱ��������");
		break;
	case 3:strcpy(errors, "ȱ�ٷֺ�");
		break;
	case 4:strcpy(errors, "���Ǳ���");
		break;
	case 5:strcpy(errors, "������Ϊ����������");
		break;
	case 6:strcpy(errors, "�ñ�ʶ��δ����");
		break;
	case 7:strcpy(errors, "������Ϊ������");
		break;
	case 8:strcpy(errors, "��ǰ���ֳ����˹涨����");
		break;
	case 9:strcpy(errors, "����ʽ����,��ͷӦΪ���������Ǻ�����");
		break;
	case 10:strcpy(errors, "ȱ�ٸ�ֵ����");
		break;
	case 11:strcpy(errors, "���Ǳ�ʶ��");
		break;
	case 12:strcpy(errors, "���Ǳ���");
		break;
	case 13:strcpy(errors, "ȱ���������");
		break;
	case 14:strcpy(errors, "ȱ���Ҵ�����");
		break;
	case 15:strcpy(errors, "���������������󳤶�");
		break;
	}
	err++;
	printf("%s\n", errors);
}
//���ͳ��򲿷�
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
		case LOD:             //ȡ��Ե�ǰ���̵����ݻ���ַΪa���ڴ��ֵ��ջ��
			s[t] = s[base(i.l, b) + i.a];
			t++;
			break;
		case STO:          //ջ����ֵ�浽��Ե�ǰ���̵����ݻ���ַΪa���ڴ�
			t--;
			s[base(i.l, b) + i.a] = s[t];
			break;
		case CAL:                    //***�����õĺ����ݲ�֧�ִ������ͷ���ֵ***
			s[t] = base(i.l, b);        //��ǰ��ַ��ջ
			s[t + 1] = b;                  //�����̻�ַ��ջ  DL    (��s[t]һ��)
			s[t + 2] = p;                  //��ǰָ���ַ��ջ RA(���ص�ַ)  ���жϳ����Ƿ����ʱ������
			b = t;            //�»�ַ
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
			p = s[t + 2];       //��t�ҵ����ú���ǰ��pֵ     Ҫ���صĵ�ַΪ0ʱ�������
			b = s[t + 1];
			break;

		}
	} while (p != 0);
}
/*-translate*/
void translate(char *s2, int ll, int aa, int i)  //�����ȡ���ı�����
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

	cout << "*****����:*****" << endl;
	ifstream in("fa.tmp", ios::in);
	if (!in)
	{
		cerr << "�ļ����ܴ򿪣�";
		return;
	}
	char s1[10], s2[10];
	int ll, aa;
	int i = 0;     //��ǰָ���ַ
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
	cout << "����C0����:" << endl;
	cin >> fname;
	fin = fopen(fname, "r");
	//	fin=fopen("7.txt","r");
	fin = fopen(fname, "r");
	if (fin)
	{
		cout << "�Ƿ��ӡԴ����?(Y/N)" << endl;
		cin >> fname;
		listswitch = (fname[0] == 'y' || fname[0] == 'Y');
		//	listswitch=true;
		cout << "�Ƿ��ӡ���ű�?(Y/N)" << endl;
		cin >> fname;
		tableswitch = (fname[0] == 'y' || fname[0] == 'Y');
		//	tableswitch=true;
		fa1 = fopen("fa1.tmp", "w");
		fprintf(fa1, "��ӡC0����?");
		fprintf(fa1, "%s\n", fname);
		init();
		err = 0;
		cc = cx = ll = 0;
		ch = ' ';
		if (-1 != getsym())
		{
			fa = fopen("fa.tmp", "w");   //���Ŀ�����
			fas = fopen("fas.tmp", "w");   //������ֱ�

			if (-1 == block(0, 0, nxtlev))    //���ñ������    ����
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
			if (err == 0)   //û�д���
			{
				fa2 = fopen("fa2.tmp", "w");
				intercall();    //���ý��ͳ���
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