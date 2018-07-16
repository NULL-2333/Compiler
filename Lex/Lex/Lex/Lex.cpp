#include<iostream>
#include<fstream>
#include<string>
#include<hash_map>
#include<stack>
#include<vector>
#include<list>
#include<set>
#include<queue>
#include<map>
#include<sstream>



using namespace std;
struct Node
{
	unsigned int value;//该节点入边上的值
	unsigned int state;//该节点的值
};


ifstream ifile;
ofstream ofile;
vector<list<Node> > nfa;
vector<list<Node> > dfa;
hash_map<string, string> idTore;//存储定义段中标识名到正则式的映射
hash_map<int, int> nfaTer2Action;//存储NFA终态到action表头对应内容。
hash_map<int, int> dfaTer2Action;//存储DFA终态到action表头对应内容
vector<string> actionTable;;//存储正则式的action
vector<int> nfaIsTer;//标记NFA中哪些是终态
map< set<int>, int > dfaNodeTable;
vector<int> dfaIsTer;//标记dfa中哪些是终结态。



int specialSign(char c);//判断特殊符号
void ChangeRe(string &re);//去掉[]
void ChangetoRe(string &re);//将标识符变为RE
bool isChaDig(char c);//判断当前字符是不是字母或数字
void addRemovedConcatenationOP(string &re);//添加.
string infixToPostfix(string expression);//中缀表达式变后缀表达式
void produceNfa(const string & re, vector<list<Node> > &tnfa, vector<int> & isTer, int index);//建立NFA
void modifyTer(vector<int> &is_t, unsigned int state, int value);//把最后一个状态设为终态
void mergeNFA(vector<list<Node> > &nfa1, const vector<list<Node> > &nfa2);//合并NFA
void joinIster(vector<int> &is_t1, const vector<int> &is_t2);//合并终态
void NFAToDFA();//NFA生成DFA
void Eclosure(set<int> &T);//求闭包
set<int> move(const set<int> &I, int value);//求通过value达到的NFA状态的集合
int dfaIsTerminated(set<int> &I);//返回终态对应动作，如果是非终态返回-1
void minidfa();
void genAnalysisCode();//生成词法分析部分的代码
bool checkState(vector<set<int>>&A, int begin, int fir, int sec);
bool equalState(const list<Node> &fir, const list<Node> &sec, vector<set<int>>&A, int begin);



int main()
{
	string s;
	cout << "Please input the inputfile name!" << endl;
	cin >> s;
	ifile.open(s.c_str(), ios::in);
	ofile.open("yylex.cpp", ios::out);
	ofile << "#define _CRT_SECURE_NO_WARNINGS" << endl;
	if (!ifile.good())
	{
		cerr << "Open the file error!" << endl;
		return 0;
	}
	char c = ifile.get();
	int judge = specialSign(c);
	//判断开头是不是%{
	if (judge != 0)
	{
		cout << "The input file has no correct formation,Please try again!\n";
		return 0;
	}
	//判断到%}或到文件尾为止进行扫描，表示辅助定义部分扫描完毕
	while (!ifile.eof() && judge != 1)
	{
		c = ifile.get();
		if (c == '%')
		{
			judge = specialSign(c);
			continue;
		}//当接受到%时，注意判断是不是特殊符号。
		ofile.put(c);
	}




	//以下开始对定义好的关键字的正规表达式的扫描，并将其存储到表中
	ifile.get();
	pair<string, string> pi;
	judge = -2;
	//循环读取，进行判断
	while (!ifile.eof() && judge != 2)
	{
		c = ifile.get();
		//回车则继续
		if (c == '\n')continue;
		//
		if (c == '%')
		{
			judge = specialSign(c);
			if (judge == -1)//用户自定义的标识符不可以含有此特殊字符%，即出现%后加了其他非规定的字符
			{
				cerr << "There is an error !" << endl;
				return 0;
			}
			continue;//跳过下面的正常串的处理，因为已经到终结。
		}
		else
		{
			ifile.unget();//如果不是特殊字符%，把当前字符放回流中。
		}
		string id, re;
		//读入正规表达式
		ifile >> id >> re;
		//去掉[]，如[0-9]变为0|1|2|3|4|5|6|7|8|9
		ChangeRe(re);
		//为pi赋值
		pi.first = id;
		pi.second = re;
		idTore.insert(pi);//加入hash map，便于搜索
	}
	//以上是对定义段的扫描


	//action
	judge = -2;
	ifile.get();
	//在actionTable中预加入begin，表示开始
	actionTable.push_back("begin");
	//读取剩余的文件，出循环条件为到文件尾或是读到%%
	while (!ifile.eof() && judge != 2)
	{
		c = ifile.get();
		if (c == '\n')continue;
		//读到第一个%，用specialSign判断，是否为%%
		if (c == '%')
		{
			judge = specialSign(c);
			if (judge == -1)//用户自定义的标识符不可以含有此特殊字符%。
			{
				cerr << "There is an error in line !" << endl;
				return 0;
			}
			continue;//跳过下面的正常串的处理，因为已经到终结。
		}
		else
		{
			ifile.unget();//如果不是特殊字符%，把当前字符放回流中。
		}
		//定义字符串1，2，re和action
		string str1;
		string str2;
		string str3;
		string temp;
		string onestr;
		string secstr;
		string re, action;
		//读取一整行到onestr
		getline(ifile, onestr);
		//*********************************************************
		temp = onestr;
		//getline(ifile, temp);
		stringstream ss(temp);
		ss >> str1 >> str2 >> str3;
		str2 = str2 + " " + str3;

		if (str1[0] == '\"'){
			str1 = str1.substr(1, str1.size() - 2);
			str1 = "\\" + str1;
		}
		re = str1;
		action = str2;
		actionTable.push_back(action);
		ChangetoRe(re);//{}情况,将{}包围的字符串转化，根据hash map中的映射关系
		//ChangeRe(re);
		addRemovedConcatenationOP(re);
		re = infixToPostfix(re);
		//cout << re << "\t" << action << endl;

		//生成NFA
		vector<list<Node> > nfa1;
		vector<int> isTer;
		int actionTableIndex = (int)actionTable.size() - 1;
		produceNfa(re, nfa1, isTer, actionTableIndex);
		mergeNFA(nfa, nfa1);//合并nfa


		unsigned int adjustp = nfaIsTer.size();
		joinIster(nfaIsTer, isTer);//合并状态表

		//保存action的内容
		for (unsigned int i = adjustp; i<nfaIsTer.size(); i++)
		{
			if (nfaIsTer[i])
			{
				pair<int, int> p;
				p.first = i;
				p.second = actionTable.size() - 1;
				nfaTer2Action.insert(p);
			}
		}
	}
	cout << "NFA生成完毕。" << endl;

	//生成DFA及DFA最小化
	NFAToDFA();

	minidfa();

	for (unsigned int i = 0; i<dfaIsTer.size(); i++)
	{
		if (dfaIsTer[i])
		{
			pair<int, int> p;
			p.first = i;
			p.second = dfaIsTer[i];
			nfaTer2Action.insert(p);
		}
	}

	cout << "DFA生成完毕。" << endl;


	//下面开始是最后一段，即用户自定义子例程段的输出。
	genAnalysisCode();
	c = 1;
	while ((c = ifile.get()) != -1)
	{
		ofile.put(c);
	}

	ifile.close();
	ofile.close();
	cout << "Done!" << endl;

	system("pause");

}

//判断是否是特殊符号
int specialSign(char c)
{
	//若c为%，则需要检查下一个字符
	if (c == '%')
	{
		char cc = ifile.get();
		switch (cc)
		{
			//%%的情况
		case '%':
			return 2;
			//%{的情况，辅助定义部分的开始
		case '{':
			return 0;
			//%}的情况，辅助定义部分的结束
		case '}':
			return 1;
		default:
			ifile.unget();
			break;
		}
	}
	return -1;
}

void ChangeRe(string &re)
{
	int length = re.size();
	string rechange;
	if (re[0] == '[')
	{
		for (int i = 1; i < length; i++)
		{
			if (isChaDig(re[i]))
			{
				rechange += re[i];
				rechange += '|';
			}
			//a-z类似情况，直接用ASCII表示
			else if (re[i] == '-')
			{
				for (int m = re[i - 1]; m < re[i + 1]; m++)
				{
					rechange += m + 1;
					rechange += '|';
				}
				i++;
			}
			//若为\，'\\'为\的转义，则需要再往后看一位
			else if (re[i] == '\\')
			{
				cout << re << endl;
				rechange += re[i];
				rechange += re[i + 1];
				rechange += '|';
				i++;
			}
		}
		re = rechange.substr(0, rechange.size() - 1);
	}
}

void ChangetoRe(string &re)
{
	stack<int> status;
	status.push(-1);
	int i = 0;
	while (i < re.size())
	{
		if (re[i] == '{')
			status.push(i);//记录下此时的位置
		if (re[i] == '}')
		{
			//读到},查看栈顶
			int prei = status.top();
			//读到-1，表示这个}没有与之配对的{
			if (prei < 0)
			{
				i++;
				continue;
			}
			//获得匹配的{}之间的长度
			int length = i - prei - 1;
			//截取该部分的string，保存到id
			string id = re.substr(prei + 1, length);
			//在hash map中寻找id，返回对应的处理string
			string replacestr = idTore[id];
			if (replacestr.empty()) continue;//表示其中的内容不是标识符，忽略不处理
			re.replace(prei, length + 2, replacestr);//+2表示包括{}一起处理，用hash map中存储的处理来替换re中相应部分
			//将i置成替换后的前一位，即prei之前一位，prei-1
			i = prei - 1;
		}
		i++;
	}
}

bool isChaDig(char c)
{
	if ((c >= 'a'&&c <= 'z' )||( c >= 'A'&&c <= 'Z' )|| (c >= '0'&&c <= '9'))
		return true;
	else return false;
}

void addRemovedConcatenationOP(string &re) {
	for (int i = 1; i < re.size(); i++) {
		//添加连接运算符(.)，第i个字符是字母或者数字，且前一个字符不为（或|,或者第i个字符为（同时前一个为\
		//.表示连接，如ab变为a.b.
		if ((isChaDig(re[i]) && re[i - 1] != '('&&re[i - 1] != '|') || (re[i] == '('&&re[i - 1] != '\\'))
		{
			re.insert(i, 1, '.');
			i++;
		}
	}
}

//中缀表达式转后缀表达式
string infixToPostfix(string expression) {
	//主要算法思想：
	//1.遇到操作数：直接输出（添加到后缀表达式中）
	//2.栈为空时，遇到运算符，直接入栈
	//3.遇到左括号：将其入栈
	//4.遇到右括号：执行出栈操作，并将出栈的元素输出，直到弹出栈的是左括号，左括号不输出。
	//5.遇到其他运算符：加减乘除：弹出所有优先级大于或者等于该运算符的栈顶元素，然后将该运算符入栈
	//6.最终将栈中的元素依次出栈，输出。
	string postfix;
	stack<char> operatorStack;//存储操作符的栈
	for (int i = 0; i<expression.length(); i++){
		char token = expression.at(i);
		if (token == '\0') {	//空格
			continue;
		}
		else if (token == '|') {
			//当前为|,弹出栈顶的*和.连接到postfix
			while (!(operatorStack.empty()) && (operatorStack.top() == '*' || operatorStack.top() == '.')){
				const char tmp1 = operatorStack.top();
				operatorStack.pop();
				postfix += tmp1;
			}
			//将token压栈
			operatorStack.push(token);
		}
		else if (token == '.') {
			//当前为.弹出栈顶的.，连接到postfix
			while (!(operatorStack.empty()) && operatorStack.top() == '.') {
				const char tmp1 = operatorStack.top();
				operatorStack.pop();
				postfix += tmp1;
			}
			operatorStack.push(token);
		}
		//此处为闭包符号，
		else if (token == '*') {
			cout << "here" << expression << endl;
			postfix += token;
		}
		else if (token == '(') {
			// (直接进栈
			operatorStack.push(token);
		}
		else if (token == ')') {
			//遇到右括号：执行出栈操作，并将出栈的元素输出，直到弹出栈的是左括号，左括号不输出。
			while (!(operatorStack.top() == '(')) {
				const char tmp1 = operatorStack.top();
				operatorStack.pop();
				postfix += tmp1;
			}
			operatorStack.pop();

		}
		//之前获取re时，对于"+"转化成了\\+
		//当是\时，不是操作符，表示转义，获取后一位，并且直接加入postfix
		else if (token == '\\')
		{
			postfix += token;
			i++;
			char token = expression.at(i);
			postfix += token;
		}
		//不是操作符，直接加入postfix
		else {
			postfix += token;
		}
	}
	// 阶段 2:将栈中的操作符出栈，加入postfix
	while (!(operatorStack.empty())) {
		const char tmp1 = operatorStack.top();
		operatorStack.pop();
		postfix += tmp1;
	}
	return postfix;
}

//把最后一个状态设为终态
void modifyTer(vector<int> &isT, unsigned int state, int value){
	if (state >= isT.size()){
		//新增最后一个状态
		isT.resize(state + 1);
	}
	//在action表的最后加上该节点
	isT[state] = value;
}
//从正规表达式构建NFA
void produceNfa(const string &re, vector<list<Node>> &tnfa, vector<int> &isTer, int index){

	int xgnext = 0;
	//用于判断+或*是普通运算符还是正闭包和闭包运算，其中1为普通运算符
	stack<vector<list<Node>>> operandStack;

	for (int i = 0; i < re.length(); i++){
		//遍历整个正规式
		char token = re[i];//读取re的第i个字符的值
		if (token == '\0'){
			//若读到空格则什么也不做
			continue;
		}
		else if (token == '+'){
			//若读到+，需要先判断此处+符号是否为正闭包运算里的+
			if (xgnext == 1){
				//若为普通运算符
				//建立新结点Node放入list中，再放入vector中形成NFA
				Node n;
				n.state = 1;
				n.value = token;
				list<Node> l;
				l.push_back(n);
				vector<list<Node>> v;
				v.push_back(l);
				operandStack.push(v);//将当前NFA先存储起来，根据下一个读到的字符判断是否需要对NFA进行修改操作
				xgnext = 0;//若后面再读到+则一定为闭包运算
			}
			else{
				//若为闭包运算
				//取出栈顶的NFA
				vector<list<Node>> nfa = operandStack.top();
				operandStack.pop();
				int size = (int)nfa.size();
				//闭包运算的实现
				vector<list<Node>> v;
				//将nfa复制到v中
				v.insert(v.end(), nfa.begin(), nfa.end());
				//编写相应与R+的NFA，其中R为之前构建好的NFA
				list<Node> l1;//对应于编号为size的节点
				Node n1;
				n1.value = -1;
				n1.state = size + 1;
				l1.push_back(n1);
				v.push_back(l1);
				list<Node> l2;//对应于编号为size+1的节点
				n1.value = -1;
				n1.state = size + 2;
				l2.push_back(n1);
				n1.value = -1;
				n1.state = size * 2 + 3;
				l2.push_back(n1);
				v.push_back(l2);
				//复制一个NFA，并进行相应的编号后移
				for (vector<list<Node>>::iterator iter = nfa.begin(); iter != nfa.end(); iter++){
					for (list<Node>::iterator itor = (*iter).begin(); itor != (*iter).end(); itor++){
						(*itor).state += size + 2;
					}
				}
				list<Node> l3;//对应于编号为2size+2
				n1.value = -1;
				n1.state = size + 2;
				l3.push_back(n1);
				n1.value = -1;
				n1.state = size * 2 + 3;
				l3.push_back(n1);
				nfa.push_back(l3);//此处push到nfa里，作为nfa的最后一个节点
				v.insert(v.end(), nfa.begin(), nfa.end());//将这个复制的NFA插入到整个NFA的最后
				list<Node> l4;//对应于编号为2size+3的节点
				n1.value = -1;
				n1.state = size * 2 + 4;
				l4.push_back(n1);
				v.push_back(l4);
				operandStack.push(v);
			}
		}
		else if (token == '*'){
			//判断是否为操作符或闭包运算
			if (xgnext == 1){
				Node n;
				n.state = 1;
				n.value = token;
				list<Node> l;
				l.push_back(n);
				vector<list<Node>> v;
				v.push_back(l);
				operandStack.push(v);//将当前NFA先存储起来，根据下一个读到的字符判断是否需要对NFA进行修改操作
				xgnext = 0;//若后面再读到*则一定为闭包运算
			}
			else{
				vector<list<Node>> nfa = operandStack.top();
				operandStack.pop();
				int size = (int)nfa.size();
				//进行闭包运算
				vector<list<Node>> v;
				list<Node> l1;//对应于编号为0的节点
				Node n1;
				n1.value = -1;
				n1.state = 1;
				l1.push_back(n1);
				n1.value = -1;
				n1.state = size + 2;
				l1.push_back(n1);
				v.push_back(l1);
				//将NFA的整体编号向后挪动一位
				for (vector<list<Node>>::iterator iter = nfa.begin(); iter != nfa.end(); iter++){
					for (list<Node>::iterator itor = (*iter).begin(); itor != (*iter).end(); itor++){
						(*itor).state += 1;
					}
				}
				list<Node> l3;//对应于编号为size+1
				n1.value = -1;
				n1.state = 1;
				l3.push_back(n1);
				n1.value = -1;
				n1.state = size + 2;
				l3.push_back(n1);
				nfa.push_back(l3);
				v.insert(v.end(), nfa.begin(), nfa.end());
				operandStack.push(v);
			}
		}
		else if (token == '\\') {
			//若读到\符号，则读到的是普通符号，而非闭包运算符
			if (xgnext == 1){
				Node n;
				n.state = 1;
				n.value = token;
				list<Node> l;
				l.push_back(n);
				vector<list<Node>> v;
				v.push_back(l);
				operandStack.push(v);//将当前NFA先存储起来，根据下一个读到的字符判断是否需要对NFA进行修改操作
			}
			else if (xgnext != 1){
				//若此时xgnext=0则将其置为1，继续读下一个字符
				xgnext = 1;
			}
		}
		else if (token == '|' || token == '.') {
			//若读到|或.，则应当为number、letter或关键字
			char op = token;//op为读取到的操作符
			//将存储在栈顶的NFA取出来，作为操作数1
			vector<list<Node>> op1 = operandStack.top();
			operandStack.pop();
			int size1 = (int)op1.size();
			//将存储在栈顶的NFA取出来，作为操作数2
			vector<list<Node>> op2 = operandStack.top();
			operandStack.pop();
			int size2 = (int)op2.size();
			//所代表的算式为op1 op op2
			if (op == '|') {//若读到的是/符号
				vector<list<Node>> v;
				list<Node> l1;//代表编号为0的节点
				Node n1;
				n1.value = -1;
				n1.state = 1;
				l1.push_back(n1);
				n1.value = -1;
				n1.state = size2 + 2;
				l1.push_back(n1);
				v.push_back(l1);
				for (vector<list<Node>>::iterator iter = op2.begin(); iter != op2.end(); iter++){
					for (list<Node>::iterator itor = (*iter).begin(); itor != (*iter).end(); itor++){
						(*itor).state += 1;
					}
				}
				list<Node> l2;//代表编号为size2+1的节点
				n1.value = -1;
				n1.state = size1 + size2 + 3;
				l2.push_back(n1);
				op2.push_back(l2);
				v.insert(v.end(), op2.begin(), op2.end());

				for (vector<list<Node>>::iterator iter = op1.begin(); iter != op1.end(); iter++){
					for (list<Node>::iterator itor = (*iter).begin(); itor != (*iter).end(); itor++){
						(*itor).state += size2 + 2;
					}
				}
				list<Node> l3;//代表编号为size2+size1+2的节点
				n1.value = -1;
				n1.state = size1 + size2 + 3;
				l3.push_back(n1);
				op1.push_back(l3);
				v.insert(v.end(), op1.begin(), op1.end());
				operandStack.push(v);
			}
			else if (op == '.') {//若读到的是.符号
				//最终形成的nfa为op2 op1对应的形式
				list<Node> l;//代表编号为size2的节点
				Node n1;
				n1.value = -1;
				n1.state = size2 + 1;
				l.push_back(n1);
				op2.push_back(l);
				for (vector<list<Node>>::iterator iter = op1.begin(); iter != op1.end(); iter++){
					for (list<Node>::iterator itor = (*iter).begin(); itor != (*iter).end(); itor++){
						(*itor).state += size2 + 1;
					}
				}
				//将op1挂在op2后面
				op2.insert(op2.end(), op1.begin(), op1.end());
				//将op2作为最终的NFA存储在栈里
				operandStack.push(op2);
			}
		}
		else{
			//若读到的是操作数，则构造一个NFA将其放入其中
			Node n;
			n.state = 1;
			n.value = token;
			list<Node> l;
			l.push_back(n);
			vector<list<Node>> v;
			v.push_back(l);
			operandStack.push(v);
		}
	}
	//正规式遍历完成，下面对栈里的NFA进行最后的处理，主要为添加终态
	tnfa = operandStack.top();
	operandStack.pop();
	int last_state = tnfa.back().back().state;
	modifyTer(isTer, last_state, index);//把最后一个状态置为终结态
	int recent = tnfa.size();
	//由于最后一个节点没有出边，所以需要单独给最后一个节点新增一个链表，用来代表最后一个节点
	tnfa.resize(recent + 1);
}

void mergeNFA(vector<list<Node> > &nfa1, const vector<list<Node> > &nfa2)
{
	// 合并NFA，将两个NFA合并为一个NFA
	// 将第二个NFA的开始节点中的全部内容复制到第一个NFA的开始节点
	// 将第二个NFA中除开始节点外的全部节点连接到第一个NFA的末尾
	// 改变第二个NFA节点的编号
	if (nfa1.empty())
	{
		nfa1 = nfa2;
		return;
	}
	Node con;	// 在nfa1的开始节点中加入一个新的节点，边上值为epslong，指向nfa2
	con.state = nfa1.size();
	con.value = -1;
	nfa1[0].push_back(con);

	size_t num = nfa1.size();
	// 把nfa2的所有点复制到nfa1
	copy(nfa2.begin(), nfa2.end(), back_inserter(nfa1));
	// 调整节点值
	for (int i = num; i < nfa1.size(); i++)
	{
		list<Node>::iterator p = nfa1[i].begin();
		while (p != nfa1[i].end())
		{
			p->state += num;
			p++;
		}
	}
}

void joinIster(vector<int> &is_t1, const vector<int> &is_t2)
{
	// 合并终态
	copy(is_t2.begin(), is_t2.end(), back_inserter(is_t1));
}

void NFAToDFA()
{
	if (nfa.size() == 1)
	{
		// nfa只有一个节点时不需要转换，直接相等即可
		dfa = nfa;
		dfaIsTer = nfaIsTer;
		dfaTer2Action = nfaTer2Action;
		return;
	}

	set<int> I0;	// 初始状态子集
	queue<set<int> > Q; //用来存放已经生成的状态集，并用于判断是否结束。
	map<int, set<int> > valueTable;//以边上权值为关键字，对应内容为相应的新的状态集。
	I0.insert(0); // 将初始状态放入首字符集中
	Eclosure(I0); // 求闭包
	int current_state = 0;	// 状态子集的标号（I0，I1，I2，I3...）
	pair<set<int>, int> ap; // pair是将2个数据组合成一个数据，可以通过pair.first与pair.secend进行调用
	ap.first = I0;
	ap.second = current_state;
	dfaNodeTable.insert(ap);	// 将子集与子集编号插入dfa节点集
	Q.push(I0);		// 将子集放入队列中
	do
	{
		set<int> It = Q.front();
		Q.pop();
		valueTable.clear();//清空以进行新一轮的判断
		//取出新的一个状态集进行判断，如果当中包含了nfa的终态，则需要进行特殊处理。
		for (set<int>::iterator p = It.begin(); p != It.end(); p++)
		{
			int s = *p;		// s为当前子集中的节点元素state值
			unsigned int state = dfaNodeTable[It];
			if (dfa.size() <= state)	//有新的dfa节点，需要拓展dfa
			{
				dfa.resize(state + 1);
				dfaIsTer.resize(state + 1);
			}
			dfaIsTer[state] = dfaIsTerminated(It);//确定该状态集是不是一个终态
			for (list<Node>::iterator i = nfa[s].begin(); i != nfa[s].end(); i++)
			{
				//i为节点s所有下一跳的节点，i->value为节点s出边上的值
				//每做一次，完成一个新的状态集的生成
				//并同时应完成相应DFA的构造
				//以下的判断是对每一状态点只对每种边做一次操作
				if (!valueTable.count(i->value) && i->value != -1)
				{
					set<int> item;
					item = move(It, i->value);	// 遍历查找It集合中元素下一状态对应的集合，组成新的子集
					Eclosure(item);
					if (!dfaNodeTable.count(item))
					{
						Q.push(item);		// 将新的子集放入Q中，并改变dfa状态值
						current_state++;
						pair<set<int>, int> ap;
						ap.first = item;
						ap.second = current_state;
						dfaNodeTable.insert(ap);	// 将dfa节点state值与对应的子集放入dfaNodeTable

					}
					//将上一步生成的状态集作为一个新的集合放入到valueTable中
					//此处可能会有所重复，因为当dfaNodeTable中已经含有相应的状态集的时候，
					//这里可能还是会有操作，多余。
					pair<int, set<int> > tp;
					tp.first = i->value;
					tp.second = item;
					valueTable.insert(tp);
					//下面构造相应的DFA，构造节点，将节点放到dfa中
					Node n0;
					n0.state = dfaNodeTable[item];
					n0.value = i->value;
					dfa[state].push_back(n0);
				}
			}
		}
	} while (!Q.empty());
	//dfa.resize(dfa.size()+1);
	//处理完毕，DFA生成，同时也产生对应action的dfa终态表
	//此处有一个问题，即如何解决临界问题
}

void Eclosure(set<int> &T)
{
	// 遍历集合T中的结点，判断每个节点的出边是不是epsilon，求闭包
	stack<int> M;
	set<int>::iterator p = T.begin();
	while (p != T.end())
	{
		M.push(*p);
		p++;
	}
	while (!M.empty())
	{
		int s = M.top();
		M.pop();
		list<Node>::iterator p = nfa[s].begin();
		while (p != nfa[s].end())
		{
			if (p->value == -1 && (!T.count(p->state)))
			{
				M.push(p->state);
				T.insert(p->state);
			}
			p++;
		}
	}
}

set<int> move(const set<int> &I, int value)//从集合里面读，然后进行move()计算
{
	set<int> r;
	set<int>::const_iterator p = I.begin();
	while (p != I.end())
	{
		int s = *p;
		list<Node>::iterator i = nfa[s].begin();
		while (i != nfa[s].end())
		{
			if (i->value == value && (!r.count(i->state)))
			{
				r.insert(i->state);
			}
			i++;
		}
		p++;
	}
	return r;
}

int dfaIsTerminated(set<int> &I)
{
	// 遍历查找集合I中是否存在终态
	for (set<int>::iterator p = I.begin(); p != I.end(); p++)
	{
		if (nfaIsTer[*p])
		{
			return nfaTer2Action[*p];
		}
	}
	return 0;
}

//检查在已经分好的状态子集中是否有某一个子集既包含fir又包含sec
bool checkState(vector<set<int>>&A, int begin, int fir, int sec){
	for (int i = 0; i < begin; i++){
		if (A[i].count(fir) && A[i].count(sec))
			return true;
	}
	return false;
}
bool equalState(const list<Node> &fir, const list<Node> &sec, vector<set<int>>&A, int begin){
	bool flag = false;
	if (fir.size() != sec.size()) return false;
	for (list<Node>::const_iterator p1 = fir.begin(); p1 != fir.end(); p1++){
		flag = false;
		for (list<Node>::const_iterator p2 = sec.begin(); p2 != sec.end(); p2++){
			if (p1->value == p2->value){//若fir和sec中某个节点的出边值一样
				if (checkState(A, begin, p1->state, p2->state)){
					//判断A中是否有某一个状态子集包含上述出边值一样的两个节点
					//若包含，则在这一次的循环中满足了等价的条件，离开本次循环，开始下一次循环
					flag = true;
					break;
				}
				return false;
			}
		}
		if (flag == false) return false;
	}
	return true;
}
//最小化DFA
void minidfa(){
	if (dfa.size() == 1) return;
	vector<set<int>> A(dfa.size() + 1);
	int begin = 0;
	//存储所有终结态状态值
	for (int i = 0; i < dfa.size(); i++){
		if (dfaIsTer[i] != 0)
			A[begin].insert(i);
	}
	begin++;
	//存储所有非终结态状态值
	for (int i = 0; i < dfa.size(); i++)
	{
		if (dfaIsTer[i] == 0)
			A[begin].insert(i);
	}

	bool flag = true;//判断是否已分割完成，完成为false，未完成为true
	while (flag){
		flag = false;
		for (int i = 0; i <= begin; i++){//遍历当前所有的状态set
			for (set<int>::iterator p = A[i].begin(); p != A[i].end(); ){//遍历当前set内的所有节点
				begin++;//新分区，用于存储新分割出来的状态集合
				if (A[i].size() > 1 && dfa[*p].empty()){//当前的set中点的数量大于1且所遍历到的节点无后项结点
					flag = true;
					//将该节点放到新的分区中，并在原来的分区中删除
					A[begin].insert(*p);
					p = A[i].erase(p);
					continue;//继续遍历该set中下一个结点
				}
				for (set<int>::iterator q = A[i].begin(); q != A[i].end(); ){//如果有后项节点或者size大于1
					//若扫描到同一节点则跳过
					if (p == q)
					{
						q++;
						continue;
					}
					//若不是等价状态，则删除原来节点，并把它加入新的分区中
					if (!equalState(dfa[*p], dfa[*q], A, begin))
					{
						flag = true;
						A[begin].insert(*q);
						q = A[i].erase(q);
					}
					else{
						//否则继续向后扫描
						q++;
					}
				}
				if (A[begin].empty()) begin--;//若当前set经过转移之后为空，则删除当前分区
				p++;
			}
		}
	}
	//现在得到的是分好状态的A，包含begin+1个状态子集
	//下面进行的是根据这些状态子集构建最小化DFA
	//只需要处理子集中包含大于一个状态的集合
	//首先统计这样的集合
	vector<int> num;
	for (int i = 0; i <= begin; i++)
	{
		if (A[i].size() != 1)
			num.push_back(i);
	}
	//处理这些子集
	for (int i = 0; i < num.size(); i++){
		int der = 0;//记录最终生成的节点的状态值
		for (set<int>::iterator p = A[num[i]].begin(); p != A[num[i]].end(); p++){
			//遍历A中的这些集合
			//将这些集合中的第一个元素的状态值记为整个节点的状态值
			if (p == A[num[i]].begin())
				der = *p;
			else{
				for (list<Node>::iterator it = dfa[*p].begin(); it != dfa[*p].end(); it++){
					//遍历dfa中这些节点对应的链表
					bool inse = false;
					for (list<Node>::iterator at = dfa[der].begin(); at != dfa[der].end(); at++){
						//遍历第一个元素在dfa中对应的链表
						//判断是否有相同的元素it与at，若没有则需要将这个节点放入代表元素的链表中
						if (at->state == it->state&&at->value == it->value){
							inse = true;
							break;
						}
					}
					if (!inse)
						dfa[der].push_back(*it);
				}
				dfa[*p].clear();//清空多余的不需要的节点
				//不再需要这个节点，因此将其置入终结态集合中
				if (dfaIsTer[*p] != 0) dfaIsTer[*p] = 0;
				//替换节点中的状态号
				for (int k = 0; k < dfa.size(); k++){
					//遍历原有dfa的所有节点
					if (!dfa[k].empty()){
						//如果在上面转换的时候没有被清空
						for (list<Node>::iterator it = dfa[k].begin(); it != dfa[k].end();){
							//遍历dfa中该节点的链表中的所有节点，找到相对应的具有相同的状态值的节点进行节点修改操作
							bool judge = false;
							if (it->state == *p){
								//若遍历到的节点的状态值正好等于该状态子集所遍历到的状态值
								for (list<Node>::iterator at = dfa[k].begin(); at != dfa[k].end(); at++){
									//再次遍历dfa中该节点的链表中所有节点，从而在dfa的节点链表上删除该节点
									if (at->state == der&&at->value == it->value)
									{
										it = dfa[k].erase(it);
										judge = true;//表示下面需要修改节点状态值，创造新结点
										break;
									}
								}
								if (!judge)
								{
									Node in;
									in.value = it->value;
									in.state = der;//新结点的状态值等于最初确定的状态子集的首位元素的状态值
									it = dfa[k].erase(it);
									dfa[k].push_back(in);
								}
							}
							else{
								it++;
							}
						}
					}
				}
			}
		}
	}
}
//根据最小化的DFA生成控制代码
void genAnalysisCode(){
	//生成一些头部代码
	ofile << "using namespace std;" << endl;
	ofile << "const int START=0;" << endl;
	ofile << "const int ERROR=32767;" << endl;
	ofile << "string analysis(char *yytext,int n)" << endl;
	ofile << "{" << endl;
	ofile << "\tint state=START;" << endl;
	ofile << "\tint N=n+1;" << endl;
	ofile << "\tfor(int i=0;i<N;i++)" << endl;
	ofile << "\t{" << endl;
	ofile << "\t\tswitch(state)" << endl;
	ofile << "\t\t{" << endl;

	for (int i = 0; i < dfa.size(); i++){
		//遍历整个dfa中的节点
		ofile << "\t\t\tcase " << i << ":" << endl;
		ofile << "\t\t\t{" << endl;
		if (dfaIsTer[i]>0){
			//如果该状态是终态的话，判断当前是否直接输出终态结果
			//若需要输出终态结果，即读到串尾
			ofile << "\t\t\t\tif(i==N-1)" << endl;
			ofile << "\t\t\t\t{" << endl;
			//打印当前对应的正则式的结果
			size_t length = actionTable[dfaIsTer[i]].size();
			ofile << "\t\t\t\t\t" << actionTable[dfaIsTer[i]].substr(0, length) << endl;
			ofile << "\t\t\t\t\tbreak;" << endl;
			ofile << "\t\t\t\t}" << endl;
		}
		for (list<Node>::iterator p = dfa[i].begin(); p != dfa[i].end(); p++){
			//遍历dfa[i]的链表
			ofile << "\t\t\t\tif(yytext[i]=='" << (char)p->value << "')" << endl;
			ofile << "\t\t\t\t{" << endl;
			ofile << "\t\t\t\t\tstate=" << p->state << ";" << endl;
			ofile << "\t\t\t\t\tbreak;" << endl;
			ofile << "\t\t\t\t}" << endl;
		}
		//错误判断：若code读到最后仍不处于终态中，则判为错误情况
		if (dfa[i].size()>0)
		{
			ofile << "\t\t\t\telse" << endl;
			ofile << "\t\t\t\t{" << endl;
			ofile << "\t\t\t\t\treturn \"ERROR\";" << endl;
			ofile << "\t\t\t\t}" << endl;
		}
		ofile << "\t\t\t\tbreak;" << endl;
		ofile << "\t\t\t}" << endl;
	}
	ofile << "\t\t}" << endl;;
	ofile << "\t}" << endl;
	ofile << "}";
}