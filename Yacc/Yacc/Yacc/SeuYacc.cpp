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
//һЩ�����Ķ���
vector<Producer> producerSet;//�洢���в���ʽ���������ڶ������ʽʱ���г�ʼ����
map<int,vector<int> > hpSet;//�Բ���ʽ����Ϊ�ؼ��֣��Զ�Ӧ����ʽ�ı��Ϊ���ݵĹ�ϣ��
map<int,int> terminSet;//�洢�ս��,����Ҫ�����м����������#
map<int,int> nonterminSet;//�洢���ս��,��ͬʱ�洢һ����action��ͷ�Ķ�Ӧ��ϵ
vector<vector<int> > actionTable;//�洢���������еڶ�ά��С������֪
map<string,int> tsymTable;//��������ս���Ĵ�����Ӧ���ֱ����ӳ��
map<string,int> ntsymTable;
map<int,int> priorityTable;//���ȼ���
vector<int> producerPrioTable;//����ʽ�����ȼ�������������ȼ�������ʹ��
set<int> leftTable;//���ϱ�
set<int> rightTable;//�ҽ�ϱ�
map<int,string> symclaTable;//�洢����ֵ���͵ı�
vector<string> produceActionTable;//�洢���嶯���ı�

//��Item�Ķ��壺��ʾ��Ŀ
class Item
{
public:
	friend bool operator <(const Item &it1,const Item &it2);//<���ŵ�����
	Item(int producerNum,int currentPos);//���캯��
	int getCurrSym();//������һ����Ҫ�ƽ��ķ��ţ�����Ѿ���������򷵻�-1����ʾ�ɹ�Լ��
	int getNextSym() const;//������һ��Ҫ�ƽ��ķ��ţ��ڽ�����Ŀ����չ�����м���Ԥ���ʱʹ�á�
	int getProdN();//���ز���ʽ�ı�š�
	bool isEnd();//�Ƿ��Ҳ��Ѿ�ȫ���ƽ���
	bool nextIsEnd() const;//�Ҳ��Ƿ񵽴����һ�����ţ��ڽ�����Ŀ����չ����ʱ�á�
	void setpredictSym(const set<int> &predictSymbol);//����predictSym��ֵ
	const set<int> &getpredictSym() const;//��ȡpredictSym��ֵ
	void move();
private:
	
	int pn;//�洢����ʽ�ı�š�
	int pos;//�洢����Ŀ���ƽ�λ�á�
	set<int> predictSym;//�洢����Ŀ��Ԥ���
};
//��Item�Ĺ��캯��
Item::Item(int producerNum,int currentPos)
{
	this->pn = producerNum;
	this->pos = currentPos;
}
//���ز���ʽ�Ҳ���һ�ƽ����ţ�������-1��ɹ�Լ��-1����#��
int Item::getCurrSym()
{
	return producerSet[pn].right[pos];
}
//�������һ���ƽ��ķ���
int Item::getNextSym() const
{
	return producerSet[pn].right[pos+1];
}
//��ȡ����ʽ�ı��
int Item::getProdN()
{
	return this->pn;
}
//�жϵ�ǰ����ʽ�Ƿ��Ѿ�ȫ���ƽ�
bool Item::isEnd()
{
	//��Ҫ���������
	//һ�Ƕ�����Ԥ���Ϊepslong��ʱ��
	//���ǵ�ǰ��ͷ��λ�õ�������Ҫ������Ԥ�����λ��
	if(producerSet[pn].right[0]==EPSLONG)
		return true;
	if(this->pos==producerSet[pn].right.size())
		return true;
	return false;
}
//�ж��Ҳ��Ƿ��Ѿ��ﵽ���һ�����ţ�����Ŀ����չ��ʱ��ʹ��
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
//��ͷ������
void Item::move()
{
	pos++;
	return;
}
bool operator <(const Item &it1,const Item &it2)
{
	//�˴�<�Ķ��壬��Ϊ�˽��������Խ������set�У���Ϊset�ڲ����ʱ���Ĭ�ϵ���<�����жϴ�С
	if(it1.pn==it2.pn)
	{
		if(it1.pos==it2.pos)
			return it1.getpredictSym()<it2.getpredictSym();
		else return it1.pos<it2.pos;
	}
	else return it1.pn<it2.pn;
}

//����������
void extension(set<Item> &itemSet);
//��������first�ĺ���,����Ϊ����ʽ��ţ�first���ż����ѳ��ַ��ս���ż�
//��Ϊ�ݹ麯����Ϊ�ⲿ���ɵ���
void first(const Item &item,set<int> &predictSymbol);
//�������ⲿ�ɵ���first����
bool getfirstSet(int producerNum, set<int> &firstSet, set<int> &isUsed);
bool produce();//������Ŀ��֮���ת����ϵ
void generateTableCode();
void generateParseCode();
void generateSemanticActionCode();//�������嶯������
void generateMainCode();
void generateConstCode();
void generateHead();
void generateSVCode();//�������ս����Ĭ��ֵ �Ĵ���:void getvalue();
int readinputfile();//ǰ�����벿�ֵ���Ҫ���������ڶ����ļ�������
int specSymParse();
bool readReg();//����ε�ɨ�����
bool readAproducer();//��ȡһ������ʽ
void genhpSet();
void outSignalTable();
void displayItem(Item & item);
void outputProducer();

void main() 
{
	cout<<"SeuYacc �﷨��������:"<<endl;

	string inputname;
	cout<<"������SeuYaccԴ����������.yΪ��׺��ʾ��:";
	cin>>inputname;
	fin.open(inputname.c_str());

	outputCppfile.open("yyparse.cpp");
	outputSymbolTablefile.open("Symbol_Table.txt");//���ɷ��ű��ļ�
	outputDotHfile.open("yytab.h");//����ȫ�ַ�������ֵ����������
	outputProducerfile.open("producer.txt");
	if (outputCppfile.fail() || fin.fail() || outputSymbolTablefile.fail() || outputDotHfile.fail() || outputProducerfile.fail())
	{
		cout<<"�޷������������ļ��������ļ���!"<<endl;
		return;
	}
	//��ʼ��
	Producer ap;
	producerSet.push_back(ap);//���Ȱ���չ�ķ���ʼ����ѹջ
	readinputfile();
	generateMainCode();
	outSignalTable();
	outputProducer();
	outputCppfile.close();
	outputSymbolTablefile.close();
	outputDotHfile.close();
	outputProducerfile.close();
	cout<<"�ɹ����ɣ�������C������������﷨������...\n";

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


//�ú���ʵ�ֶ��ڵ�ǰ��Ŀ������Ŀ����չ
void extension(set<Item> &itemSet)
{
	queue<Item> Q;
	//����Ҫ�ѵ�ǰ��Ŀ��������״̬���뵽�����С�(��ȥ��������������)
	for(set<Item>::iterator ps=itemSet.begin();ps!=itemSet.end();ps++)
	{
		const Item &item1 = const_cast<Item&>(*ps);
		Item &item2 = const_cast<Item&>(item1);
		if(!item2.isEnd()&&nonterminSet.count(item2.getCurrSym()))
			Q.push(item2);
	}
	//����ѭ���жϡ�
	set<int> isU;
	set<int> isE;
	while(!Q.empty())
	{
		Item citem=Q.front();
		Q.pop();
		//��ȡ��ǰ���еײ�����Ŀ�ĵ�ǰ�ƽ����ţ�������Ϊ����ʽ���󲿷��ţ���hpSet�в�ѯ�����Ӧ�Ĳ���ʽ�ı��
		vector<int> vpi=hpSet[citem.getCurrSym()];
		//��ǰ���еײ�����Ŀ��Ԥ����ļ���
		set<int> predictSym;
		first(citem,predictSym);	
		for(int i=0;i<vpi.size();i++)
		{
			//��������Ŀ�������еĲ���ʽ�Ҳ�
			Item tp(vpi[i],0);//�Ե�ǰ����ʽΪ��������һ����Ŀ
			tp.setpredictSym(predictSym);//���ø���Ŀ��Ԥ���
			if(!itemSet.count(tp))
			{
				//�������Ŀ������Ŀ���У����轫�����뵽��Ŀ����ȥ
				itemSet.insert(tp); 
				//����ʱ����Ŀ����״̬��Ȼ��Ϊ��̬����������Q�м�����չ
				if(nonterminSet.count(tp.getCurrSym()))
				{
					Q.push(tp);
				}
			}
		}
	}
}
//Ԥ����ļ���
void first(const Item &item,set<int> &predictSymbol)
{
	//����ǰ��Ŀ�ٴ����ƽ�֮��ͻ������̬
	if(item.nextIsEnd())
	{
		predictSymbol=item.getpredictSym();
	}
	else 
	{
		//�����һ���ƽ��ķ���
		int sym=item.getNextSym();
		//���÷�������̬�����У���ֱ����Ԥ��������в���÷���
		if(terminSet.count(sym))
			predictSymbol.insert(sym);
		else
		{
			//������Ҫ����Ԥ�������
			//�����ҵ����ܶ�Ӧ�Ĳ���ʽ�Լ����Ӧ���Ҳ�����ʽ����
			vector<int> tv=hpSet[sym];
			set<int> isused;
			for(int i=0;i<tv.size();i++)
			{
				getfirstSet(tv[i],predictSymbol,isused);
			}
		}
	}
}
//���ڷ���̬����Ŀ��Ԥ����ļ���
bool getfirstSet(int producerNum, set<int> &firstSet, set<int> &isUsed)
{
	//�����ص���Ŀ��Ӧ�Ĳ���ʽ�Ҳ�ΪEPSLONG������Ԥ��������в���
	if ( producerSet.at(producerNum).right[0] == EPSLONG )
		return 0;
	else{
		int i = 0;
		//����ǰ��Ŀ��Ӧ�Ĳ���ʽ���Ҳ��ĵ�һ�����ż�Ϊ�ս������ֱ�ӽ�����뵽�����
		if (terminSet.count(producerSet.at(producerNum).right[i]))
			firstSet.insert(producerSet.at(producerNum).right[i]);
		else if (nonterminSet.count(producerSet.at(producerNum).right[i]))
		{
			//�жϵ�ǰ��ͷ���ڷ����Ƿ��Ѿ����������������������Ѿ���������˾Ͳ����ٽ��д���
			//��û�б�����
			if (!isUsed.count(producerSet.at(producerNum).right[i]))
			{
				//���ò���ʽ���������б�
				isUsed.insert(producerSet.at(producerNum).right[i]);
				vector <int> tv;
				//��ȡ�ò���ʽ�Ҳ��п��Լ��������Ĳ���ʽ����Ӧ�ı��
				tv = hpSet[producerSet.at(producerNum).right[i]];
				int c = 1;
				//�����������еĲ���ʽ
				for (vector < int >::const_iterator p1 = tv.begin(); p1 != tv.end(); p1++)
				{
					//�жϸò���ʽ�Ƿ���epsilon����ʽ
					if (!getfirstSet(*p1, firstSet, isUsed))
					{
						c = 0;
					}
				}
				//c = 0����Ҫ��epsilon����ʽ���д���
				while (c == 0)
				{
					i++;
					//���ò���ʽ�������������µĲ���ʽ�Ҳ�δ������
					if (producerSet.at(producerNum).right[i] != 0 &&
						nonterminSet.count(producerSet.at(producerNum).right[i]) &&
						isUsed.count(producerSet.at(producerNum).right[i]) == 0)
					{
						vector < int > tv2;
						//��ȡ�ò���ʽ�Ҳ��п��Լ��������Ĳ���ʽ����Ӧ�ı��
						tv2 = hpSet[producerSet.at(producerNum).right[i]];
						//�жϸò���ʽ�Ƿ���epsilon����ʽ
						for (vector < int >::const_iterator p2 = tv2.begin(); p2 != tv2.end(); p2++)
							if (!getfirstSet(*p2, firstSet, isUsed))
								c = 0;
					}
					else if (terminSet.count(producerSet.at(producerNum).right[i]))
						//����ǰ��Ŀ��Ӧ�Ĳ���ʽ���Ҳ��ĵ�һ�����ż�Ϊ�ս������ֱ�ӽ�����뵽Ԥ���������
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
	//��ӡ���е���Ŀ
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
	//��ӡ������Ŀ
	//��ӡ��Ŀ��Ӧ�Ĳ���ʽ��
	cout<<"PID:"<<item.getProdN()<<"      ";
	//��ӡ��Ŀ��Ӧ�Ĳ���ʽ��
	cout<<"LEFT:"<<producerSet[item.getProdN()].left<<"      ";
	//��ӡ��Ŀ��Ӧ�Ĳ���ʽ�Ҳ�
	cout<<"RIGHT:";
	for(int i=0;i<producerSet[item.getProdN()].right.size();i++)
	{
		cout<<producerSet[item.getProdN()].right[i]<<"  ";
	}
	//��ӡԤ���
	cout<<"\npredict:";
	for(set<int>::const_iterator i=item.getpredictSym().begin();i!=item.getpredictSym().end();i++)
	{
		cout<<*i<<"  ";
	}
	cout<<"\n-----------------------------------\n";
}
void genhpSet()
{
	//�ڱ�������.y�ļ���ʱ��ÿ��һ������ʽ���͵��øú�������hpSet��
	//����ÿ������ʽ�Ҳ����Լ������ɵĲ���ʽ�ļ���
	//0�Ų���ʽ���ô���
	for (int i = 1; i < producerSet.size(); i++)
	{
		//�жϸò���ʽ�Ƿ�����hpSet������
		//���Ѱ�������Ҫ�½�
		//�������������left�½�hpSet��һ��ֵ������vector<int>��ʼ����Ϊ��
		if (!hpSet.count(producerSet[i].left))
		{
			pair<int, vector<int> > tp;
			tp.first = producerSet[i].left;
			vector<int> vt;
			tp.second = vt;
			hpSet.insert(tp);
		}
		//���ò���ʽ��Ӧ�ı�ż����left��Ӧ��vector<int>��
		hpSet[producerSet[i].left].push_back(i);
	}
}


bool produce()
{
	/**
	 *  �˴�����ͼ�ı�ʾ����Ϊ��ά���飬��Ϊ�����ʱҲ���Զ�ά�������ʽ��
	 *  �ڴ������У���������ʾҪ��ת��״̬�ţ��ø�����ʾĳһ����Լ�Ĳ���ʽ�š�
	 *  DONE!
	 */
	//������һ��Ҫ�õ��ĳ�������
	int curState=0;//�洢��Ŀ����״̬�š�����Ŀ���Ĳ�����̬����
	int widgeN=nonterminSet.size()+terminSet.size();//��ʾ���п��ܵ���Ŀ���ı�
	queue<set<Item> > Q;
	map<set<Item>,int> itemsetTable;//����Ŀ����ĳһ���ֱ�ʶ��ӳ��
	map<int, set<Item> > moveItemset;//�Ա�Ϊ�ؼ��֣��Խ��з������Ŀ����Ŀ��Ϊ���ݡ�
	set<Item> I0;

	Item firstitem(0,0);//�����ʼ��Ŀ���е���Ŀ��
	set<int> predicts;
	predicts.insert(NONTERMINS-1);//��ʾ����״̬
	firstitem.setpredictSym(predicts);

	I0.insert(firstitem);

	firstitem.move();//�޸����ƽ�λ����Ϊ�ں��������ȷ״̬ʱҪ��

	extension(I0);//�ƽ�1λ�󣬽�����չ
	Q.push(I0);//��ɺ�IO�������Q

	display(I0);
	pair<set<Item>,int> te;//����Ŀ��0���뵽��Ŀ���ı���
	te.first=I0;//first����IO,������չ���
	te.second=curState;//second������Ŀ����״̬��
	itemsetTable.insert(te);//��te����itemsetTable��te��������Ŀ���ţ�����second��������ʽ��ţ���ͷλ�ã�Ҳ�������ϵ�.����������pos��ʾ����Ԥ�������������Ϊһ��item���������set
	//����ο�������ʦ������115ҳ������Ŀ����ͷ�������㷨��������������Щ�Ķ�
	//��Ŀ���幹�죨û��ר������������ڷ�����Ĺ���������ʹ�õ��ˣ�
	//��Ҫ�㷨��
	//
	//���ڳ�̬����Ŀ�������ǽ�����չ��������ص��ƽ���Լ������У�ͬʱ���������µ���Ŀ�����ͽ���������Q�ȴ�����
	while(!Q.empty())
	{
		//**********************1.��itemset��ѡ����Ҫ�������Ŀ����ţ�����Ϊ��������к�
		set<Item> itemset=Q.front();
		Q.pop();//�Ӷ�����ȡ��һ����Ŀ����
		moveItemset.clear();//��գ��Խ�����һ��ѭ��

		vector<int> f(widgeN);//����ڶ�ά�Ĵ�С����Ԥ�ȶ����
		for(int i=0;i<f.size();i++)
		{
			f[i]=ERROR;//��ERRORֵ��vector���г�ʼ����
		}

		actionTable.push_back(f);

		int column,row;
		//��map�в���itemset��ʾ����Ŀ����Ӧ�ı�ţ������к�
		row=itemsetTable[itemset];
		cout<<"state = "<<row<<endl;
		//**********************2.�����Ŀ�����еĸ����ƽ��󣬲������µ���Ŀ���壬�����л���ֿɹ�Լ����Ŀ���������
		for(set<Item>::iterator ps=itemset.begin();ps!=itemset.end();ps++)
		{
			const Item &item1 = const_cast<Item&>(*ps);
			Item &item2 = const_cast<Item&>(item1);
			if(item2.isEnd())//���ֿɹ�Լ��Ŀ������д���
			{
				for(set<int>::const_iterator pi=ps->getpredictSym().begin();pi!=ps->getpredictSym().end();pi++)
				{
					//�Կɹ�Լ��Ŀ��������ÿ��Ԥ������������
					column=terminSet[*pi];//��ȡ�к�
					//action����ԭ���Ѿ�Ҫ��Լ��λ�ã��������ֱ��ж��ɹ�Լ������
					if(actionTable[row][column]<=0&&item2.getProdN()!=actionTable[row][column]*(-1))
					{
						cout<<"Reduction & Reduction Confliction"<<endl;
						cout<<"row="<<row<<" column="<<column<<" old="<<actionTable[row][column]
					       <<" new="<<item2.getProdN()<<endl;
						return false;
					}
					actionTable[row][column]=item2.getProdN()*(-1);//��ɸ����Ա�ʾ��Ϊ��Լ��,���Ϊ0���ڱ�ʾaccept
				}
			}
			else//�޿ɹ�Լ��Ŀ
			{
				Item tp=*ps;
				tp.move();//��ͷ����һλ
				moveItemset[item2.getCurrSym()].insert(tp);
			}
		}
		//**********************3.�������²�������Ŀ��������չ���ж��Ƿ���Ҫ���뵽������ȥ�����Ҽ������������
		for(map<int,set<Item> >::iterator ph=moveItemset.begin();ph!=moveItemset.end();ph++)
		{
			extension(ph->second);//�Ե�ǰ��moveItemset�е���Ŀ����set<Item>������չ

			if(!itemsetTable.count(ph->second))//���map itemsetTable��û�и���Ŀ���壬��Ϊ�������µ�״̬�ţ�Ȼ�����ȴ��������
			{
				pair<set<Item>,int> tp;
				tp.first=ph->second;//������չ���ph->second�е���Ŀ��			
				curState++;//ֻ�ڴ˴�������Ŀ������ֻ�ڴ˴��޸���ֵ
				tp.second=curState;

				itemsetTable.insert(tp);//����������Ŀ������itemsetTable 
				//�����ǰ����Ŀ��δ���ֹ��Ļ�����Ҫ�����������еȴ�����
				Q.push(ph->second);
			}	

			//���濪ʼ���ɶ�Ӧ��action��
			//������ñ�����Ӧ�ı��е���ţ������ս�����ս��map���ĸ���
			column=(nonterminSet.count(ph->first)?nonterminSet[ph->first]:terminSet[ph->first]);

			//���ڱ��е�λ�ã��ж������ֵ�Ƿ�Ϊ�����˴���һЩ�������ж��Ƿ��й�Լ���ƽ���ͻ
			if(actionTable[row][column]<=0)
			{
				//����ǰ�����ȼ�����ph�е����ȼ�������ph�е�����
				if(producerPrioTable[actionTable[row][column]*(-1)]<priorityTable[ph->first])
				{
					actionTable[row][column]=itemsetTable[ph->second];
					continue;
				}
				//�������ҽ�ϣ���Ȼ���ǵ�minic����û���ҽ�ϣ��������ﻹ��Ԥ����һ�£�������ͬ��
				if(rightTable.count(ph->first))
				{
					actionTable[row][column]=itemsetTable[ph->second];
					continue;
				}//���������ϣ������
				if(leftTable.count(ph->first))
				{
					continue;
				}
	
				//������������������㣬�򱨴�
				cout<<"Reduction & Shift Confliction"<<endl;
				cout<<"row="<<row<<" column="<<column<<" old="<<actionTable[row][column]
					<<" new="<<itemsetTable[ph->second]
					<<" signal="<<ph->first<<endl;
				return false;
			}
			else
				//actionTable[row][column]>0�����
				actionTable[row][column]=itemsetTable[ph->second];
			
		}
	}
	return true;
	//�����������Ŀ����������Ĺ���
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
	//�˲�����ɲ���������
	//������ɶ�Token��������
	outputCppfile<<"int readToken()\n";
	outputCppfile<<"{\n";
	outputCppfile<<"\tif(fin.eof())\n";
	outputCppfile<<"\t\treturn SOURCE_END;\n";
	outputCppfile<<"\tchar buf[256];//��֧�ֳ���256���ַ��ķ���\n";
	outputCppfile<<"\tint pos=0;\n";
	outputCppfile<<"\tbool isEnd=false;\n";
	outputCppfile<<"\tbool isToken=false;//��isOperator����\n";
	outputCppfile<<"\tbool isOperator=false;\n";
	outputCppfile<<"\tstring ops(\"!@#$%^&*()+-=|\[]{};':\\\",.<>/?\");\n";
	outputCppfile<<"\tstring wss(\"\\t\\n\\r \");\n";
	outputCppfile<<"\tstring letter(\"_0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ\");\n";

	outputCppfile<<"\tchar c=fin.get();\n";
	outputCppfile<<"\twhile(c==' '||c=='\\t'||c=='\\n'||c=='\\r') c=fin.get();//�˵��հ׷�\n";
	outputCppfile<<"\twhile(!isEnd)\n";
	outputCppfile<<"\t{\n";
	outputCppfile<<"\t\tif(c==-1)\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tbuf[pos]='\\0';\n";
	outputCppfile<<"\t\t\tisEnd=true;\n";
	outputCppfile<<"\t\t\tcontinue;\n";
	outputCppfile<<"\t\t}";
	outputCppfile<<"\t\tif(ops.find(c)>=0&&ops.find(c)<ops.size())//��ʾ��c���������ַ�ʱ��\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\t//����������ַ��Ĵ������\n";
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
	outputCppfile<<"\tstack<Sym> symStack;//����ջ\n";
	outputCppfile<<"\tstack<SV> valStack;//����ֵջ\n";
	outputCppfile<<"\tSym st;//��������ʱ����ʱջ��Ԫ�ش洢����\n";
	outputCppfile<<"\tst.symbol=0;\n";
	outputCppfile<<"\tst.state=0;\n";
	outputCppfile<<"\tSV val;\n";
	outputCppfile<<"\tsymStack.push(st);//����ֵջ����Ҫ�ͷ���ջͬ��\n";
	outputCppfile<<"\tvalStack.push(val);\n";
	outputCppfile<<"\tinputsymbol=readToken();\n";
	outputCppfile<<"\twhile(1)\n";
	outputCppfile<<"\t{\n";
	outputCppfile<<"\t\tst=symStack.top();\n";
	outputCppfile<<"\t\tint col=symbolTable[inputsymbol];\n";
	outputCppfile<<"\t\tint result=predictTable(st.state,col);\n";
	outputCppfile<<"\t\tif(result==E)//����\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tcout<<\"������yacc�﷨�淶���������!\"<<endl;\n";
	outputCppfile<<"\t\t\treturn 0;\n";
	outputCppfile<<"\t\t}\n";
	outputCppfile<<"\t\tif(result==ACCEPT)\n";
	outputCppfile<<"\t\t{\n";
	outputCppfile<<"\t\t\tcout<<\"����ɹ�!\"<<endl;\n";
	outputCppfile<<"\t\t\treturn 1;\n";
	outputCppfile<<"\t\t}\n";
	outputCppfile<<"\t\tif(result<0)//������ʾΪ��Լ��Ŀ\n";
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
	outputCppfile<<"\t\t\tint n=producerN[result];//ȡ�øúŲ���ʽ�Ҳ�����������������ջ��\n";
	outputCppfile<<"\t\t\tfor(int i=0;i<n;i++)\n";
	outputCppfile<<"\t\t\t{\n";
	outputCppfile<<"\t\t\t\tsymStack.pop();\n";
	outputCppfile<<"\t\t\t\tu[n-i]=valStack.top();\n";
	outputCppfile<<"\t\t\t\tvalStack.pop();\n";
	outputCppfile<<"\t\t\t}\n";
	outputCppfile<<"\t\t\trunaction(result);//ִ�����嶯��\n";
	outputCppfile<<"\t\t\t//�ٽ�����ʽ�󲿵ķ���ѹջ������ֵһͬѹջ\n";
	outputCppfile<<"\t\t\tst.symbol=pLeftSection[result];\n";
	outputCppfile<<"\t\t\tst.state=predictTable(symStack.top().state,symbolTable[st.symbol]);\n";
	outputCppfile<<"\t\t\tsymStack.push(st);\n";
	outputCppfile<<"\t\t\t//������ʽ�󲿷��ŵ�����ֵ��ջ�����ֵ��runaction()���Ѿ��޸�\n";
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
	//�������SymbolTable�ĳ�ʼ���δ���.
	outputCppfile<<"\tstring filename;\n";
	outputCppfile<<"\tcout<<\"�������д�Ĳ����ļ���:\"<<endl;\n";
	outputCppfile<<"\tcin>>filename;\n";
	outputCppfile<<"\tfin.open(filename.c_str());\n";
	outputCppfile<<"\tif(fin.fail())\n";
	outputCppfile<<"\t{\n";
	outputCppfile<<"\t\tcout<<\"�޷����ļ� \"<<filename<<endl;\n";
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
	if(c!='%') return TERMINATE;//Ҫ�������ļ���һ���ַ�����Ϊ%
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
					state=SEGMENT_REG;//��������
					continue;//��������Ĳ���%����䣬ֱ�ӻص�switch
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
				//����%{������ȫ������ļ�
				while(1)
				{
					c=fin.get();
					if(c=='%')
					{
						state=specSymParse();
						if(state==DEF_SEG_END) break;//�������ײ�%}�����˳�ѭ����
						else if(state!=NOMATCH)
						{
							return TERMINATE;//����Ȳ�����DEF_SEG_ENDҲ������NOMATCH���򱨴�
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
				
				pos++;//������һ���ַ�
				lpos=pos;
				while(line[pos]!='>'&&pos<line.size()) pos++;
				if(pos==line.size())
				{
					cout<<"%type or token formation error!"<<endl;
					return 0;
				}
				string valstr=line.substr(lpos,pos-lpos);//ȡ��ֵ����

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
				//�˲��ִ�����֮������˱�symclaTable�Ͳ���ntsymTable
			}
		case UNION://output 2
			{
				c=fin.get();
				while(c!='{'&&c!=-1) c=fin.get();
				if(c==-1) return TERMINATE;	
				outputDotHfile<<"#ifndef YYVAL_H\n";//�����ͷ�ļ�yyval_h.h
				outputDotHfile<<"#define YYVAL_H\n";
				outputCppfile<<"union SV\n";
				outputDotHfile<<"union SV\n";
				outputCppfile<<"{\n";
				outputDotHfile<<"{\n";
				while((c=fin.get())!='}') {outputCppfile.put(c);outputDotHfile.put(c);}
				outputCppfile<<"};\n";//�˶�û�����κα��棬����˵������ִ���Ҳ����У��
				outputDotHfile<<"};\n";
				outputCppfile<<"SV u[10];\n";//�ڴ�������ĿΪ10���������飬�Ա�������嶯��ʱ���á�
				outputCppfile<<"SV yyval;\n";//�ڴ�����ȫ�ֱ���yyval�����ڷ���lex���ŵ�����ֵ
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
					//�洢��������
					pair<string,int> tp;
					tp.first=t;
					tp.second=TERMINS+tsymTable.size();
					tsymTable.insert(tp);//�Ѳ�����Ҳ�ŵ��ս�����С�
				    
					pair<int,int> tpp;
					tpp.first=tp.second;
					tpp.second=terminSet.size()+nonterminSet.size();
					terminSet.insert(tpp);
					if(state==LEFT)//�������ҽ�ϱ�
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
					priorityTable.insert(ptp);//�Ѳ������������ȼ����С�
				}
				break;
			}
		case SEGMENT_REG:
			{
				//�˴�Ϊ����ε�ɨ��ʹ��������
				//��һ��������ʽ�ļ��ϣ���ɸ��ֱ�����ݡ�			
				//����producerPrioTable		
				producerPrioTable.push_back(0);//��֤����ֵ���ʲ������
				if(!readReg())
				{
					cout<<"read reg segment error!\n";
					return 0;
				}
				//����ProducerSet;
				producerSet[0].left=NONTERMINS-1;//��ʾ����չ�ķ���ʼ����
				producerSet[0].right.push_back(producerSet[1].left);
				//����tsymTable
				pair<string,int> tp1;
				string s("#");
				tp1.first=s;
				tp1.second=NONTERMINS-1;
				tsymTable.insert(tp1);
				//����terminSet
				pair<int,int> tp2;
				tp2.first=tp1.second;
				tp2.second=terminSet.size()+nonterminSet.size();
				terminSet.insert(tp2);
				//�ڶ�����ɶ���������ɡ�
				genhpSet();//�������hpSet������
				if(!produce())
				{
					cout<<"produce action table error!"<<endl;
					return 0;
				}

				//����ļ������һЩ����
				generateConstCode();
				//������������������Token�ĺ���
				generateTableCode();
				//���Ĳ����ִ�����嶯���ĺ���
				generateSemanticActionCode();
				//���岽�����������,���Ȳ���void getvalue()���������ս����Ĭ�ϳ�ֵ
				generateSVCode();
				generateParseCode();
				//�����������������Ӧ��������������
				//generateMainCode();
				break;
			}
		case SELF_DEF://����û��Զ���������
			{
				c=fin.get();
				while(c!=-1)
				{
					outputCppfile.put(c);////*******output to file
					c=fin.get();
					
				}
				outputCppfile.put('\n');
				continue;//����while���жϽ�����
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
	case '%'://����ηָ���
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
				// ��token|union|left|right����buf��
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
			else return TERMINATE;//���󣬳�����ֹ
		}

	default:
		fin.unget();
		return NOMATCH;
	}
}

bool readReg()
{
	//����ε�ɨ�����
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
				fin.unget();//���������ַ�����Ϊ��readinputfile()�����ж�%%���д���ĳ���
				break;
			}
			if(state==TERMINATE) 
			{
				cout<<"Regulation Segment ERROR!"<<endl;
				return false;
			}	
			if(state==NOMATCH) fin.unget();//��%Ҳ�˻أ��������������
			else
			{
				cout<<"Regulation Segment ERROR2"<<endl;
				return false;//����ֱ�ӷ���.
			}
		}


		while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();//�׵��ո�
		fin.unget();//�˻أ���������readAproducer()����
		if(!readAproducer())
		{
			cout<<"producer error!see above!"<<endl;
			return false;
		}
		c=fin.get();
		while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	}

}
bool readAproducer()//δ�׵�ǰ��Ŀո�
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
	buf[0]='\0';//���buf��
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
		nonterminSet.insert(tpp);//�����ս�����ڼ������ݡ��洢���Ƿ��ս����Ӧ��action��ı�ͷ
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
		ap.left=ntsymTable[temp];//�½�һ������ʽ���ȱ����󲿵�ֵ
		int priority=0;//�洢����ʽ�����ȼ�����ʱ��ȡ�ķ����ǣ�����ʽ�����ȼ�ͬ�Ҳ��������ҵĲ���
	               //�����ȼ���ͬ
		while(1)
		{
		//�����׵��ո�
			c=fin.get();
			while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
			if(c==';'||c=='{'||c=='|') break;//����ʽ������ǻ����Ƕ�����ʼ���
			//������ʽ���Ҳ�
			if(c=='\'')//�������'�������������Ķ������
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

					tsymTable.insert(tp);//�Ѳ�����Ҳ�ŵ��ս�����С�
				
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
			if(c=='\\')//��\e��ʾ��
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
		char action[1000];//�洢���嶯��
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
					if(c=='$')//��ʾ�ǲ���ʽ�󲿵�����ֵ����
					{
						//���봮"u[0]"
						for( i=0;i<4;i++)
							action[pi+i]=change[i];
						pi+=4;
						action[pi]='.';//��������ֵ�ľ�������
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
						while(c>='0'&&c<='9')//֧�ָ�����Ҳ����ս��
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
						//action[pi+2]=c;//ֻ���޸Ĵ˴���ֵ���ɡ�
						//pi+=4;
						action[pi++]='.';//��������ֵ�ľ�������
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
			//�������嶯��
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
	// �ɶ�Ӧ���ս����Ż���ս��
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
	// �ɶ�Ӧ�ķ��ս����Ż�÷��ս��
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
	// ������ʽ������ļ���

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