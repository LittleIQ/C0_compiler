#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <fstream>
using namespace std;

enum fct {
	LIT, LOD, STO, CAL, INT, JMP, JPC, ADD, SUB, MUL, DIV, RED, WRT, RET
};
/*const char* list[] = {
	"lit", "lod", "sto", "cal", "int", "jmp", "jpc", "add", "sub", "mul", "div", "red", "wrt", "ret"
};*/
const char* list[] = {
	"LIT", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "ADD", "SUB", "MUL", "DIV", "RED", "WRT", "RET"
};
struct instruction {
	enum fct f;
	int l;
	int a;
};

int coNum = 0;
instruction code[100];
int stacksize = 500;
int p,b,t;       //指令指针，指令基址，栈顶指针
instruction i;
int *s;

enum fct stringToEnum(const char* x){
	enum fct ff;
	int count = sizeof(list);
	for(int i = 0;i<count;i++){
		if(strcmp(x,list[i])==0)
		{
			ff = fct(i);
			break;
		}
	}
	return ff;
}

void inCode(string url){
	ifstream in;
	string line;
	in.open(url.c_str());
	coNum = 0;
	while(getline(in,line)){
		string str1, str2, str3;
		istringstream is(line);
		is>>str1>>str2>>str3;
		code[coNum].f = stringToEnum(str1.c_str());
		code[coNum].l =  atoi(str2.c_str());
		code[coNum].a = atoi(str3.c_str());
		//cout<<code[coNum].f<<" "<<code[coNum].l<<" "<<code[coNum].a<<endl;
		coNum++;
	}
	in.close();
}

int base(int l,int *s,int b){
    int b1;
    b1 = b;
    while (l > 0){
        b1 = s[b1];
        l--;
    }
    return b1;
}

int main()
{
	t = 0;
	b = 0;
	p = 0;
	s = new int[stacksize];
	s[0] = s[1] = s[2] = 0;
	inCode("codew.txt");
    do{
		i = code[p];
		p++;
		cout<<list[i.f]<<" "<<s[t]<<" ";
		switch (i.f){
			case LIT:
				s[t] = i.a;
				t++;
				cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
				break;

			case LOD:
                s[t] = s[base(i.l,s,b) + i.a];
                t++;
                cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
                break;

            case STO:
                t--;
                s[base(i.l, s, b) + i.a] = s[t];
                cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
                break;

            case CAL:
				s[t] = base(i.l, s, b); /*将父过程基地址入栈*/
				s[t + 1] = b;           /*将本过程基地址入栈，此两项用于base函数*/
				s[t + 2] = p;           /*将当前指令指针入栈*/
				b = t;                /*改变基地址指针值为新过程的基地址*/
				p = i.a;              /*跳转*/
				cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
				break;

            case INT:
				t += i.a;
				cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
				break;

            case JMP:
				p = i.a;
				cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
				break;

            case JPC:
                t--;
                if (s[t] == 0)
                {
                    p = i.a;
                }
                cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
                break;
                
            case ADD:
				t--;
				s[t - 1] = s[t - 1] + s[t];
				cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
				break;

            case SUB:
				t--;
				s[t - 1] = s[t - 1] - s[t];
				cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
				break;

            case MUL:
				t--;
				s[t - 1] = s[t - 1] * s[t];
				cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
				break;

            case DIV:
				t--;
				if(s[t]==0){
					cout<<"栈顶元素为0！不能相除"<<endl;
				}else{
					s[t - 1] = s[t - 1] / s[t];
				}
				cout<<s[t-1]<<" "<<s[t]<<" ";
				cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
				break;

            case RED:
				t++;
				cout<<"请输入："<<endl;
				cin>>s[t - 1];
				break;

            case WRT:
				cout<<s[t - 1]<<endl;
				t--;

			case RET:
				t = b;
				p = s[t + 2];
				b = s[t + 1];
				cout<<i.f<<" "<<i.l<<" "<<i.a<<endl;
				break;
		}
	}while(p != 0);
	return 0;
}
