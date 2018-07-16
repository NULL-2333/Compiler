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
	unsigned int value;//�ýڵ�����ϵ�ֵ
	unsigned int state;//�ýڵ��ֵ
};


ifstream ifile;
ofstream ofile;
vector<list<Node> > nfa;
vector<list<Node> > dfa;
hash_map<string, string> idTore;//�洢������б�ʶ��������ʽ��ӳ��
hash_map<int, int> nfaTer2Action;//�洢NFA��̬��action��ͷ��Ӧ���ݡ�
hash_map<int, int> dfaTer2Action;//�洢DFA��̬��action��ͷ��Ӧ����
vector<string> actionTable;;//�洢����ʽ��action
vector<int> nfaIsTer;//���NFA����Щ����̬
map< set<int>, int > dfaNodeTable;
vector<int> dfaIsTer;//���dfa����Щ���ս�̬��



int specialSign(char c);//�ж��������
void ChangeRe(string &re);//ȥ��[]
void ChangetoRe(string &re);//����ʶ����ΪRE
bool isChaDig(char c);//�жϵ�ǰ�ַ��ǲ�����ĸ������
void addRemovedConcatenationOP(string &re);//���.
string infixToPostfix(string expression);//��׺���ʽ���׺���ʽ
void produceNfa(const string & re, vector<list<Node> > &tnfa, vector<int> & isTer, int index);//����NFA
void modifyTer(vector<int> &is_t, unsigned int state, int value);//�����һ��״̬��Ϊ��̬
void mergeNFA(vector<list<Node> > &nfa1, const vector<list<Node> > &nfa2);//�ϲ�NFA
void joinIster(vector<int> &is_t1, const vector<int> &is_t2);//�ϲ���̬
void NFAToDFA();//NFA����DFA
void Eclosure(set<int> &T);//��հ�
set<int> move(const set<int> &I, int value);//��ͨ��value�ﵽ��NFA״̬�ļ���
int dfaIsTerminated(set<int> &I);//������̬��Ӧ����������Ƿ���̬����-1
void minidfa();
void genAnalysisCode();//���ɴʷ��������ֵĴ���
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
	//�жϿ�ͷ�ǲ���%{
	if (judge != 0)
	{
		cout << "The input file has no correct formation,Please try again!\n";
		return 0;
	}
	//�жϵ�%}���ļ�βΪֹ����ɨ�裬��ʾ�������岿��ɨ�����
	while (!ifile.eof() && judge != 1)
	{
		c = ifile.get();
		if (c == '%')
		{
			judge = specialSign(c);
			continue;
		}//�����ܵ�%ʱ��ע���ж��ǲ���������š�
		ofile.put(c);
	}




	//���¿�ʼ�Զ���õĹؼ��ֵ�������ʽ��ɨ�裬������洢������
	ifile.get();
	pair<string, string> pi;
	judge = -2;
	//ѭ����ȡ�������ж�
	while (!ifile.eof() && judge != 2)
	{
		c = ifile.get();
		//�س������
		if (c == '\n')continue;
		//
		if (c == '%')
		{
			judge = specialSign(c);
			if (judge == -1)//�û��Զ���ı�ʶ�������Ժ��д������ַ�%��������%����������ǹ涨���ַ�
			{
				cerr << "There is an error !" << endl;
				return 0;
			}
			continue;//����������������Ĵ�����Ϊ�Ѿ����սᡣ
		}
		else
		{
			ifile.unget();//������������ַ�%���ѵ�ǰ�ַ��Ż����С�
		}
		string id, re;
		//����������ʽ
		ifile >> id >> re;
		//ȥ��[]����[0-9]��Ϊ0|1|2|3|4|5|6|7|8|9
		ChangeRe(re);
		//Ϊpi��ֵ
		pi.first = id;
		pi.second = re;
		idTore.insert(pi);//����hash map����������
	}
	//�����ǶԶ���ε�ɨ��


	//action
	judge = -2;
	ifile.get();
	//��actionTable��Ԥ����begin����ʾ��ʼ
	actionTable.push_back("begin");
	//��ȡʣ����ļ�����ѭ������Ϊ���ļ�β���Ƕ���%%
	while (!ifile.eof() && judge != 2)
	{
		c = ifile.get();
		if (c == '\n')continue;
		//������һ��%����specialSign�жϣ��Ƿ�Ϊ%%
		if (c == '%')
		{
			judge = specialSign(c);
			if (judge == -1)//�û��Զ���ı�ʶ�������Ժ��д������ַ�%��
			{
				cerr << "There is an error in line !" << endl;
				return 0;
			}
			continue;//����������������Ĵ�����Ϊ�Ѿ����սᡣ
		}
		else
		{
			ifile.unget();//������������ַ�%���ѵ�ǰ�ַ��Ż����С�
		}
		//�����ַ���1��2��re��action
		string str1;
		string str2;
		string str3;
		string temp;
		string onestr;
		string secstr;
		string re, action;
		//��ȡһ���е�onestr
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
		ChangetoRe(re);//{}���,��{}��Χ���ַ���ת��������hash map�е�ӳ���ϵ
		//ChangeRe(re);
		addRemovedConcatenationOP(re);
		re = infixToPostfix(re);
		//cout << re << "\t" << action << endl;

		//����NFA
		vector<list<Node> > nfa1;
		vector<int> isTer;
		int actionTableIndex = (int)actionTable.size() - 1;
		produceNfa(re, nfa1, isTer, actionTableIndex);
		mergeNFA(nfa, nfa1);//�ϲ�nfa


		unsigned int adjustp = nfaIsTer.size();
		joinIster(nfaIsTer, isTer);//�ϲ�״̬��

		//����action������
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
	cout << "NFA������ϡ�" << endl;

	//����DFA��DFA��С��
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

	cout << "DFA������ϡ�" << endl;


	//���濪ʼ�����һ�Σ����û��Զ��������̶ε������
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

//�ж��Ƿ����������
int specialSign(char c)
{
	//��cΪ%������Ҫ�����һ���ַ�
	if (c == '%')
	{
		char cc = ifile.get();
		switch (cc)
		{
			//%%�����
		case '%':
			return 2;
			//%{��������������岿�ֵĿ�ʼ
		case '{':
			return 0;
			//%}��������������岿�ֵĽ���
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
			//a-z���������ֱ����ASCII��ʾ
			else if (re[i] == '-')
			{
				for (int m = re[i - 1]; m < re[i + 1]; m++)
				{
					rechange += m + 1;
					rechange += '|';
				}
				i++;
			}
			//��Ϊ\��'\\'Ϊ\��ת�壬����Ҫ������һλ
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
			status.push(i);//��¼�´�ʱ��λ��
		if (re[i] == '}')
		{
			//����},�鿴ջ��
			int prei = status.top();
			//����-1����ʾ���}û����֮��Ե�{
			if (prei < 0)
			{
				i++;
				continue;
			}
			//���ƥ���{}֮��ĳ���
			int length = i - prei - 1;
			//��ȡ�ò��ֵ�string�����浽id
			string id = re.substr(prei + 1, length);
			//��hash map��Ѱ��id�����ض�Ӧ�Ĵ���string
			string replacestr = idTore[id];
			if (replacestr.empty()) continue;//��ʾ���е����ݲ��Ǳ�ʶ�������Բ�����
			re.replace(prei, length + 2, replacestr);//+2��ʾ����{}һ������hash map�д洢�Ĵ������滻re����Ӧ����
			//��i�ó��滻���ǰһλ����prei֮ǰһλ��prei-1
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
		//������������(.)����i���ַ�����ĸ�������֣���ǰһ���ַ���Ϊ����|,���ߵ�i���ַ�Ϊ��ͬʱǰһ��Ϊ\
		//.��ʾ���ӣ���ab��Ϊa.b.
		if ((isChaDig(re[i]) && re[i - 1] != '('&&re[i - 1] != '|') || (re[i] == '('&&re[i - 1] != '\\'))
		{
			re.insert(i, 1, '.');
			i++;
		}
	}
}

//��׺���ʽת��׺���ʽ
string infixToPostfix(string expression) {
	//��Ҫ�㷨˼�룺
	//1.������������ֱ���������ӵ���׺���ʽ�У�
	//2.ջΪ��ʱ�������������ֱ����ջ
	//3.���������ţ�������ջ
	//4.���������ţ�ִ�г�ջ������������ջ��Ԫ�������ֱ������ջ���������ţ������Ų������
	//5.����������������Ӽ��˳��������������ȼ����ڻ��ߵ��ڸ��������ջ��Ԫ�أ�Ȼ�󽫸��������ջ
	//6.���ս�ջ�е�Ԫ�����γ�ջ�������
	string postfix;
	stack<char> operatorStack;//�洢��������ջ
	for (int i = 0; i<expression.length(); i++){
		char token = expression.at(i);
		if (token == '\0') {	//�ո�
			continue;
		}
		else if (token == '|') {
			//��ǰΪ|,����ջ����*��.���ӵ�postfix
			while (!(operatorStack.empty()) && (operatorStack.top() == '*' || operatorStack.top() == '.')){
				const char tmp1 = operatorStack.top();
				operatorStack.pop();
				postfix += tmp1;
			}
			//��tokenѹջ
			operatorStack.push(token);
		}
		else if (token == '.') {
			//��ǰΪ.����ջ����.�����ӵ�postfix
			while (!(operatorStack.empty()) && operatorStack.top() == '.') {
				const char tmp1 = operatorStack.top();
				operatorStack.pop();
				postfix += tmp1;
			}
			operatorStack.push(token);
		}
		//�˴�Ϊ�հ����ţ�
		else if (token == '*') {
			cout << "here" << expression << endl;
			postfix += token;
		}
		else if (token == '(') {
			// (ֱ�ӽ�ջ
			operatorStack.push(token);
		}
		else if (token == ')') {
			//���������ţ�ִ�г�ջ������������ջ��Ԫ�������ֱ������ջ���������ţ������Ų������
			while (!(operatorStack.top() == '(')) {
				const char tmp1 = operatorStack.top();
				operatorStack.pop();
				postfix += tmp1;
			}
			operatorStack.pop();

		}
		//֮ǰ��ȡreʱ������"+"ת������\\+
		//����\ʱ�����ǲ���������ʾת�壬��ȡ��һλ������ֱ�Ӽ���postfix
		else if (token == '\\')
		{
			postfix += token;
			i++;
			char token = expression.at(i);
			postfix += token;
		}
		//���ǲ�������ֱ�Ӽ���postfix
		else {
			postfix += token;
		}
	}
	// �׶� 2:��ջ�еĲ�������ջ������postfix
	while (!(operatorStack.empty())) {
		const char tmp1 = operatorStack.top();
		operatorStack.pop();
		postfix += tmp1;
	}
	return postfix;
}

//�����һ��״̬��Ϊ��̬
void modifyTer(vector<int> &isT, unsigned int state, int value){
	if (state >= isT.size()){
		//�������һ��״̬
		isT.resize(state + 1);
	}
	//��action��������ϸýڵ�
	isT[state] = value;
}
//��������ʽ����NFA
void produceNfa(const string &re, vector<list<Node>> &tnfa, vector<int> &isTer, int index){

	int xgnext = 0;
	//�����ж�+��*����ͨ������������հ��ͱհ����㣬����1Ϊ��ͨ�����
	stack<vector<list<Node>>> operandStack;

	for (int i = 0; i < re.length(); i++){
		//������������ʽ
		char token = re[i];//��ȡre�ĵ�i���ַ���ֵ
		if (token == '\0'){
			//�������ո���ʲôҲ����
			continue;
		}
		else if (token == '+'){
			//������+����Ҫ���жϴ˴�+�����Ƿ�Ϊ���հ��������+
			if (xgnext == 1){
				//��Ϊ��ͨ�����
				//�����½��Node����list�У��ٷ���vector���γ�NFA
				Node n;
				n.state = 1;
				n.value = token;
				list<Node> l;
				l.push_back(n);
				vector<list<Node>> v;
				v.push_back(l);
				operandStack.push(v);//����ǰNFA�ȴ洢������������һ���������ַ��ж��Ƿ���Ҫ��NFA�����޸Ĳ���
				xgnext = 0;//�������ٶ���+��һ��Ϊ�հ�����
			}
			else{
				//��Ϊ�հ�����
				//ȡ��ջ����NFA
				vector<list<Node>> nfa = operandStack.top();
				operandStack.pop();
				int size = (int)nfa.size();
				//�հ������ʵ��
				vector<list<Node>> v;
				//��nfa���Ƶ�v��
				v.insert(v.end(), nfa.begin(), nfa.end());
				//��д��Ӧ��R+��NFA������RΪ֮ǰ�����õ�NFA
				list<Node> l1;//��Ӧ�ڱ��Ϊsize�Ľڵ�
				Node n1;
				n1.value = -1;
				n1.state = size + 1;
				l1.push_back(n1);
				v.push_back(l1);
				list<Node> l2;//��Ӧ�ڱ��Ϊsize+1�Ľڵ�
				n1.value = -1;
				n1.state = size + 2;
				l2.push_back(n1);
				n1.value = -1;
				n1.state = size * 2 + 3;
				l2.push_back(n1);
				v.push_back(l2);
				//����һ��NFA����������Ӧ�ı�ź���
				for (vector<list<Node>>::iterator iter = nfa.begin(); iter != nfa.end(); iter++){
					for (list<Node>::iterator itor = (*iter).begin(); itor != (*iter).end(); itor++){
						(*itor).state += size + 2;
					}
				}
				list<Node> l3;//��Ӧ�ڱ��Ϊ2size+2
				n1.value = -1;
				n1.state = size + 2;
				l3.push_back(n1);
				n1.value = -1;
				n1.state = size * 2 + 3;
				l3.push_back(n1);
				nfa.push_back(l3);//�˴�push��nfa���Ϊnfa�����һ���ڵ�
				v.insert(v.end(), nfa.begin(), nfa.end());//��������Ƶ�NFA���뵽����NFA�����
				list<Node> l4;//��Ӧ�ڱ��Ϊ2size+3�Ľڵ�
				n1.value = -1;
				n1.state = size * 2 + 4;
				l4.push_back(n1);
				v.push_back(l4);
				operandStack.push(v);
			}
		}
		else if (token == '*'){
			//�ж��Ƿ�Ϊ��������հ�����
			if (xgnext == 1){
				Node n;
				n.state = 1;
				n.value = token;
				list<Node> l;
				l.push_back(n);
				vector<list<Node>> v;
				v.push_back(l);
				operandStack.push(v);//����ǰNFA�ȴ洢������������һ���������ַ��ж��Ƿ���Ҫ��NFA�����޸Ĳ���
				xgnext = 0;//�������ٶ���*��һ��Ϊ�հ�����
			}
			else{
				vector<list<Node>> nfa = operandStack.top();
				operandStack.pop();
				int size = (int)nfa.size();
				//���бհ�����
				vector<list<Node>> v;
				list<Node> l1;//��Ӧ�ڱ��Ϊ0�Ľڵ�
				Node n1;
				n1.value = -1;
				n1.state = 1;
				l1.push_back(n1);
				n1.value = -1;
				n1.state = size + 2;
				l1.push_back(n1);
				v.push_back(l1);
				//��NFA�����������Ų��һλ
				for (vector<list<Node>>::iterator iter = nfa.begin(); iter != nfa.end(); iter++){
					for (list<Node>::iterator itor = (*iter).begin(); itor != (*iter).end(); itor++){
						(*itor).state += 1;
					}
				}
				list<Node> l3;//��Ӧ�ڱ��Ϊsize+1
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
			//������\���ţ������������ͨ���ţ����Ǳհ������
			if (xgnext == 1){
				Node n;
				n.state = 1;
				n.value = token;
				list<Node> l;
				l.push_back(n);
				vector<list<Node>> v;
				v.push_back(l);
				operandStack.push(v);//����ǰNFA�ȴ洢������������һ���������ַ��ж��Ƿ���Ҫ��NFA�����޸Ĳ���
			}
			else if (xgnext != 1){
				//����ʱxgnext=0������Ϊ1����������һ���ַ�
				xgnext = 1;
			}
		}
		else if (token == '|' || token == '.') {
			//������|��.����Ӧ��Ϊnumber��letter��ؼ���
			char op = token;//opΪ��ȡ���Ĳ�����
			//���洢��ջ����NFAȡ��������Ϊ������1
			vector<list<Node>> op1 = operandStack.top();
			operandStack.pop();
			int size1 = (int)op1.size();
			//���洢��ջ����NFAȡ��������Ϊ������2
			vector<list<Node>> op2 = operandStack.top();
			operandStack.pop();
			int size2 = (int)op2.size();
			//���������ʽΪop1 op op2
			if (op == '|') {//����������/����
				vector<list<Node>> v;
				list<Node> l1;//������Ϊ0�Ľڵ�
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
				list<Node> l2;//������Ϊsize2+1�Ľڵ�
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
				list<Node> l3;//������Ϊsize2+size1+2�Ľڵ�
				n1.value = -1;
				n1.state = size1 + size2 + 3;
				l3.push_back(n1);
				op1.push_back(l3);
				v.insert(v.end(), op1.begin(), op1.end());
				operandStack.push(v);
			}
			else if (op == '.') {//����������.����
				//�����γɵ�nfaΪop2 op1��Ӧ����ʽ
				list<Node> l;//������Ϊsize2�Ľڵ�
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
				//��op1����op2����
				op2.insert(op2.end(), op1.begin(), op1.end());
				//��op2��Ϊ���յ�NFA�洢��ջ��
				operandStack.push(op2);
			}
		}
		else{
			//���������ǲ�����������һ��NFA�����������
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
	//����ʽ������ɣ������ջ���NFA�������Ĵ�����ҪΪ�����̬
	tnfa = operandStack.top();
	operandStack.pop();
	int last_state = tnfa.back().back().state;
	modifyTer(isTer, last_state, index);//�����һ��״̬��Ϊ�ս�̬
	int recent = tnfa.size();
	//�������һ���ڵ�û�г��ߣ�������Ҫ���������һ���ڵ�����һ�����������������һ���ڵ�
	tnfa.resize(recent + 1);
}

void mergeNFA(vector<list<Node> > &nfa1, const vector<list<Node> > &nfa2)
{
	// �ϲ�NFA��������NFA�ϲ�Ϊһ��NFA
	// ���ڶ���NFA�Ŀ�ʼ�ڵ��е�ȫ�����ݸ��Ƶ���һ��NFA�Ŀ�ʼ�ڵ�
	// ���ڶ���NFA�г���ʼ�ڵ����ȫ���ڵ����ӵ���һ��NFA��ĩβ
	// �ı�ڶ���NFA�ڵ�ı��
	if (nfa1.empty())
	{
		nfa1 = nfa2;
		return;
	}
	Node con;	// ��nfa1�Ŀ�ʼ�ڵ��м���һ���µĽڵ㣬����ֵΪepslong��ָ��nfa2
	con.state = nfa1.size();
	con.value = -1;
	nfa1[0].push_back(con);

	size_t num = nfa1.size();
	// ��nfa2�����е㸴�Ƶ�nfa1
	copy(nfa2.begin(), nfa2.end(), back_inserter(nfa1));
	// �����ڵ�ֵ
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
	// �ϲ���̬
	copy(is_t2.begin(), is_t2.end(), back_inserter(is_t1));
}

void NFAToDFA()
{
	if (nfa.size() == 1)
	{
		// nfaֻ��һ���ڵ�ʱ����Ҫת����ֱ����ȼ���
		dfa = nfa;
		dfaIsTer = nfaIsTer;
		dfaTer2Action = nfaTer2Action;
		return;
	}

	set<int> I0;	// ��ʼ״̬�Ӽ�
	queue<set<int> > Q; //��������Ѿ����ɵ�״̬�����������ж��Ƿ������
	map<int, set<int> > valueTable;//�Ա���ȨֵΪ�ؼ��֣���Ӧ����Ϊ��Ӧ���µ�״̬����
	I0.insert(0); // ����ʼ״̬�������ַ�����
	Eclosure(I0); // ��հ�
	int current_state = 0;	// ״̬�Ӽ��ı�ţ�I0��I1��I2��I3...��
	pair<set<int>, int> ap; // pair�ǽ�2��������ϳ�һ�����ݣ�����ͨ��pair.first��pair.secend���е���
	ap.first = I0;
	ap.second = current_state;
	dfaNodeTable.insert(ap);	// ���Ӽ����Ӽ���Ų���dfa�ڵ㼯
	Q.push(I0);		// ���Ӽ����������
	do
	{
		set<int> It = Q.front();
		Q.pop();
		valueTable.clear();//����Խ�����һ�ֵ��ж�
		//ȡ���µ�һ��״̬�������жϣ�������а�����nfa����̬������Ҫ�������⴦��
		for (set<int>::iterator p = It.begin(); p != It.end(); p++)
		{
			int s = *p;		// sΪ��ǰ�Ӽ��еĽڵ�Ԫ��stateֵ
			unsigned int state = dfaNodeTable[It];
			if (dfa.size() <= state)	//���µ�dfa�ڵ㣬��Ҫ��չdfa
			{
				dfa.resize(state + 1);
				dfaIsTer.resize(state + 1);
			}
			dfaIsTer[state] = dfaIsTerminated(It);//ȷ����״̬���ǲ���һ����̬
			for (list<Node>::iterator i = nfa[s].begin(); i != nfa[s].end(); i++)
			{
				//iΪ�ڵ�s������һ���Ľڵ㣬i->valueΪ�ڵ�s�����ϵ�ֵ
				//ÿ��һ�Σ����һ���µ�״̬��������
				//��ͬʱӦ�����ӦDFA�Ĺ���
				//���µ��ж��Ƕ�ÿһ״̬��ֻ��ÿ�ֱ���һ�β���
				if (!valueTable.count(i->value) && i->value != -1)
				{
					set<int> item;
					item = move(It, i->value);	// ��������It������Ԫ����һ״̬��Ӧ�ļ��ϣ�����µ��Ӽ�
					Eclosure(item);
					if (!dfaNodeTable.count(item))
					{
						Q.push(item);		// ���µ��Ӽ�����Q�У����ı�dfa״ֵ̬
						current_state++;
						pair<set<int>, int> ap;
						ap.first = item;
						ap.second = current_state;
						dfaNodeTable.insert(ap);	// ��dfa�ڵ�stateֵ���Ӧ���Ӽ�����dfaNodeTable

					}
					//����һ�����ɵ�״̬����Ϊһ���µļ��Ϸ��뵽valueTable��
					//�˴����ܻ������ظ�����Ϊ��dfaNodeTable���Ѿ�������Ӧ��״̬����ʱ��
					//������ܻ��ǻ��в��������ࡣ
					pair<int, set<int> > tp;
					tp.first = i->value;
					tp.second = item;
					valueTable.insert(tp);
					//���湹����Ӧ��DFA������ڵ㣬���ڵ�ŵ�dfa��
					Node n0;
					n0.state = dfaNodeTable[item];
					n0.value = i->value;
					dfa[state].push_back(n0);
				}
			}
		}
	} while (!Q.empty());
	//dfa.resize(dfa.size()+1);
	//������ϣ�DFA���ɣ�ͬʱҲ������Ӧaction��dfa��̬��
	//�˴���һ�����⣬����ν���ٽ�����
}

void Eclosure(set<int> &T)
{
	// ��������T�еĽ�㣬�ж�ÿ���ڵ�ĳ����ǲ���epsilon����հ�
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

set<int> move(const set<int> &I, int value)//�Ӽ����������Ȼ�����move()����
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
	// �������Ҽ���I���Ƿ������̬
	for (set<int>::iterator p = I.begin(); p != I.end(); p++)
	{
		if (nfaIsTer[*p])
		{
			return nfaTer2Action[*p];
		}
	}
	return 0;
}

//������Ѿ��ֺõ�״̬�Ӽ����Ƿ���ĳһ���Ӽ��Ȱ���fir�ְ���sec
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
			if (p1->value == p2->value){//��fir��sec��ĳ���ڵ�ĳ���ֵһ��
				if (checkState(A, begin, p1->state, p2->state)){
					//�ж�A���Ƿ���ĳһ��״̬�Ӽ�������������ֵһ���������ڵ�
					//��������������һ�ε�ѭ���������˵ȼ۵��������뿪����ѭ������ʼ��һ��ѭ��
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
//��С��DFA
void minidfa(){
	if (dfa.size() == 1) return;
	vector<set<int>> A(dfa.size() + 1);
	int begin = 0;
	//�洢�����ս�̬״ֵ̬
	for (int i = 0; i < dfa.size(); i++){
		if (dfaIsTer[i] != 0)
			A[begin].insert(i);
	}
	begin++;
	//�洢���з��ս�̬״ֵ̬
	for (int i = 0; i < dfa.size(); i++)
	{
		if (dfaIsTer[i] == 0)
			A[begin].insert(i);
	}

	bool flag = true;//�ж��Ƿ��ѷָ���ɣ����Ϊfalse��δ���Ϊtrue
	while (flag){
		flag = false;
		for (int i = 0; i <= begin; i++){//������ǰ���е�״̬set
			for (set<int>::iterator p = A[i].begin(); p != A[i].end(); ){//������ǰset�ڵ����нڵ�
				begin++;//�·��������ڴ洢�·ָ������״̬����
				if (A[i].size() > 1 && dfa[*p].empty()){//��ǰ��set�е����������1�����������Ľڵ��޺�����
					flag = true;
					//���ýڵ�ŵ��µķ����У�����ԭ���ķ�����ɾ��
					A[begin].insert(*p);
					p = A[i].erase(p);
					continue;//����������set����һ�����
				}
				for (set<int>::iterator q = A[i].begin(); q != A[i].end(); ){//����к���ڵ����size����1
					//��ɨ�赽ͬһ�ڵ�������
					if (p == q)
					{
						q++;
						continue;
					}
					//�����ǵȼ�״̬����ɾ��ԭ���ڵ㣬�����������µķ�����
					if (!equalState(dfa[*p], dfa[*q], A, begin))
					{
						flag = true;
						A[begin].insert(*q);
						q = A[i].erase(q);
					}
					else{
						//����������ɨ��
						q++;
					}
				}
				if (A[begin].empty()) begin--;//����ǰset����ת��֮��Ϊ�գ���ɾ����ǰ����
				p++;
			}
		}
	}
	//���ڵõ����Ƿֺ�״̬��A������begin+1��״̬�Ӽ�
	//������е��Ǹ�����Щ״̬�Ӽ�������С��DFA
	//ֻ��Ҫ�����Ӽ��а�������һ��״̬�ļ���
	//����ͳ�������ļ���
	vector<int> num;
	for (int i = 0; i <= begin; i++)
	{
		if (A[i].size() != 1)
			num.push_back(i);
	}
	//������Щ�Ӽ�
	for (int i = 0; i < num.size(); i++){
		int der = 0;//��¼�������ɵĽڵ��״ֵ̬
		for (set<int>::iterator p = A[num[i]].begin(); p != A[num[i]].end(); p++){
			//����A�е���Щ����
			//����Щ�����еĵ�һ��Ԫ�ص�״ֵ̬��Ϊ�����ڵ��״ֵ̬
			if (p == A[num[i]].begin())
				der = *p;
			else{
				for (list<Node>::iterator it = dfa[*p].begin(); it != dfa[*p].end(); it++){
					//����dfa����Щ�ڵ��Ӧ������
					bool inse = false;
					for (list<Node>::iterator at = dfa[der].begin(); at != dfa[der].end(); at++){
						//������һ��Ԫ����dfa�ж�Ӧ������
						//�ж��Ƿ�����ͬ��Ԫ��it��at����û������Ҫ������ڵ�������Ԫ�ص�������
						if (at->state == it->state&&at->value == it->value){
							inse = true;
							break;
						}
					}
					if (!inse)
						dfa[der].push_back(*it);
				}
				dfa[*p].clear();//��ն���Ĳ���Ҫ�Ľڵ�
				//������Ҫ����ڵ㣬��˽��������ս�̬������
				if (dfaIsTer[*p] != 0) dfaIsTer[*p] = 0;
				//�滻�ڵ��е�״̬��
				for (int k = 0; k < dfa.size(); k++){
					//����ԭ��dfa�����нڵ�
					if (!dfa[k].empty()){
						//���������ת����ʱ��û�б����
						for (list<Node>::iterator it = dfa[k].begin(); it != dfa[k].end();){
							//����dfa�иýڵ�������е����нڵ㣬�ҵ����Ӧ�ľ�����ͬ��״ֵ̬�Ľڵ���нڵ��޸Ĳ���
							bool judge = false;
							if (it->state == *p){
								//���������Ľڵ��״ֵ̬���õ��ڸ�״̬�Ӽ�����������״ֵ̬
								for (list<Node>::iterator at = dfa[k].begin(); at != dfa[k].end(); at++){
									//�ٴα���dfa�иýڵ�����������нڵ㣬�Ӷ���dfa�Ľڵ�������ɾ���ýڵ�
									if (at->state == der&&at->value == it->value)
									{
										it = dfa[k].erase(it);
										judge = true;//��ʾ������Ҫ�޸Ľڵ�״ֵ̬�������½��
										break;
									}
								}
								if (!judge)
								{
									Node in;
									in.value = it->value;
									in.state = der;//�½���״ֵ̬�������ȷ����״̬�Ӽ�����λԪ�ص�״ֵ̬
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
//������С����DFA���ɿ��ƴ���
void genAnalysisCode(){
	//����һЩͷ������
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
		//��������dfa�еĽڵ�
		ofile << "\t\t\tcase " << i << ":" << endl;
		ofile << "\t\t\t{" << endl;
		if (dfaIsTer[i]>0){
			//�����״̬����̬�Ļ����жϵ�ǰ�Ƿ�ֱ�������̬���
			//����Ҫ�����̬�������������β
			ofile << "\t\t\t\tif(i==N-1)" << endl;
			ofile << "\t\t\t\t{" << endl;
			//��ӡ��ǰ��Ӧ������ʽ�Ľ��
			size_t length = actionTable[dfaIsTer[i]].size();
			ofile << "\t\t\t\t\t" << actionTable[dfaIsTer[i]].substr(0, length) << endl;
			ofile << "\t\t\t\t\tbreak;" << endl;
			ofile << "\t\t\t\t}" << endl;
		}
		for (list<Node>::iterator p = dfa[i].begin(); p != dfa[i].end(); p++){
			//����dfa[i]������
			ofile << "\t\t\t\tif(yytext[i]=='" << (char)p->value << "')" << endl;
			ofile << "\t\t\t\t{" << endl;
			ofile << "\t\t\t\t\tstate=" << p->state << ";" << endl;
			ofile << "\t\t\t\t\tbreak;" << endl;
			ofile << "\t\t\t\t}" << endl;
		}
		//�����жϣ���code��������Բ�������̬�У�����Ϊ�������
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