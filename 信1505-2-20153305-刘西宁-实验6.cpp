//2017年12月8日13:29:24
//刘西宁
//编译原理 实验6

/*
	扩充文法开始符号为^    注意输入的文法中不能包含^
	空用~来表示
	项目格式
	项目集编号_点的位置_产生式_是否遍历过 例 0_1_S->E_0
	分析表error用-表示
*/

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <sstream>

using namespace std;

//全局变量声明
string programOrigin[20];//初始输入的文法
string program[30];//转化后的文法
char VT[20];//终结符数组
char VN[20];//非终结符数组
string first[30];//first集
string follow[30];//follow集
string effectiveProject[100];//有效项目
string analyseTable[100][30];
string analyseStr;//分析串

int statusS[100];//状态栈
char chS[100];//符号栈

int statusSI;//状态栈实际长度
int chSI;//符号栈实际长度

char beginVN;//文法开始非终结符
char tuoGuangVN;
int analyseStep;
int analyseStrI;//当前分析字符在分析串中的下标
int effProN;//有效项目实际个数
int proSetN;//有效项目集个数
int no;//初始的文法规则数
int n;//转化后的文法规则数
int splitI;//分割字符串的下标
int vtN;//终结符个数
int vnN;//非终结符个数
string splitStr;//用于分割的字符串
string splitSubStr;//用于分割的字符串的分隔符


//函数声明
void transform();//转化文法
string nextSubStr();//获取下一个分隔字符串子串

int ch2num(char c);//字符转数字
void createVTVN();//查找所有的vt/vn保存到数组中

void createFirst();//构建first集
void createFollow();//构建Follow集

int vtIndex(char c);
int vnIndex(char c);
int isVT(char c);

string num2str(int num);
int str2num(string str);

int programIndex(string pro);

void createEffectiveSet();//构建有效项目集
int setNofEffPro(string str);

void analyse();
void showAnalyseData(string, string);


void addEffPro(int setN, int dotIndex, string pro);
void showProgram();
void showFirst();//显示各非终结符的first集

void showFollow();//显示各非终结符的follow集

void showAnalyseTable();


void main() {
	cout << "请输入文法规则数：";
	cin >> no;
	cout << "请输入文法规则(第一个为拓展后的产生式):" << endl;
	for (int i = 0; i < no; i++) {
		cin >> programOrigin[i];
	}

	/*no = 4;
	programOrigin[0] = "S->E";
	programOrigin[1] = "E->E+T|T";
	programOrigin[2] = "T->T*F|F";
	programOrigin[3] = "F->(E)|i";*/
	transform();

	/*n = 7;
	program[0] = "^->E";
	program[1] = "E->E+T";
	program[2] = "E->T";
	program[3] = "T->T*F";
	program[4] = "T->F";
	program[5] = "F->(E)";
	program[6] = "F->i";*/
	//showProgram();
	createVTVN();
	createFirst();
	//showFirst();

	createFollow();
	//showFollow();

	createEffectiveSet();
	//showAnalyseTable();
	cout << "请输入要分析的字符串（#结束）:";
	cin >> analyseStr;
	//analyseStr = "i+i*i#";
	analyse();
	

}









//转化文法
void transform() {
	n = 0;
	//string begin = "^->";
	//begin.push_back(programOrigin[0][0]);
	//program[n++] = begin;
	for (int i = 0; i < no; i++) {
		//判断文法规则中是否包含 | 符号
		if (programOrigin[i].find("|") != string::npos) {
			//包含
			//则将该文法进行拆分
			string tb = programOrigin[i].substr(0, 3);
			string te = programOrigin[i].substr(3);
			splitI = 0;
			splitStr = te;
			splitSubStr = "|";
			string spt;
			while ((spt = nextSubStr()) != "") {
				program[n++] = tb + spt;
			}

		}
		else {
			//不包含或|
			program[n++] = programOrigin[i];
		}
	}

	beginVN = program[0][3];
	tuoGuangVN = programOrigin[0][0];
}

string nextSubStr() {
	string t = "";
	int beginI = splitI;
	int f = 0;

	while (splitI < splitStr.length()) {
		int p = 1;
		for (int i = 0; i < splitSubStr.length(); i++) {
			if (splitSubStr[i] != splitStr[splitI + i]) {
				p = 0;
				break;
			}
		}
		splitI++;
		if (p == 1) {
			//匹配
			f = 1;
			splitI += splitSubStr.length() - 1;
			break;
		}
	}
	t = splitStr.substr(beginI, splitI - f);
	return t;
}

//构建first集
void createFirst()
{
	queue<char> q = queue<char>();
	//遍历非终结符，将所有的非终结符入队列,并初始化first集
	for (int i = vnN - 1; i >= 0; i--)
	{
		q.push(VN[i]);
		first[i] = "";
	}
	while (!q.empty())
	{
		//取出队列的对头元素
		char vn = q.front();
		q.pop();
		int flag = 0;
		string firstT = "";
		//遍历产生式
		for (int i = 0; i < n; i++)
		{
			//该产生式的左部非终结符为vn
			if (program[i][0] == vn) {
				int rightIndex = 3;
				while (rightIndex < program[i].length() && flag == 0)
				{
					char ch = program[i][rightIndex++];
					int vtI = vtIndex(ch);
					//符号为终结符
					if (vtI != -1 || ch == '~') {
						if (!(firstT.find(ch) < firstT.length())) {
							firstT.push_back(ch);
						}
						break;
					}
					else {
						//该产生式的右部开始符号为非终结符
						int vnI = vnIndex(ch);
						//如果该给非终结符的first已求出，则加到vn的first集
						if (first[vnI] != "") {
							firstT.append(first[vnI]);
							for (int k = 0; k < first[vnI].length(); k++)
							{
								if (first[vnI][k] != '~' && firstT.find(first[vnI][k]) >= firstT.length()) {
									firstT.push_back(first[vnI][k]);
								}
							}
							if (first[vnI].find('~') >= first[vnI].length())
							{
								break;
							}
							else {
								if (rightIndex == program[i].length()) {
									if (firstT.find('~') >= firstT.length()) {
										firstT.push_back('~');
									}
								}
							}
						}
						else if (program[i][3] == vn) {
							break;
						}
						else {
							flag = 1;
							break;
						}
					}
				}
			}
		}
		if (flag == 0) {
			first[vnIndex(vn)] = firstT;
		}
		else {
			q.push(vn);
		}

	}
}

//构建follow集，前提是必须先构建好first集
void createFollow()
{
	//文法开始符号的follow集为结束符#
	follow[0] = "#";
	queue<char> q = queue<char>();
	//遍历非终结符，将所有的非终结符(文法开始符号除外)入队列,并初始化follow集
	for (int i = 1; i < vnN; i++)
	{
		q.push(VN[i]);
		follow[i] = "";
	}
	while (!q.empty())
	{
		//取出队列的对头元素
		char vn = q.front();
		q.pop();
		int flag = 0;
		string followT = "";
		//遍历产生式
		for (int i = 0; i < n; i++)
		{
			string programT = program[i].substr(3);//取该产生式的右部表达式
			//查找该非终结符在该产生式右部的下标
			int vnIndexOfProgramRight = programT.find(vn);
			if (vnIndexOfProgramRight < programT.length()) {//在该产生式右部存在该非终结符
				if (vnIndexOfProgramRight != programT.length() - 1)
				{
					char thisVNLastCh = programT[vnIndexOfProgramRight + 1];
					if (vtIndex(thisVNLastCh) != -1) {//后面的一个字符是终结符
						//将该终结符加入到该非终结符的follow集中
						if (followT.find(thisVNLastCh) >= followT.length())
						{
							followT.push_back(thisVNLastCh);
						}
					}
					else {//后面的一个字符是非终结符
						//将该终结符的first集的非空元素加入到该非终结符的follow集中
						string firstT = first[vnIndex(thisVNLastCh)];
						for (int k = 0; k < firstT.length(); k++)
						{
							if (firstT[k] != '~' && followT.find(firstT[k]) >= followT.length())
							{
								followT.push_back(firstT[k]);
							}
						}
					}
				}
				//取出该产生式右部中该非终结符后面的字符串
				string lastStr = vnIndexOfProgramRight == programT.length() - 1 ? "" : programT.substr(vnIndexOfProgramRight + 1);
				int hasNotNull = 0;//后面的表达式时候有不产生空的，0没有，1有
				for (int k = 0; k < lastStr.length(); k++)
				{
					int lastStrVNI = vnIndex(lastStr[k]);
					//判断该非终结符的first是否含有空
					if (lastStrVNI == -1 || first[lastStrVNI].find('~') >= first[lastStrVNI].length())
					{
						hasNotNull = 1;
						break;
					}
				}
				if (hasNotNull == 0) {//后面的表达式可以产生空
					//该产生式的左部非终结符的follow集属于该非终结符的follow集
					string followVTT = follow[vnIndex(program[i][0])];
					if (followVTT == "") {
						flag = 1;
						break;
					}
					for (int k = 0; k < followVTT.length(); k++)
					{
						if (followT.find(followVTT[k]) >= followT.length())
						{
							followT.push_back(followVTT[k]);
						}
					}
				}


			}
		}
		if (flag == 0) {
			follow[vnIndex(vn)] = followT;
		}
		else {
			q.push(vn);
		}

	}
}


void analyse() {
	cout << "步骤\t状态栈\t符号栈\t输入串\tACTION\tGOTO" << endl;
	analyseStep = 0;
	analyseStrI = 0;
	statusSI = 0;
	chSI = 0;
	statusS[statusSI++] = 0;//开始状态入栈
	chS[chSI++] = '#';//结束符号入栈

	while (!(chSI == 0 || statusSI == 0 || analyseStrI == analyseStr.length()))
	{
		int currStatus = statusS[statusSI - 1];//当前状态
		char currAnalyseCh = analyseStr[analyseStrI];//当前分析字符
		int currAnaltseChI = vtIndex(currAnalyseCh);
		if (currAnaltseChI == -1)
		{//当前分析字符不是终结符
			//出错
			cout << "输入的句型不是该文法的句型，程序结束！" << endl;
			exit(0);
		}
		string currAction = analyseTable[currStatus][currAnaltseChI];
		if (currAction[0] == 'S')
		{//移进操作
			showAnalyseData(currAction,"");
			//将状态入栈
			statusS[statusSI++] = ch2num(currAction[1]);
			//将当前分析字符入栈
			chS[chSI++] = currAnalyseCh;
			analyseStrI++;//移至下一个分析字符
		}
		else if(currAction[0] == 'r')
		{
			//规约操作
			string guiYueStr = program[ch2num(currAction[1])];
			int guiYueStrVN = vnIndex(guiYueStr[0]);
			int guiYueHouStatusT = statusS[statusSI - (guiYueStr.length() - 3) - 1];
			int guiYueStatus = str2num(analyseTable[guiYueHouStatusT][vtN + guiYueStrVN]);
			showAnalyseData(currAction, num2str(guiYueStatus));
			for (int i = guiYueStr.length() - 1; i > 2; i--)
			{
				if (chS[chSI - 1] == guiYueStr[i]) {
					chSI--;
					statusSI--;
				}
				else {
					cout << "规约出错!" << endl;
					exit(0);
				}
			}
			statusS[statusSI++] = guiYueStatus;//新状态入栈
			chS[chSI++] = guiYueStr[0];//规约的非终结符入栈
		}
		else if(currAction == "acc")
		{
			//接受
			if (chSI == 2 && chS[chSI - 1] == beginVN && analyseStr[analyseStrI] == '#') {
				showAnalyseData(currAction, "");
				cout << "该句型被接受！" << endl;
				exit(0);
			}
			else {
				cout << "分析出错！" << endl;
				exit(0);
			}
		}
		else {
			cout << "分析出错，该句型不是该文法的句型！" << endl;
			exit(0);
		}

	}
}


void showAnalyseData(string actionStr,string gotoStr) {
	cout << ++analyseStep << "\t";
	//输出状态栈
	for (int i = 0; i < statusSI; i++)
	{
		cout << statusS[i];
	}
	cout << "\t";
	//输出符号栈
	for (int i = 0; i < chSI; i++)
	{
		cout << chS[i];
	}
	cout << "\t";
	//输出输入串
	for (int i = analyseStrI; i < analyseStr.length(); i++)
	{
		cout << analyseStr[i];
	}
	cout << "\t";
	cout << actionStr << "\t" << gotoStr << endl;

}


//构建有效项目集
void createEffectiveSet()
{
	//将结束符加到终结符数组
	if (vtIndex('#') == -1) {
		VT[vtN++] = '#';
	}
	//将分析表二维数组初始化为'-'，代表错误
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < vnN + vtN; j++) {
			analyseTable[i][j] = "-";
		}
	}
	effProN = 0;
	proSetN = 0;
	//将产生式入有效项目数组
	for (int i = 0; i < n; i++)
	{
		if (setNofEffPro(program[i]) != -1) {
			continue;
		}
		addEffPro(proSetN++, 1, program[i]);
		int blI = effProN - 1;
		while (blI < effProN)
		{
			string currEffPro = effectiveProject[blI++];
			int setN = str2num(currEffPro.substr(0, currEffPro.find('_')));//当前项目所属项目集
			int dotIndex = str2num(currEffPro.substr(currEffPro.find('_') + 1, currEffPro.find('_', currEffPro.find('_') + 1)));//当前项目的点所在的位置
			string currProgram = currEffPro.substr(currEffPro.find('_', currEffPro.find('_') + 1) + 1);//当前项目的产生式
			currProgram = currProgram.substr(0, currProgram.length() - 2);
			//string currProgram = effectiveProject[blI].substr(effectiveProject[blI].find('_', effectiveProject[blI].find('_') + 1) + 1);
			char currCh = currProgram[3];

			for (int k = 0; k < n; k++)
			{
				if (k == i || setNofEffPro(program[k]) == setN) {
					continue;
				}
				if (program[k][0] == currCh) {
					addEffPro(proSetN - 1, 1, program[k]);
				}
			}

		}
	}
	int i = 0;
	while (i < effProN)
	{
		/*
			依次遍历项目
			点到达项目最后，
				做规约
				对分析表做相应的赋值
			点没有到达最后，
				做移进，判断点移之后的项目是否已存在，
					如果不存在
						则新建项目集，并在该项目集中查找所有可以同时点移的项目都放在这个新的项目集
						对新的项目集进行遍历，将所有可以产生的新的产生式进入该项目集
					如果点移之后的项目已存在，则什么都不做

					对分析表做相应的赋值

		*/
		string currEffPro = effectiveProject[i++];//向前项目
		int setN = str2num(currEffPro.substr(0, currEffPro.find('_')));//当前项目所属项目集
		int dotIndex = str2num(currEffPro.substr(currEffPro.find('_') + 1, currEffPro.find('_', currEffPro.find('_') + 1)));//当前项目的点所在的位置
		string currProgram = currEffPro.substr(currEffPro.find('_', currEffPro.find('_') + 1) + 1);//当前项目的产生式
		int isBL = str2num(currProgram.substr(currProgram.length() - 1));//是否已遍历过   0未遍历过   1遍历过
		currProgram = currProgram.substr(0, currProgram.length() - 2);
		if (isBL == 1)
		{//当前项目已经遍历过
			continue;
		}
		effectiveProject[i - 1][effectiveProject[i - 1].length() - 1] = '1';
		//判断点是否已到达当前项目的最后面
		if ((dotIndex + 2) == currProgram.length()) {//点已到达当前项目产生式的最后面
			//形成可规约串，找当前项目产生式的左部非终结符的follow集，该follow集对应的终结符都可进行规约
			char currProgramLeftVN = currProgram[0];//当前项目产生式的左部非终结符
			string followT = follow[vnIndex(currProgramLeftVN)];//当前项目产生式的左部非终结符的follow集
			for (int k = 0; k < followT.length(); k++)
			{//对follow集中的每一个终结符对应的分析表为规约，规约的产生式为该项目的产生式
				int currVTI = vtIndex(followT[k]);

				string currResult = "r";
				if (currProgramLeftVN == tuoGuangVN && followT[k] == '#')
				{
					currResult = "acc";
				}
				else {
					currResult.append(num2str(programIndex(currProgram)));
				}
				if (analyseTable[setN][currVTI] != "-" && analyseTable[setN][currVTI] != currResult) {
					cout << "该文法不能用SLR(1)来解决，程序结束！" << endl;
					exit(0);
				}
				else
				{
					analyseTable[setN][currVTI] = currResult;
				}
			}
		}
		else {//点未到达当前项目产生式的最后面
			//构造点后移后的产生式
			string currStr = "";
			currStr.append(num2str(dotIndex + 1));
			currStr.push_back('_');
			currStr.append(currProgram);
			//判断该点产生式是否包含在有效项目数组中
			int checkSetN = setNofEffPro(currStr);
			if (checkSetN == -1) {//该点产生式未包含在有效项目数组中
				//将该点产生式存到有效项目数组中，并分配新的有效项目集编号
				addEffPro(proSetN++, dotIndex + 1, currProgram);
				//将当前项目集中可以同时移动该点的产生式也放到给新项目集中
				char currDotch = currProgram[dotIndex + 2];
				int j = i;
				while (j < effProN)
				{
					string currEffProJ = effectiveProject[j++];
					int setNJ = str2num(currEffProJ.substr(0, currEffProJ.find('_')));//当前项目所属项目集
					int dotIndexJ = str2num(currEffProJ.substr(currEffProJ.find('_') + 1, currEffProJ.find('_', currEffProJ.find('_') + 1)));//当前项目的点所在的位置
					string currProgramJ = currEffProJ.substr(currEffProJ.find('_', currEffProJ.find('_') + 1) + 1);//当前项目的产生式
					int isBLJ = str2num(currProgramJ.substr(currProgramJ.length() - 1));//是否已遍历过   0未遍历过   1遍历过
					currProgramJ = currProgramJ.substr(0, currProgramJ.length() - 2);
					if (isBLJ == 1)
					{//当前项目已经遍历过
						continue;
					}

					if (setNJ != setN) {//当前项目集已不是正在查找的项目集
						break;
					}
					//该项目点移经过的字符是否和该产生式点移的字符相同
					char currDotChJ = currProgramJ[dotIndexJ + 2];
					if (currDotChJ == currDotch) {
						//相同
						//则将该点移后的项目也放到新的项目集中
						effectiveProject[j - 1][effectiveProject[j - 1].length() - 1] = '1';
						addEffPro(proSetN - 1, dotIndexJ + 1, currProgramJ);
					}


				}
				int p = i;
				while (p < effProN)
				{
					string currEffProP = effectiveProject[p++];
					int setNP = str2num(currEffProP.substr(0, currEffProP.find('_')));//当前项目所属项目集
					int dotIndexP = str2num(currEffProP.substr(currEffProP.find('_') + 1, currEffProP.find('_', currEffProP.find('_') + 1)));//当前项目的点所在的位置
					string currProgramP = currEffProP.substr(currEffProP.find('_', currEffProP.find('_') + 1) + 1);//当前项目的产生式
					int isBLP = str2num(currProgramP.substr(currProgramP.length() - 1));//是否已遍历过   0未遍历过   1遍历过
					currProgramP = currProgramP.substr(0, currProgramP.length() - 2);

					if (setNP == proSetN - 1) {//当前项目集是正在查找的项目集
						//点后面的字符
						char dotNextChP = currProgramP[dotIndexP + 2];
						if (vnIndex(dotNextChP) != -1)
						{
							//点后面的字符是非终结符
							//在产生式中查找左部为此非终结符的产生式进入该项目集
							for (int pk = 0; pk < n; pk++)
							{
								if (program[pk][0] == dotNextChP) {
									addEffPro(setNP, 1, program[pk]);
								}
							}
						}
					}
				}

				//构造分析表
				checkSetN = proSetN - 1;
			}






				//则该项目经过两个点之间的字符可到达检查到的项目集的编号
			char passCh = currProgram[2 + dotIndex];//两个点之间的字符
			//判断该字符是终结符还是非终结符
			int passChVTI = vtIndex(passCh);
			if (passChVTI != -1) {//该字符是终结符
				string currResult = "S";
				currResult.append(num2str(checkSetN));
				if (analyseTable[setN][passChVTI] == "-") {
					analyseTable[setN][passChVTI] = currResult;
				}
				else if(analyseTable[setN][passChVTI] != currResult){
					cout << "该文法不能用SLR(1)来解决，程序结束！" << endl;
					exit(0);
				}
			}
			else {//该字符是非终结符
				int passChVNI = vnIndex(passCh);
				string currResult = "";
				currResult.append(num2str(checkSetN));
				if (analyseTable[setN][vtN + passChVNI] == "-")
				{
					analyseTable[setN][vtN + passChVNI] = currResult;
				}
				else if(analyseTable[setN][vtN + passChVNI] != currResult){
					cout << "该文法不能用SLR(1)来解决，程序结束！" << endl;
					exit(0);
				}
			}
		}

	}
}


void addEffPro(int setN, int dotIndex, string pro) {
	string effPro = "";
	effPro.append(num2str(setN));
	effPro.push_back('_');
	effPro.append(num2str(dotIndex));
	effPro.push_back('_');
	effPro.append(pro);
	effPro.append("_0");
	if (setNofEffPro(effPro) == -1)
	{
		effectiveProject[effProN++] = effPro;
	}
}


//查找有效项目所在的有效项目集
int setNofEffPro(string str) {
	for (int i = 0; i < effProN; i++)
	{
		if (effectiveProject[i].find(str) < effectiveProject[i].length())
		{
			return str2num(effectiveProject[i].substr(0, effectiveProject[i].find('_')));
		}
	}
	return -1;
}

void showProgram() {
	cout << endl;
	cout << "转化后的文法规则：" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << program[i] << endl;
	}
}


//从文法规则中找到终结符和非终结符，保存到终结符数组和非终结符数组
void createVTVN() {
	vnN = 0;
	vtN = 0;
	for (int i = 0; i < n; i++)
	{
		if (vnIndex(program[i][0]) == -1) {
			//非终结符数组中没有改文法规则的左部表达式
			VN[vnN++] = program[i][0];
		}
		for (int j = 3; j < program[i].length(); j++)
		{
			if (isVT(program[i][j]) == 1 && vtIndex(program[i][j]) == -1) {
				VT[vtN++] = program[i][j];
			}
		}
	}
}


int vnIndex(char c) {
	for (int i = 0; i < vnN; i++)
	{
		if (VN[i] == c) {
			return i;
		}
	}
	return -1;
}

int vtIndex(char c) {
	for (int i = 0; i < vtN; i++)
	{
		if (VT[i] == c) {
			return i;
		}
	}
	return -1;
}

int isVT(char c) {
	if (c == ')' || c == '(' || (c >= 'a' && c <= 'z') || c == '+' || c == '-' || c == '*' || c == '/' || c == ';')
	{
		return 1;
	}
	return 0;
}


//显示各非终结符的first集
void showFirst() {
	cout << "First集：" << endl;
	for (int i = 0; i < vnN; i++)
	{
		cout << VN[i] << ":" << first[i]<<endl;
	}
}


//显示各非终结符的follow集
void showFollow() {
	cout << "Follow集：" << endl;
	for (int i = 0; i < vnN; i++)
	{
		cout << VN[i] << ":" << follow[i] << endl;
	}
}


int programIndex(string pro) {
	for (int i = 0; i < n; i++)
	{
		if (program[i] == pro) {
			return i;
		}
	}
	return -1;
}

void showAnalyseTable() {
	cout << "\t";
	for (int i = 0; i < vtN; i++)
	{
		cout << VT[i]<<"\t";
	}
	for (int i = 1; i < vnN; i++)
	{
		cout << VN[i] << "\t";
	}
	cout << endl;
	for (int i = 0; i < proSetN; i++)
	{
		cout << i << "\t";
		for (int j = 0; j < vtN + vnN; j++)
		{
			if (j != vtN) {
				cout << analyseTable[i][j] << "\t";
			}
		}
		cout << endl;
	}
}


string num2str(int num) {
	stringstream ss;
	ss << num;
	return ss.str();
}

int str2num(string str) {
	int num;
	stringstream ss(str);
	ss >> num;
	return num;
}


int ch2num(char c) {
	return c - 48;
}