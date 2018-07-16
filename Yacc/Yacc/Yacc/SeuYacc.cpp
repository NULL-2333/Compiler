#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<set>
#include<vector>
#include<stack>
#include<queue>
#include<map>
#include<set>
#include<conio.h>


#define ACCEPT 0
#define ERROR 40000
#define EPSLONG 39999
#define SEGMENT_DELIM 39998
#define DEF_SEG_BEGIN 39997
#define DEF_SEG_END 39996
#define TOKEN 39995
#define TYPE 39994
#define UNION 39993
#define LEFT 39992
#define TERMINATE 39991
#define NOMATCH 39000
#define RIGHT 38999
#define SEGMENT_REG 38998
#define SELF_DEF 38997
#define NONTERMINS 35000
#define TERMINS 30000
using namespace std;
struct Producer
{
	int left;
	vector<int> right;
};
ifstream fin;
ofstream outputCppfile;
ofstream outputSymbolTablefile;
ofstream outputDotHfile;
ofstream outputProducerfile;
char NO=1;
//一些常量的定义
vector<Producer> producerSet;//存储所有产生式的向量，在读入产生式时进行初始化！
map<int,vector<int> > hpSet;//以产生式的左部为关键字，以对应产生式的编号为内容的哈希表。
map<int,int> terminSet;//存储终结符,并且要在其中加入结束符号#
map<int,int> nonterminSet;//存储非终结符,并同时存储一个到action表头的对应关系
vector<vector<int> > actionTable;//存储动作表，其中第二维大小必须已知
map<string,int> tsymTable;//这个表是终结符的串到相应数字编码的映射
map<string,int> ntsymTable;
map<int,int> priorityTable;//优先级表
vector<int> producerPrioTable;//产生式的优先级表，和上面的优先级表联合使用
set<int> leftTable;//左结合表
set<int> rightTable;//右结合表
map<int,string> symclaTable;//存储语义值类型的表
vector<string> produceActionTable;//存储语义动作的表

//类Item的定义：表示项目
class Item
{
public:
	friend bool operator <(const Item &it1,const Item &it2);//<符号的重载
	Item(int producerNum,int currentPos);//构造函数
	int getCurrSym();//返回下一个将要移进的符号，如果已经到达最后，则返回-1，表示可归约。
	int getNextSym() const;//返回再一下要移进的符号，在进行项目集扩展运算中计算预测符时使用。
	int getProdN();//返回产生式的编号。
	bool isEnd();//是否右部已经全部移进。
	bool nextIsEnd() const;//右部是否到达最后一个符号，在进行项目集扩展运算时用。
	void setpredictSym(const set<int> &predictSymbol);//设置predictSym的值
	const set<int> &getpredictSym() const;//获取predictSym的值
	void move();
private:
	
	int pn;//存储产生式的编号。
	int pos;//存储此项目的移进位置。
	set<int> predictSym;//存储此项目的预测符
};
//类Item的构造函数
Item::Item(int producerNum,int currentPos)
{
	this->pn = producerNum;
	this->pos = currentPos;
}
//返回产生式右部下一移进符号，若返回-1则可归约（-1代表#）
int Item::getCurrSym()
{
	return producerSet[pn].right[pos];
}
//获得再下一个移进的符号
int Item::getNextSym() const
{
	return producerSet[pn].right[pos+1];
}
//获取产生式的编号
int Item::getProdN()
{
	return this->pn;
}
//判断当前产生式是否已经全部移进
bool Item::isEnd()
{
	//主要有两种情况
	//一是读到的预测符为epslong的时候
	//二是当前读头的位置等于下面要读到的预测符的位置
	if(producerSet[pn].right[0]==EPSLONG)
		return true;
	if(this->pos==producerSet[pn].right.size())
		return true;
	return false;
}
//判断右部是否已经达到最后一个符号，在项目集扩展的时候使用
bool Item::nextIsEnd() const
{
	if(this->pos==producerSet[pn].right.size()-1)
		return true;
	return false;
}
void Item::setpredictSym(const set<int> &predictSymbol)
{
	predictSym=predictSymbol;
}
const set<int> &Item::getpredictSym() const
{
	return predictSym;
}
//读头的右移
void Item::move()
{
	pos++;
	return;
}
bool operator <(const Item &it1,const Item &it2)
{
	//此处<的定义，是为了接下来可以将其插入set中，因为set在插入的时候会默认调用<函数判断大小
	if(it1.pn==it2.pn)
	{
		if(it1.pos==it2.pos)
			return it1.getpredictSym()<it2.getpredictSym();
		else return it1.pos<it2.pos;
	}
	else return it1.pn<it2.pn;
}

//函数的声明
void extension(set<Item> &itemSet);
//下面是求first的函数,参数为产生式编号，first符号集，已出现非终结符号集
//此为递归函数，为外部不可调用
void first(const Item &item,set<int> &predictSymbol);
//下面是外部可调的first函数
bool getfirstSet(int producerNum, set<int> &firstSet, set<int> &isUsed);
bool produce();//产生项目集之间的转换关系
void generateTableCode();
void generateParseCode();
void generateSemanticActionCode();//生成语义动作函数
void generateMainCode();
void generateConstCode();
void generateHead();
void generateSVCode();//产生给终结符赋默认值 的代码:void getvalue();
int readinputfile();//前段输入部分的主要函数，用于读入文件并处理
int specSymParse();
bool readReg();//规则段的扫描程序
bool readAproducer();//读取一个产生式
void genhpSet();
void outSignalTable();
void displayItem(Item & item);
void outputProducer();

void main() 
{
	cout<<"SeuYacc 语法分析程序:"<<endl;

	string inputname;
	cout<<"请输入SeuYacc源程序名（以.y为后缀标示）:";
	cin>>inputname;
	fin.open(inputname.c_str());

	outputCppfile.open("yyparse.cpp");
	outputSymbolTablefile.open("Symbol_Table.txt");//生成符号表文件
	outputDotHfile.open("yytab.h");//生成全局符号语义值变量的类型
	outputProducerfile.open("producer.txt");
	if (outputCppfile.fail() || fin.fail() || outputSymbolTablefile.fail() || outputDotHfile.fail() || outputProducerfile.fail())
	{
		cout<<"无法打开输入或输出文件，请检查文件名!"<<endl;
		return;
	}
	//初始化
	Producer ap;
	producerSet.push_back(ap);//首先把拓展文法开始符号压栈
	readinputfile();
	generateMainCode();
	outSignalTable();
	outputProducer();
	outputCppfile.close();
	outputSymbolTablefile.close();
	outputDotHfile.close();
	outputProducerfile.close();
	cout<<"成功生成！可以在C编译器里进行语法解析了...\n";

	system("pause");
}


void outSignalTable()
{
	outputDotHfile<<"\n#ifndef YYTAB_H\n";
	outputDotHfile<<"#define YYTAB_H\n";
	outputSymbolTablefile<<setw(20)<<"Symbol"<<setw(20)<<"Value"<<endl;
	outputSymbolTablefile<<setfill('*')<<setw(41)<<" "<<setfill(' ')<<endl;
	int num=0;
	for(map<string,int>::iterator pi=tsymTable.begin();pi!=tsymTable.end();pi++)
	{
		string delim("~!@#$%^&*()-=+`{}[]\|'\":;/?.,<>");
		int x=pi->first.find_first_of(delim);
		if(x<0||x>=pi->first.size())
		{
			outputDotHfile<<"#define"<<setw(20)<<pi->first<<setw(20)<<pi->second<<endl;
		}
		outputSymbolTablefile<<setw(20)<<pi->first<<setw(20)<<pi->second<<endl;
		num++;
	}
	outputSymbolTablefile<<setfill('*')<<setw(41)<<" "<<setfill(' ')<<endl;
	outputSymbolTablefile<<setw(20)<<"TOTAL:"<<num<<endl;
	outputDotHfile<<"#endif\n";
}


//该函数实现对于当前项目集中项目的扩展
void extension(set<Item> &itemSet)
{
	queue<Item> Q;
	//首先要把当前项目集中所有状态放入到队列中。(除去不会产生新项的项)
	for(set<Item>::iterator ps=itemSet.begin();ps!=itemSet.end();ps++)
	{
		const Item &item1 = const_cast<Item&>(*ps);
		Item &item2 = const_cast<Item&>(item1);
		if(!item2.isEnd()&&nonterminSet.count(item2.getCurrSym()))
			Q.push(item2);
	}
	//进入循环判断。
	set<int> isU;
	set<int> isE;
	while(!Q.empty())
	{
		Item citem=Q.front();
		Q.pop();
		//获取当前队列底部的项目的当前移进符号，将其作为产生式的左部符号，在hpSet中查询到相对应的产生式的编号
		vector<int> vpi=hpSet[citem.getCurrSym()];
		//当前队列底部的项目的预测符的计算
		set<int> predictSym;
		first(citem,predictSym);	
		for(int i=0;i<vpi.size();i++)
		{
			//遍历该项目集内所有的产生式右部
			Item tp(vpi[i],0);//以当前产生式为基础构建一个项目
			tp.setpredictSym(predictSym);//设置该项目的预测符
			if(!itemSet.count(tp))
			{
				//如果此项目不在项目集中，则需将它加入到项目集中去
				itemSet.insert(tp); 
				//若此时该项目所处状态依然不为终态，则加入队列Q中继续扩展
				if(nonterminSet.count(tp.getCurrSym()))
				{
					Q.push(tp);
				}
			}
		}
	}
}
//预测符的计算
void first(const Item &item,set<int> &predictSymbol)
{
	//若当前项目再次右移进之后就会进入终态
	if(item.nextIsEnd())
	{
		predictSymbol=item.getpredictSym();
	}
	else 
	{
		//获得下一个移进的符号
		int sym=item.getNextSym();
		//若该符号在终态集合中，则直接在预测符集合中插入该符号
		if(terminSet.count(sym))
			predictSymbol.insert(sym);
		else
		{
			//否则需要计算预测符集合
			//首先找到可能对应的产生式以及其对应的右部产生式符号
			vector<int> tv=hpSet[sym];
			set<int> isused;
			for(int i=0;i<tv.size();i++)
			{
				getfirstSet(tv[i],predictSymbol,isused);
			}
		}
	}
}
//对于非终态下项目的预测符的计算
bool getfirstSet(int producerNum, set<int> &firstSet, set<int> &isUsed)
{
	//若返回的项目对应的产生式右部为EPSLONG，则不向预测符集合中插入
	if ( producerSet.at(producerNum).right[0] == EPSLONG )
		return 0;
	else{
		int i = 0;
		//若当前项目对应的产生式的右部的第一个符号即为终结符，则直接将其插入到结果中
		if (terminSet.count(producerSet.at(producerNum).right[i]))
			firstSet.insert(producerSet.at(producerNum).right[i]);
		else if (nonterminSet.count(producerSet.at(producerNum).right[i]))
		{
			//判断当前读头所在符号是否已经被读过，若被读过，则已经被处理，因此就不用再进行处理
			//若没有被读过
			if (!isUsed.count(producerSet.at(producerNum).right[i]))
			{
				//将该产生式插入已用列表
				isUsed.insert(producerSet.at(producerNum).right[i]);
				vector <int> tv;
				//获取该产生式右部中可以继续产生的产生式所对应的编号
				tv = hpSet[producerSet.at(producerNum).right[i]];
				int c = 1;
				//遍历上述所有的产生式
				for (vector < int >::const_iterator p1 = tv.begin(); p1 != tv.end(); p1++)
				{
					//判断该产生式是否有epsilon产生式
					if (!getfirstSet(*p1, firstSet, isUsed))
					{
						c = 0;
					}
				}
				//c = 0，需要对epsilon产生式进行处理
				while (c == 0)
				{
					i++;
					//若该产生式还可以向后产生新的产生式且并未被处理
					if (producerSet.at(producerNum).right[i] != 0 &&
						nonterminSet.count(producerSet.at(producerNum).right[i]) &&
						isUsed.count(producerSet.at(producerNum).right[i]) == 0)
					{
						vector < int > tv2;
						//获取该产生式右部中可以继续产生的产生式所对应的编号
						tv2 = hpSet[producerSet.at(producerNum).right[i]];
						//判断该产生式是否有epsilon产生式
						for (vector < int >::const_iterator p2 = tv2.begin(); p2 != tv2.end(); p2++)
							if (!getfirstSet(*p2, firstSet, isUsed))
								c = 0;
					}
					else if (terminSet.count(producerSet.at(producerNum).right[i]))
						//若当前项目对应的产生式的右部的第一个符号即为终结符，则直接将其插入到预测符表结果中
						firstSet.insert(producerSet.at(producerNum).right[i]);
				}
			}
		}
		return 1;
	}
}
void display(set<Item> & i0)
{
	cout<<"\n----------------ItemSet ---------------------"<<endl;
	cout<<"\nitem -----------------------------\n";
	//打印所有的项目
	for(set<Item>::iterator pi=i0.begin();pi!=i0.end();pi++)
	{
		const Item &item1 = const_cast<Item&>(*pi);
		Item &item2 = const_cast<Item&>(item1);
		
		displayItem(item2);
	}
	cout<<"\n------------------------\n";
}
void displayItem(Item & item)
{
	//打印单个项目
	//打印项目对应的产生式号
	cout<<"PID:"<<item.getProdN()<<"      ";
	//打印项目对应的产生式左部
	cout<<"LEFT:"<<producerSet[item.getProdN()].left<<"      ";
	//打印项目对应的产生式右部
	cout<<"RIGHT:";
	for(int i=0;i<producerSet[item.getProdN()].right.size();i++)
	{
		cout<<producerSet[item.getProdN()].right[i]<<"  ";
	}
	//打印预测符
	cout<<"\npredict:";
	for(set<int>::const_iterator i=item.getpredictSym().begin();i!=item.getpredictSym().end();i++)
	{
		cout<<*i<<"  ";
	}
	cout<<"\n-----------------------------------\n";
}
void genhpSet()
{
	//在遍历整个.y文件的时候，每读一个产生式，就调用该函数更新hpSet表
	//构建每个产生式右部可以继续生成的产生式的集合
	//0号产生式不用处理
	for (int i = 1; i < producerSet.size(); i++)
	{
		//判断该产生式是否已在hpSet集合中
		//若已包含则不需要新建
		//若不包含则根据left新建hpSet的一组值，其中vector<int>初始设置为空
		if (!hpSet.count(producerSet[i].left))
		{
			pair<int, vector<int> > tp;
			tp.first = producerSet[i].left;
			vector<int> vt;
			tp.second = vt;
			hpSet.insert(tp);
		}
		//将该产生式对应的编号加入该left对应的vector<int>中
		hpSet[producerSet[i].left].push_back(i);
	}
}


bool produce()
{
	/**
	 *  此处考虑图的表示方法为二维数组，因为在输出时也是以二维数组的形式。
	 *  在此数组中，用正数表示要跳转的状态号，用负数表示某一待归约的产生式号。
	 *  DONE!
	 */
	//首先是一组要用到的常量定义
	int curState=0;//存储项目集的状态号。随项目集的产生动态更新
	int widgeN=nonterminSet.size()+terminSet.size();//表示所有可能的项目集的边
	queue<set<Item> > Q;
	map<set<Item>,int> itemsetTable;//从项目集到某一数字标识的映射
	map<int, set<Item> > moveItemset;//以边为关键字，以进行分类的项目的项目集为内容。
	set<Item> I0;

	Item firstitem(0,0);//定义初始项目集中的项目。
	set<int> predicts;
	predicts.insert(NONTERMINS-1);//表示结束状态
	firstitem.setpredictSym(predicts);

	I0.insert(firstitem);

	firstitem.move();//修改其移进位，因为在后面输出正确状态时要用

	extension(I0);//移进1位后，进行扩展
	Q.push(I0);//完成后，IO进入队列Q

	display(I0);
	pair<set<Item>,int> te;//将项目集0放入到项目集的表中
	te.first=I0;//first保存IO,包含扩展结果
	te.second=curState;//second保存项目集的状态号
	itemsetTable.insert(te);//将te加入itemsetTable，te包含：项目集号（存在second），产生式编号，读头位置（也就是书上的.这里我们用pos表示），预测符，这三者作为一个item，多个构成set
	//这里参考了秦老师的书上115页构造项目集族和分析表的算法，但是我们做了些改动
	//项目集族构造（没有专门输出，而在在分析表的构建过程中使用到了）
	//主要算法：
	//
	//对于初态的项目集，我们进行扩展，并将相关的移进归约填入表中，同时若生成了新的项目集，就将其加入队列Q等待处理
	while(!Q.empty())
	{
		//**********************1.从itemset中选出需要处理的项目集族号，体现为分析表的行号
		set<Item> itemset=Q.front();
		Q.pop();//从队列中取得一个项目集。
		moveItemset.clear();//清空，以进行新一轮循环

		vector<int> f(widgeN);//数组第二维的大小必须预先定义好
		for(int i=0;i<f.size();i++)
		{
			f[i]=ERROR;//以ERROR值对vector进行初始化。
		}

		actionTable.push_back(f);

		int column,row;
		//在map中查找itemset表示的项目集对应的编号，用作行号
		row=itemsetTable[itemset];
		cout<<"state = "<<row<<endl;
		//**********************2.求该项目集族中的各项移进后，产生的新的项目集族，过程中会出现可归约的项目，加入表中
		for(set<Item>::iterator ps=itemset.begin();ps!=itemset.end();ps++)
		{
			const Item &item1 = const_cast<Item&>(*ps);
			Item &item2 = const_cast<Item&>(item1);
			if(item2.isEnd())//出现可归约项目，需进行处理。
			{
				for(set<int>::const_iterator pi=ps->getpredictSym().begin();pi!=ps->getpredictSym().end();pi++)
				{
					//对可归约项目，对其中每个预测符都填入表中
					column=terminSet[*pi];//获取列号
					//action表中原本已经要归约的位置，在这里又被判定成归约，出错
					if(actionTable[row][column]<=0&&item2.getProdN()!=actionTable[row][column]*(-1))
					{
						cout<<"Reduction & Reduction Confliction"<<endl;
						cout<<"row="<<row<<" column="<<column<<" old="<<actionTable[row][column]
					       <<" new="<<item2.getProdN()<<endl;
						return false;
					}
					actionTable[row][column]=item2.getProdN()*(-1);//变成负数以表示此为归约项,如果为0用于表示accept
				}
			}
			else//无可归约项目
			{
				Item tp=*ps;
				tp.move();//读头往后一位
				moveItemset[item2.getCurrSym()].insert(tp);
			}
		}
		//**********************3.对所有新产生的项目集进行扩展并判断是否需要加入到队列中去，并且继续补充分析表
		for(map<int,set<Item> >::iterator ph=moveItemset.begin();ph!=moveItemset.end();ph++)
		{
			extension(ph->second);//对当前的moveItemset中的项目集合set<Item>进行扩展

			if(!itemsetTable.count(ph->second))//如果map itemsetTable中没有该项目集族，则为它设置新的状态号，然后加入等待处理队列
			{
				pair<set<Item>,int> tp;
				tp.first=ph->second;//复制扩展后的ph->second中的项目集			
				curState++;//只在此处增加项目集，故只在此处修改其值
				tp.second=curState;

				itemsetTable.insert(tp);//将新增的项目集插入itemsetTable 
				//如果当前的项目集未出现过的话，需要将其放入队列中等待处理。
				Q.push(ph->second);
			}	

			//下面开始生成对应的action表
			//首先求得边所对应的表中的序号，看非终结符和终结符map中哪个有
			column=(nonterminSet.count(ph->first)?nonterminSet[ph->first]:terminSet[ph->first]);

			//对于表中的位置，判断里面的值是否为负，此处加一些条件，判断是否有归约、移进冲突
			if(actionTable[row][column]<=0)
			{
				//若当前的优先级低于ph中的优先级，则按照ph中的填入
				if(producerPrioTable[actionTable[row][column]*(-1)]<priorityTable[ph->first])
				{
					actionTable[row][column]=itemsetTable[ph->second];
					continue;
				}
				//若满足右结合（虽然我们的minic里面没有右结合，但是这里还是预留了一下），操作同上
				if(rightTable.count(ph->first))
				{
					actionTable[row][column]=itemsetTable[ph->second];
					continue;
				}//若满足左结合，则继续
				if(leftTable.count(ph->first))
				{
					continue;
				}
	
				//如果上述条件都不满足，则报错。
				cout<<"Reduction & Shift Confliction"<<endl;
				cout<<"row="<<row<<" column="<<column<<" old="<<actionTable[row][column]
					<<" new="<<itemsetTable[ph->second]
					<<" signal="<<ph->first<<endl;
				return false;
			}
			else
				//actionTable[row][column]>0的情况
				actionTable[row][column]=itemsetTable[ph->second];
			
		}
	}
	return true;
	//到此完成了项目集到分析表的构造
}


void generateTableCode()
{
	outputCppfile<<"int actionTable["<<actionTable.size()<<"]["<<actionTable.front().size()<<"]=\n\t\t{";
	for(int i=0;i<actionTable.size();i++)
	{
		outputCppfile<<"\t\t{";
		for(int j=0;j<actionTable.front().size();j++)
		{
			if(actionTable[i][j]!=40000)
				outputCppfile<<actionTable[i][j];
			else
				outputCppfile<<"E";
			if(j==actionTable.front().size()-1) continue;
			outputCppfile<<",";
		}
		outputCppfile<<"}";
		if(i==actionTable.size()-1) continue;
		outputCppfile<<",\n";
	}
	outputCppfile<<"};\n";
	outputCppfile<<"int predictTable(int cstate,char symbol)\n";
	outputCppfile<<"{\n";
	outputCppfile<<"\treturn actionTable[cstate][symbol];\n";
	outputCppfile<<"}\n";
	outputCppfile<<"\n";
	//此部分完成查表程序的输出
	//下面完成读Token程序的输出
	outputCppfile<<"int readToken()\n";
	outputCppfile<<"{\n";
	outputCppfile<<"\tif(fin.eof())\n";
	outputCppfile<<"\t\treturn SOURCE_END;\n";
	outputCppfile<<"\tchar buf[256];//不支持超过256个字符的符号\n";
	outputCppfile<<"\tint pos=0;\n";
	outputCppfile<<"\tbool isEnd=false;\n";
	outputCppfile<<"\tbool isToken=false;//与isOperator互斥\n";
	outputCppfile<<"\tbool isOperator=false;\n";
	outputCppfile<<"\tstring ops(\"!@#$%^&*()+-=|\[]{};':\\\",.<>/?\");\n";
	outputCppfile<<"\tstring wss(\"\\t\\n\\r \");\n";
	outputCppfile<<"\tstring letter(\"_0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ\");\n";

	outputCppfile<<"\tchar c=fin.get();\n";
	outputCppfile<<"\twhile(c==' '||c=='\\t'||c=='\\n'||c=='\\r') c=fin.get();//滤掉空白符\n";
	outputCppfile<<"\twhile(!isEnd)\n";
	outputCppfile<<"\t{\n";
	outputCppfile<<"\t\tif(c==-1)\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tbuf[pos]='\\0';\n";
	outputCppfile<<"\t\t\tisEnd=true;\n";
	outputCppfile<<"\t\t\tcontinue;\n";
	outputCppfile<<"\t\t}";
	outputCppfile<<"\t\tif(ops.find(c)>=0&&ops.find(c)<ops.size())//表示当c不是正常字符时。\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\t//进入非正常字符的处理过程\n";
	outputCppfile<<"\t\t\tif(isToken)\n";
	outputCppfile<<"\t\t\t{\n";
	outputCppfile<<"\t\t\t\tfin.unget();\n";
	outputCppfile<<"\t\t\t\tbuf[pos]='\\0';\n";
	outputCppfile<<"\t\t\t\tisEnd=true;\n";
	outputCppfile<<"\t\t\t}\n";
	outputCppfile<<"\t\t\telse\n";
	outputCppfile<<"\t\t\t{\n";
	outputCppfile<<"\t\t\t\tisOperator=true;\n";
	outputCppfile<<"\t\t\t\tbuf[pos]=c;\n";
	outputCppfile<<"\t\t\t\tpos++;\n";
	outputCppfile<<"\t\t\t\tc=fin.get();\n";
	outputCppfile<<"\t\t\t}\n";
	outputCppfile<<"\t\t\tcontinue;\n";
	outputCppfile<<"\t\t}\n";
	outputCppfile<<"\t\tif(letter.find(c)>=0&&letter.find(c)<letter.size())\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tif(isOperator)\n";
	outputCppfile<<"\t\t\t{\n";
	outputCppfile<<"\t\t\t\tfin.unget();\n";
	outputCppfile<<"\t\t\t\tbuf[pos]='\\0';\n";
	outputCppfile<<"\t\t\t\tisEnd=true;\n";
	outputCppfile<<"\t\t\t}\n";
	outputCppfile<<"\t\t\telse\n";
	outputCppfile<<"\t\t\t{\n";
	outputCppfile<<"\t\t\t\tisToken=true;\n";
	outputCppfile<<"\t\t\t\tbuf[pos]=c;\n";
	outputCppfile<<"\t\t\t\tpos++;\n";
	outputCppfile<<"\t\t\t\tc=fin.get();\n";
	outputCppfile<<"\t\t\t}\n";
	outputCppfile<<"\t\t\tcontinue;\n";
	outputCppfile<<"\t\t}\n";
	outputCppfile<<"\t\tif(wss.find(c)>=0&&wss.find(c)<wss.size())\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tbuf[pos]='\\0';\n";
	outputCppfile<<"\t\t\tisEnd=true;\n";
	outputCppfile<<"\t\t}\n";
	outputCppfile<<"\t\t\n";
	outputCppfile<<"\t\t\n";

	outputCppfile<<"\t\telse return -1;\n";
	outputCppfile<<"\t}\n";
	outputCppfile<<"\treturn Ana(buf,strlen(buf));\n";
	outputCppfile<<"}\n";
}

void generateSemanticActionCode()
{
	outputCppfile<<"void runaction(int num)\n";
	outputCppfile<<"{\n";
	outputCppfile<<"\tswitch(num)\n";
	outputCppfile<<"\t{\n";
	for(int i=0;i<produceActionTable.size();i++)
	{
		if(produceActionTable[i].size()!=0)
		{
			outputCppfile<<"\tcase "<<i+1<<":\n";
			outputCppfile<<"\t\t{\n";
			outputCppfile<<"\t\t\t"<<produceActionTable[i]<<"\n";
			outputCppfile<<"\t\t\tbreak;\n";
			outputCppfile<<"\t\t}\n";
		}
	}
	outputCppfile<<"\t}\n";
	outputCppfile<<"}\n";
}

void generateParseCode()
{
	outputCppfile<<"int parse()\n";
	outputCppfile<<"{\n";
	outputCppfile<<"\tint inputsymbol=0;\n";
	outputCppfile<<"\tint cstate=0;\n";
	outputCppfile<<"\tstack<Sym> symStack;//符号栈\n";
	outputCppfile<<"\tstack<SV> valStack;//语义值栈\n";
	outputCppfile<<"\tSym st;//用作分析时的临时栈顶元素存储变量\n";
	outputCppfile<<"\tst.symbol=0;\n";
	outputCppfile<<"\tst.state=0;\n";
	outputCppfile<<"\tSV val;\n";
	outputCppfile<<"\tsymStack.push(st);//语义值栈必须要和符号栈同步\n";
	outputCppfile<<"\tvalStack.push(val);\n";
	outputCppfile<<"\tinputsymbol=readToken();\n";
	outputCppfile<<"\twhile(1)\n";
	outputCppfile<<"\t{\n";
	outputCppfile<<"\t\tst=symStack.top();\n";
	outputCppfile<<"\t\tint col=symbolTable[inputsymbol];\n";
	outputCppfile<<"\t\tint result=predictTable(st.state,col);\n";
	outputCppfile<<"\t\tif(result==E)//出错\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tcout<<\"不符合yacc语法规范，编译出错!\"<<endl;\n";
	outputCppfile<<"\t\t\treturn 0;\n";
	outputCppfile<<"\t\t}\n";
	outputCppfile<<"\t\tif(result==ACCEPT)\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tcout<<\"编译成功!\"<<endl;\n";
	outputCppfile<<"\t\t\treturn 1;\n";
	outputCppfile<<"\t\t}\n";
	outputCppfile<<"\t\tif(result<0)//负数表示为归约项目\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tresult*=-1;\n";
	outputCppfile<<"\t\t\tchar p[256]=\"\";\n";
	outputCppfile<<"\t\t\tproducerfile.open(\"producer.txt\");\n";
	outputCppfile<<"\t\t\tfor(int i=0;i<=result;i++)\n";
	outputCppfile<<"\t\t\t{\n";
	outputCppfile<<"\t\t\t\tproducerfile.getline(p, 256);\n";
	outputCppfile<<"\t\t\t}\n";
	outputCppfile<<"\t\t\tcout<<p<<endl;\n";
	outputCppfile<<"\t\t\tproducerfile.close();\n";
	outputCppfile<<"\t\t\tint n=producerN[result];//取得该号产生式右部符号数量，以作弹栈用\n";
	outputCppfile<<"\t\t\tfor(int i=0;i<n;i++)\n";
	outputCppfile<<"\t\t\t{\n";
	outputCppfile<<"\t\t\t\tsymStack.pop();\n";
	outputCppfile<<"\t\t\t\tu[n-i]=valStack.top();\n";
	outputCppfile<<"\t\t\t\tvalStack.pop();\n";
	outputCppfile<<"\t\t\t}\n";
	outputCppfile<<"\t\t\trunaction(result);//执行语义动作\n";
	outputCppfile<<"\t\t\t//再将产生式左部的符号压栈，语义值一同压栈\n";
	outputCppfile<<"\t\t\tst.symbol=pLeftSection[result];\n";
	outputCppfile<<"\t\t\tst.state=predictTable(symStack.top().state,symbolTable[st.symbol]);\n";
	outputCppfile<<"\t\t\tsymStack.push(st);\n";
	outputCppfile<<"\t\t\t//将产生式左部符号的语义值入栈。这个值在runaction()中已经修改\n";
	outputCppfile<<"\t\t\tvalStack.push(u[0]);\n";
	outputCppfile<<"\t\t}\n";
	outputCppfile<<"\t\telse\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tst.symbol=inputsymbol;\n";
	outputCppfile<<"\t\t\tst.state=result;\n";
	outputCppfile<<"\t\t\tsymStack.push(st);\n";
	outputCppfile<<"\t\t\tSV tu;\n";
	outputCppfile<<"\t\t\tif(yyval.ival==0)\n";
	outputCppfile<<"\t\t\t\tgetvalue(inputsymbol,tu);\n";
	outputCppfile<<"\t\t\telse tu=yyval;\n";
	outputCppfile<<"\t\t\tvalStack.push(tu);\n";
	outputCppfile<<"\t\t\tinputsymbol=readToken();\n";
	outputCppfile<<"\t\t}\n";
	outputCppfile<<"\t}\n";
	outputCppfile<<"}\n";
}

void generateMainCode()
{
	outputCppfile<<"void main()\n";
	outputCppfile<<"{\n";

	outputCppfile<<"\tpair<int,int> tp;\n";
	for(map<string,int>::iterator pti=tsymTable.begin();pti!=tsymTable.end();pti++)
	{
		if(pti->first!="#")
		{
			outputCppfile<<"\ttp.first="<<pti->second<<";\n";
			outputCppfile<<"\ttp.second="<<terminSet[pti->second]<<";\n";
			outputCppfile<<"\tsymbolTable.insert(tp);\n";
		}
		else
		{
			outputCppfile<<"\ttp.first=40001;\n";
			outputCppfile<<"\ttp.second="<<terminSet[pti->second]<<";\n";
			outputCppfile<<"\tsymbolTable.insert(tp);\n";
		}
	}
	for(map<int,int>::iterator pti=nonterminSet.begin();pti!=nonterminSet.end();pti++)
	{
		outputCppfile<<"\ttp.first="<<pti->first<<";\n";
		outputCppfile<<"\ttp.second="<<pti->second<<";\n";
		outputCppfile<<"\tsymbolTable.insert(tp);\n";
	}
	//以上完成SymbolTable的初始化段代码.
	outputCppfile<<"\tstring filename;\n";
	outputCppfile<<"\tcout<<\"请输入编写的测试文件名:\"<<endl;\n";
	outputCppfile<<"\tcin>>filename;\n";
	outputCppfile<<"\tfin.open(filename.c_str());\n";
	outputCppfile<<"\tif(fin.fail())\n";
	outputCppfile<<"\t{\n";
	outputCppfile<<"\t\tcout<<\"无法打开文件 \"<<filename<<endl;\n";
	outputCppfile<<"\t\treturn;\n";
	outputCppfile<<"\t}\n";
	outputCppfile<<"\tparse();\n";
	outputCppfile<<"\tsystem(\"pause\");\n";
	outputCppfile<<"}\n";
}
void generateSVCode()
{
	outputCppfile<<"void getvalue(int symbol,SV & val)\n";
	outputCppfile<<"{\n";
	outputCppfile<<"\tswitch(symbol)\n";
	outputCppfile<<"\t{\n";
	for(map<int,int>::iterator pi=terminSet.begin();pi!=terminSet.end();pi++)
	{
		if(symclaTable.count(pi->first))
		outputCppfile<<"\tcase "<<pi->first<<":val."<<symclaTable[pi->first]<<"="<<pi->first
			<<";break;\n";
	}
	outputCppfile<<"\t}\n";
	outputCppfile<<"}\n";


}

void generateConstCode()
{
	outputCppfile<<"using namespace std;\n";
	outputCppfile<<"extern int Ana(char *yytext,int n);\n";
	outputCppfile<<"struct Sym\n";
	outputCppfile<<"{\n";
	outputCppfile<<"\tint symbol;\n";
	outputCppfile<<"\tint state;\n";
	outputCppfile<<"};\n";
	outputCppfile<<"map<int,int> symbolTable;\n";
	outputCppfile<<"int producerN["<<producerSet.size()<<"]={";
	for(int i=0;i<producerSet.size();i++)
	{
		outputCppfile<<producerSet[i].right.size();
		if(i!=producerSet.size()-1)
			outputCppfile<<",";
	}
	outputCppfile<<"};\n";
	outputCppfile<<"int pLeftSection["<<producerSet.size()<<"]={";
	for(int i=0;i<producerSet.size();i++)
	{
		outputCppfile<<producerSet[i].left;
		if(i!=producerSet.size()-1)
			outputCppfile<<",";
	}
	outputCppfile<<"};\n";
	outputCppfile<<"ifstream fin;\n";
	outputCppfile<<"ifstream producerfile;\n";
}
void generateHead()
{
	outputCppfile<<"#include<iostream>\n";
	outputCppfile<<"#include<map>\n";
	outputCppfile<<"#include<stack>\n";
	outputCppfile<<"#include<fstream>\n";
	outputCppfile<<"#include<string>\n";
	outputCppfile<<"#define E 40000\n";
	outputCppfile<<"#define ACCEPT 0\n";
	outputCppfile<<"#define SOURCE_END 40001\n";	
}

int readinputfile()
{
	generateHead();
	char c=fin.get();
	if(c!='%') return TERMINATE;//要求输入文件第一个字符必须为%
	int state=specSymParse();
	while(!fin.eof()&&c!=-1)
	{
		switch(state)
		{
		case SEGMENT_DELIM:
			{
				NO++;
				if(NO==2)
				{
					state=SEGMENT_REG;//进入规则段
					continue;//跳过后面的查找%的语句，直接回到switch
				}
				if(NO==3)
				{
					state=SELF_DEF;
					continue;
				}
				break;
			}
		case DEF_SEG_BEGIN://output 1
			{
				//进入%{处理，完全输出到文件
				while(1)
				{
					c=fin.get();
					if(c=='%')
					{
						state=specSymParse();
						if(state==DEF_SEG_END) break;//如果到达底部%}，则退出循环。
						else if(state!=NOMATCH)
						{
							return TERMINATE;//如果既不等于DEF_SEG_END也不等于NOMATCH，则报错。
						}
					}
					outputCppfile.put(c);///////********output to file
				}
				break;
			}
		case TOKEN:case TYPE:
			{
				string line;
				getline(fin,line);

				int lpos=0;
				int pos=0;
				while(line[pos]!='<'&&pos<line.size()) pos++;
				if(pos==line.size())
				{
					cout<<"%type formation error!"<<endl;
					return 0;
				}
				
				pos++;//往后移一个字符
				lpos=pos;
				while(line[pos]!='>'&&pos<line.size()) pos++;
				if(pos==line.size())
				{
					cout<<"%type or token formation error!"<<endl;
					return 0;
				}
				string valstr=line.substr(lpos,pos-lpos);//取得值类型

				char ntbuf[20]={0};
				int i=0;
				while(1)
				{
					pos++;
					if(pos>line.size()) break;
					if(line[pos]==' '||pos==line.size())
					{
						if(ntbuf[0]=='\0') continue;
						ntbuf[i]='\0';
						string ntstr(ntbuf);

						pair<string,int> tp;
						tp.first=ntstr;
						if(state==TYPE)
						{
							tp.second=NONTERMINS+ntsymTable.size();
						}
						else
						{
							tp.second=TERMINS+tsymTable.size();
						}
						pair<int,int> tp2;
						tp2.first=tp.second;
						tp2.second=terminSet.size()+nonterminSet.size();

						if(state==TYPE)
						{
							ntsymTable.insert(tp);
							nonterminSet.insert(tp2);
						}
						else
						{
							tsymTable.insert(tp);
							terminSet.insert(tp2);
						}

						pair<int,string> stp;
						stp.first=tp.second;
						stp.second=valstr;
						symclaTable.insert(stp);
						
						i=0;
						ntbuf[0]='\0';
					}
					else
					{
						ntbuf[i]=line[pos];	
						i++;
					}
				}
				break;
				//此部分处理完之后，完成了表symclaTable和部分ntsymTable
			}
		case UNION://output 2
			{
				c=fin.get();
				while(c!='{'&&c!=-1) c=fin.get();
				if(c==-1) return TERMINATE;	
				outputDotHfile<<"#ifndef YYVAL_H\n";//输出到头文件yyval_h.h
				outputDotHfile<<"#define YYVAL_H\n";
				outputCppfile<<"union SV\n";
				outputDotHfile<<"union SV\n";
				outputCppfile<<"{\n";
				outputDotHfile<<"{\n";
				while((c=fin.get())!='}') {outputCppfile.put(c);outputDotHfile.put(c);}
				outputCppfile<<"};\n";//此段没有做任何保存，即是说如果出现错误也不能校错。
				outputDotHfile<<"};\n";
				outputCppfile<<"SV u[10];\n";//在此声明数目为10的联合数组，以便后面语义动作时调用。
				outputCppfile<<"SV yyval;\n";//在此声明全局变量yyval，用于返回lex符号的语义值
				outputDotHfile<<"#endif";
				break;
			}
		case LEFT:case RIGHT:
			{
				string line;
				getline(fin,line);
				int lpos=0;
				int pos=0;
				static int priority=0;
				priority++;
				while(pos!=line.find_last_of('\''))
				{
					lpos=line.find_first_of('\'',lpos);
					pos=line.find_first_of('\'',lpos+1);
					string t=line.substr(lpos+1,pos-lpos-1);
					lpos=pos+1;	
					//存储操作符。
					pair<string,int> tp;
					tp.first=t;
					tp.second=TERMINS+tsymTable.size();
					tsymTable.insert(tp);//把操作符也放到终结符表中。
				    
					pair<int,int> tpp;
					tpp.first=tp.second;
					tpp.second=terminSet.size()+nonterminSet.size();
					terminSet.insert(tpp);
					if(state==LEFT)//加入左右结合表
					{
						leftTable.insert(tp.second);
					}
					else
					{
						rightTable.insert(tp.second);
					}

					pair<int,int> ptp;
					ptp.first=tp.second;
					ptp.second=priority;
					priorityTable.insert(ptp);//把操作符放入优先级表中。
				}
				break;
			}
		case SEGMENT_REG:
			{
				//此处为规则段的扫描和处理输出。
				//第一步读产生式的集合，完成各种表的数据。			
				//修正producerPrioTable		
				producerPrioTable.push_back(0);//保证索引值访问不会出错。
				if(!readReg())
				{
					cout<<"read reg segment error!\n";
					return 0;
				}
				//修正ProducerSet;
				producerSet[0].left=NONTERMINS-1;//表示是拓展文法开始符号
				producerSet[0].right.push_back(producerSet[1].left);
				//修正tsymTable
				pair<string,int> tp1;
				string s("#");
				tp1.first=s;
				tp1.second=NONTERMINS-1;
				tsymTable.insert(tp1);
				//修正terminSet
				pair<int,int> tp2;
				tp2.first=tp1.second;
				tp2.second=terminSet.size()+nonterminSet.size();
				terminSet.insert(tp2);
				//第二步完成动作表的生成。
				genhpSet();//首先完成hpSet的生成
				if(!produce())
				{
					cout<<"produce action table error!"<<endl;
					return 0;
				}

				//输出文件定义的一些常量
				generateConstCode();
				//第三步输出查表函数及读Token的函数
				generateTableCode();
				//第四步输出执行语义动作的函数
				generateSemanticActionCode();
				//第五步输出分析函数,首先产生void getvalue()函数，给终结符赋默认初值
				generateSVCode();
				generateParseCode();
				//第六步输出主函数，应该留到最后输出。
				//generateMainCode();
				break;
			}
		case SELF_DEF://输出用户自定义子例程
			{
				c=fin.get();
				while(c!=-1)
				{
					outputCppfile.put(c);////*******output to file
					c=fin.get();
					
				}
				outputCppfile.put('\n');
				continue;//跳回while，判断结束。
			}
		}
		c=fin.get();
		while(c!='%'&&c!=-1) c=fin.get();
		if(c=='%')
			state=specSymParse();
	}
}
int specSymParse()
{
	char nextc=fin.get();
	switch(nextc)
	{
	case '%'://进入段分隔符
		{
			return SEGMENT_DELIM;
		}
	case '{':
		{
			return DEF_SEG_BEGIN;
		}
	case '}':
		{
			return DEF_SEG_END;
		}
	case 't':case 'u':case 'l':case 'r'://%toke//%union//%left
		{
			char buf[10];
			int pos=1;
			buf[0]=nextc;
			nextc=fin.get();
			while(nextc!=' '&&nextc!='\n'&&nextc!='\t')
			{
				// 将token|union|left|right读入buf中
				buf[pos]=nextc;
				nextc=fin.get();
				pos++;
				if(pos==5) break;
			}
			buf[pos]='\0';
			string bbuf(buf);
			string t1("token");
			string t2("type");
			string t3("union");
			string t4("left");
			string t5("right");
			if(bbuf==t1) return TOKEN;
			if(bbuf==t2) return TYPE;
			if(bbuf==t3) return UNION;
			if(bbuf==t4) return LEFT;
			if(bbuf==t5) return RIGHT;
			else return TERMINATE;//错误，程序中止
		}

	default:
		fin.unget();
		return NOMATCH;
	}
}

bool readReg()
{
	//规则段的扫描程序
	char c=fin.get();
	while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	int state=-1;
	char buf[256];
	int pos=0;
	while(1)
	{
		if(c==-1) break;
		if(c=='%')
		{
			state=specSymParse();
			if(state==SEGMENT_DELIM)
			{
				fin.unget();
				fin.unget();//连退两个字符，因为在readinputfile()里面有对%%进行处理的程序
				break;
			}
			if(state==TERMINATE) 
			{
				cout<<"Regulation Segment ERROR!"<<endl;
				return false;
			}	
			if(state==NOMATCH) fin.unget();//将%也退回，留作下面程序处理
			else
			{
				cout<<"Regulation Segment ERROR2"<<endl;
				return false;//出错，直接返回.
			}
		}


		while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();//抛掉空格
		fin.unget();//退回，交给函数readAproducer()处理
		if(!readAproducer())
		{
			cout<<"producer error!see above!"<<endl;
			return false;
		}
		c=fin.get();
		while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	}

}
bool readAproducer()//未抛掉前后的空格
{
	char c=fin.get();
	char buf[256];
	int pos=0;
	while(c!=' '&&c!='\n'&&c!='\t'&&c!='\r'&&c!=-1)
	{
		buf[pos]=c;
		pos++;
		c=fin.get();
	}
	fin.unget();
	buf[pos]='\0';
	string temp(buf);
	buf[0]='\0';//清空buf。
	pos=0;
	if(tsymTable.count(temp))
	{
		cout<<"NonTerminatedSym define error!"<<endl;
		return false;
	}
	if(!ntsymTable.count(temp))
	{
		pair<string,int> tp;
		tp.first=temp;
		tp.second=NONTERMINS+ntsymTable.size();
		ntsymTable.insert(tp);
	
		pair<int,int> tpp;
		tpp.first=tp.second;
		tpp.second=terminSet.size()+nonterminSet.size();
		nonterminSet.insert(tpp);//往非终结符表内加入内容。存储的是非终结符对应到action表的表头
	}
	c=fin.get();
	while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	if(c!=':')
	{
		cout<<"Producer formation Error!"<<endl;
		return false;
	}
	while(1)
	{
		Producer ap;
		ap.left=ntsymTable[temp];//新建一个产生式，先保存左部的值
		int priority=0;//存储产生式的优先级，此时采取的方法是，产生式的优先级同右部符号最右的操作
	               //符优先级相同
		while(1)
		{
		//首先抛掉空格
			c=fin.get();
			while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
			if(c==';'||c=='{'||c=='|') break;//产生式结束标记或者是动作开始标记
			//读产生式的右部
			if(c=='\'')//如果遇到'，则进入操作符的读入程序
			{
				c=fin.get();
				while(c!='\'')
				{
					buf[pos]=c;
					pos++;
					c=fin.get();
				}
				buf[pos]='\0';
				string temp3(buf);
				pos=0;
				buf[0]='\0';//clear buf
				if(!tsymTable.count(temp3))
				{
					pair<string,int> tp;
					tp.first=temp3;
					tp.second=TERMINS+tsymTable.size();

					tsymTable.insert(tp);//把操作符也放到终结符表中。
				
					pair<int,int> tpp;
					tpp.first=tp.second;
					tpp.second=terminSet.size()+nonterminSet.size();
					terminSet.insert(tpp);
					//ap.right.push_back(tp.second);

					pair<int,int> ptp;
					ptp.first=tp.second;
					ptp.second=0;
					priorityTable.insert(ptp);
				}
				ap.right.push_back(tsymTable[temp3]);
				priority=priorityTable[tsymTable[temp3]];
				continue;
			}
			if(c=='\\')//用\e表示空
			{
				c=fin.get();
				if(c=='e')
				{
					break;
				}
				else
				{
					cout<<"epslong error!"<<endl;
					return false;
				}
			}
			else
			{
				while(c!=' '&&c!=-1&&c!='\n'&&c!='\t'&&c!='\r')
				{
					buf[pos]=c;
					pos++;
					c=fin.get();
				}
				buf[pos]='\0';
				string temp1(buf);
				pos=0;
				buf[0]='\0';//clear buf
				if(tsymTable.count(temp1))
				{
					ap.right.push_back(tsymTable[temp1]);
				}
				else
				{
					if(!ntsymTable.count(temp1))
					{
						pair<string,int> tp;
						tp.first=temp1;
						tp.second=NONTERMINS+ntsymTable.size();
						ntsymTable.insert(tp);
	
						pair<int,int> tpp;
						tpp.first=tp.second;
						tpp.second=terminSet.size()+nonterminSet.size();
						nonterminSet.insert(tpp);

					}
					ap.right.push_back(ntsymTable[temp1]);
				}
			}
		}
		int i;
		char action[1000];//存储语义动作
		action[0]='\0';
		if(c=='{')
		{
			char change[4]={'u','[','0',']'};
			int pi=0;
			c=fin.get();
			while(c!='}')
			{
				if(c=='$')
				{
					c=fin.get();
					if(c=='$')//表示是产生式左部的语义值符号
					{
						//插入串"u[0]"
						for( i=0;i<4;i++)
							action[pi+i]=change[i];
						pi+=4;
						action[pi]='.';//加上语义值的具体类型
						pi++;
						string s=symclaTable[ap.left];
						for( i=0;i<s.size();i++)
							action[pi+i]=s[i];
						pi+=i;
					}
					else 
					if(c>='1'&&c<='9')
					{
						int num=0;
						action[pi++]='u';
						action[pi++]='[';
						while(c>='0'&&c<='9')//支持更多的右部非终结符
						{
							action[pi++]=c;
							num=num*10+c-'0';
							c=fin.get();
						}
						fin.unget();
						action[pi++]=']';
						/*
						for(int i=0;i<4;i++)
						{
							action[pi+i]=change[i];
						}*/
						//action[pi+2]=c;//只需修改此处的值即可。
						//pi+=4;
						action[pi++]='.';//加上语义值的具体类型
						string s=symclaTable[ap.right[num-1]];
						for( i=0;i<s.size();i++)
							action[pi+i]=s[i];
						pi+=i;
					}
					else
					{
						cout<<"Action definition error!"<<endl;
						return false;
					}
				}
				else
				{
					action[pi]=c;
					pi++;	
				}
				c=fin.get();
			}
			action[pi]='\0';
			c=fin.get();
			while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
		}
		if(c==';'||c=='|')
		{
			//保存语义动作
			string oneaction(action);
			produceActionTable.push_back(oneaction);
			producerSet.push_back(ap);
			producerPrioTable.push_back(priority);
			if(c==';')
				break;
		}
		else
		{
			cout<<"Producer formation error,there should be a ';' after each producer!"<<endl;
			return false;
		}
	}
	return true;
}
string getTermin(int i)
{
	// 由对应的终结符编号获得终结符
	map<string, int>::iterator iter;
	iter = tsymTable.begin();
	while (iter != tsymTable.end())
	{
		if (iter->second == i)
			return iter->first;
		iter++;
	}
	return "ERROR";
}
string getNtermin(int i)
{
	// 由对应的非终结符编号获得非终结符
	map<string, int>::iterator iter;
	iter = ntsymTable.begin();
	while (iter != ntsymTable.end())
	{
		if (iter->second == i)
			return iter->first;
		iter++;
	}
	return "ERROR";
}
void outputProducer()
{
	// 将产生式输出到文件中

	for (int i = 0; i < producerSet.size(); i++)
	{
		outputProducerfile << i;
		outputProducerfile << "\t";
		string temp = getTermin(producerSet[i].left);
		if (temp == "ERROR")
			temp = getNtermin(producerSet[i].left);
		outputProducerfile << temp <<"-->";
		for (int j = 0; j < producerSet[i].right.size(); j++)
		{
			
			temp = getNtermin(producerSet[i].right[j]);
			if (temp == "ERROR")
				temp = getTermin(producerSet[i].right[j]);
			outputProducerfile << temp;
		}
		outputProducerfile << "\n";
	}
}