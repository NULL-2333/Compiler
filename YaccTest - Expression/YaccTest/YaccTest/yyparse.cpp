#include<iostream>
#include<map>
#include<stack>
#include<fstream>
#include<string>
#define E 40000
#define ACCEPT 0
#define SOURCE_END 40001

union SV
{

	int ival;
	float fval;
};
SV u[10];
SV yyval;
using namespace std;
extern int Ana(char *yytext,int n);
struct Sym
{
	int symbol;
	int state;
};
map<int,int> symbolTable;
int producerN[7]={1,1,3,3,3,3,1};
int pLeftSection[7]={34999,35000,35001,35001,35001,35001,35001};
ifstream fin;
ifstream producerfile;
int actionTable[12][8]=
		{		{1,2,3,E,E,E,E,E},
		{E,E,E,-6,-6,-6,-6,-6},
		{E,E,E,E,E,E,E,0},
		{E,E,E,4,5,6,7,-1},
		{1,E,8,E,E,E,E,E},
		{1,E,9,E,E,E,E,E},
		{1,E,10,E,E,E,E,E},
		{1,E,11,E,E,E,E,E},
		{E,E,E,-3,-3,6,7,-3},
		{E,E,E,-2,-2,6,7,-2},
		{E,E,E,-4,-4,-4,-4,-4},
		{E,E,E,-5,-5,-5,-5,-5}};
int predictTable(int cstate,char symbol)
{
	return actionTable[cstate][symbol];
}

int readToken()
{
	if(fin.eof())
		return SOURCE_END;
	char buf[256];//不支持超过256个字符的符号
	int pos=0;
	bool isEnd=false;
	bool isToken=false;//与isOperator互斥
	bool isOperator=false;
	string ops("!@#$%^&*()+-=|[]{};':\",.<>/?");
	string wss("\t\n\r ");
	string letter("_0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");
	char c=fin.get();
	while(c==' '||c=='\t'||c=='\n'||c=='\r') c=fin.get();//滤掉空白符
	while(!isEnd)
	{
		if(c==-1)
		{
			buf[pos]='\0';
			isEnd=true;
			continue;
		}		if(ops.find(c)>=0&&ops.find(c)<ops.size())//表示当c不是正常字符时。
		{
			//进入非正常字符的处理过程
			if(isToken)
			{
				fin.unget();
				buf[pos]='\0';
				isEnd=true;
			}
			else
			{
				isOperator=true;
				buf[pos]=c;
				pos++;
				c=fin.get();
			}
			continue;
		}
		if(letter.find(c)>=0&&letter.find(c)<letter.size())
		{
			if(isOperator)
			{
				fin.unget();
				buf[pos]='\0';
				isEnd=true;
			}
			else
			{
				isToken=true;
				buf[pos]=c;
				pos++;
				c=fin.get();
			}
			continue;
		}
		if(wss.find(c)>=0&&wss.find(c)<wss.size())
		{
			buf[pos]='\0';
			isEnd=true;
		}
		
		
		else return -1;
	}
	return Ana(buf,strlen(buf));
}
void runaction(int num)
{
	switch(num)
	{
	case 1:
		{
			u[0].ival=u[1].ival;cout<<"最终运算结果为："<<u[0].ival<<endl;
			break;
		}
	case 2:
		{
			u[0].ival=u[1].ival-u[3].ival;
			break;
		}
	case 3:
		{
			u[0].ival=u[1].ival+u[3].ival;
			break;
		}
	case 4:
		{
			u[0].ival=u[1].ival*u[3].ival;
			break;
		}
	case 5:
		{
			u[0].ival=u[1].ival/u[3].ival;
			break;
		}
	case 6:
		{
			u[1].ival=yyval.ival;u[0].ival=u[1].ival;
			break;
		}
	}
}
void getvalue(int symbol,SV & val)
{
	switch(symbol)
	{
	case 30000:val.ival=30000;break;
	}
}
int parse()
{
	int inputsymbol=0;
	int cstate=0;
	stack<Sym> symStack;//符号栈
	stack<SV> valStack;//语义值栈
	Sym st;//用作分析时的临时栈顶元素存储变量
	st.symbol=0;
	st.state=0;
	SV val;
	symStack.push(st);//语义值栈必须要和符号栈同步
	valStack.push(val);
	inputsymbol=readToken();
	while(1)
	{
		st=symStack.top();
		int col=symbolTable[inputsymbol];
		int result=predictTable(st.state,col);
		if(result==E)//出错
		{
			cout<<"不符合yacc语法规范，编译出错!"<<endl;
			return 0;
		}
		if(result==ACCEPT)
		{
			cout<<"编译成功!"<<endl;
			return 1;
		}
		if(result<0)//负数表示为归约项目
		{
			result*=-1;
			char p[256]="";
			producerfile.open("producer.txt");
			for(int i=0;i<=result;i++)
			{
				producerfile.getline(p, 256);
			}
			cout<<p<<endl;
			producerfile.close();
			int n=producerN[result];//取得该号产生式右部符号数量，以作弹栈用
			for(int i=0;i<n;i++)
			{
				symStack.pop();
				u[n-i]=valStack.top();
				valStack.pop();
			}
			runaction(result);//执行语义动作
			//再将产生式左部的符号压栈，语义值一同压栈
			st.symbol=pLeftSection[result];
			st.state=predictTable(symStack.top().state,symbolTable[st.symbol]);
			symStack.push(st);
			//将产生式左部符号的语义值入栈。这个值在runaction()中已经修改
			valStack.push(u[0]);
		}
		else
		{
			st.symbol=inputsymbol;
			st.state=result;
			symStack.push(st);
			SV tu;
			if(yyval.ival==0)
				getvalue(inputsymbol,tu);
			else tu=yyval;
			valStack.push(tu);
			inputsymbol=readToken();
		}
	}
}

void main()
{
	pair<int,int> tp;
	tp.first=40001;
	tp.second=7;
	symbolTable.insert(tp);
	tp.first=30003;
	tp.second=5;
	symbolTable.insert(tp);
	tp.first=30001;
	tp.second=3;
	symbolTable.insert(tp);
	tp.first=30002;
	tp.second=4;
	symbolTable.insert(tp);
	tp.first=30004;
	tp.second=6;
	symbolTable.insert(tp);
	tp.first=30000;
	tp.second=0;
	symbolTable.insert(tp);
	tp.first=35000;
	tp.second=1;
	symbolTable.insert(tp);
	tp.first=35001;
	tp.second=2;
	symbolTable.insert(tp);
	string filename;
	cout<<"请输入编写的测试文件名:"<<endl;
	cin>>filename;
	fin.open(filename.c_str());
	if(fin.fail())
	{
		cout<<"无法打开文件 "<<filename<<endl;
		return;
	}
	parse();
	system("pause");
}
