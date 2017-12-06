//
//  byyl_Tutorial5
//	刘西宁
//

#include <iostream>
#include <string>
#include <stack>

using namespace std;

string programOrigin[20];//初始输入的文法
string program[30];//转化后的文法
char VT[20];//终结符数组
char VN[20];//非终结符数组
string replace[20];//可以替换的非终结符
string replaceM[20];
int replaceN;//可以替换的非终结符的个数
int firstVT[20][20] = {0};
int lastVT[20][20] = {0};
char relation[20][20];
string analyseStr;
char analyseS[50];//模拟分析栈
int sk;//分析栈数组实际长度
int ap;//分析字符的下标
int step;//分析步骤

int no;//初始的文法规则数
int n;//转化后的文法规则数
int splitI;//分割字符串的下标
int vtn;//终结符个数
int vnn;//非终结符个数
string splitStr;//用于分割的字符串
string splitSubStr;//用于分割的字符串的分隔符

void transform();//转化文法
string nextSubStr();//获取下一个分隔字符串子串
void createVTVN();
void createFirstVT();
void createLastVT();
void createRelation();//构建优先关系表
void createReplace();//创建可以替换的非终结符数组

int analyse();
char guiYue(string);
char guiYueSimple(string);

int vtIndex(char);//检查是否已经存在该终结符
int vnIndex(char);//检查是否已经存在该非终结符

int isVT(char);//判断该字符是否为终结符

void showProgram();//显示转化后的文法
void showFirstVT();//显示firstVT数组
void showLastVT();
void showRelation();//显示优先关系表
void showAnalyseData(char,string);//显示分析信息




int main(int argc, const char * argv[]) {
	cout << "请输入文法规则数：";
	cin >> no;
	cout << "请输入文法规则:" << endl;
	for (int i = 0; i < no; i++) {
		cin >> programOrigin[i];
	}
	
	/*no = 4;
	programOrigin[0] = "S->S;G|G";
	programOrigin[1] = "G->G(T)|H";
	programOrigin[2] = "H->a|(S)";
	programOrigin[3] = "T->T+S|S";*/
	transform();
	showProgram();
	createVTVN();
	createFirstVT();
	showFirstVT();
	createLastVT();
	showLastVT();
	createRelation();
	showRelation();
	cout << "请输入分析串(#结束)：";
	cin >> analyseStr;
	//analyseStr = "a;(a+a)#";
	int result = analyse();
	if (result == 1) {
		cout << "Accept" << endl;
	}
	else {
		cout << "Error" << endl;
	}
	return 0;
}


//转化文法
void transform() {
	n = 0;
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

void showProgram() {
	cout << endl;
	cout << "转化后的文法规则：" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << program[i] << endl;
	}
}

void createFirstVT()
{
	//初始化栈
	stack<string> s = stack<string>();
	
	//遍历文法规则数组，找出终结符和非终结符以及直接firstVT集合
	for (int i = 0; i < n; i++)
	{
		char leftVN = program[i][0];
		
		//从该文法规则右部表达式中查找第一个终结符
		for (int k = 3; k < program[i].length(); k++)
		{
			char rightCh = program[i][k];
			if (isVT(rightCh) == 1) {
				//是终结符
				
				//将该非终结符和终结符入栈

				//将该对应的坐标赋为1
				int h = vnIndex(leftVN);
				int l = vtIndex(rightCh);
				if (firstVT[h][l] == 0) {
					firstVT[h][l] = 1;
					string spt = "";
					spt.push_back(leftVN);
					spt.push_back(',');
					spt.push_back(rightCh);
					s.push(spt);
				}
				break;
			}
		}
	}

	while (!s.empty()) {
		//弹栈
		string sp = s.top();
		s.pop();
		char spVN = sp[0];
		char spVT = sp[2];
		//遍历文法规则
		for (int i = 0; i < n; i++)
		{
			//判断该文法规则的右部第一个字符是否为非终结符,且如果为非终结符判断是否是该栈顶的非终结符
			if (program[i][3] == spVN) {
				//将该栈顶的终结符与该文法规则的左部非终结符入栈
				int h = vnIndex(program[i][0]);
				int l = vtIndex(spVT);
				if (firstVT[h][l] == 0) {
					firstVT[h][l] = 1;
					string spt = "";
					spt.push_back(program[i][0]);
					spt.push_back(',');
					spt.push_back(spVT);
					s.push(spt);
				}
			}
		}

	}
}



void createLastVT()
{
	//初始化栈
	stack<string> s = stack<string>();
	
	//遍历文法规则数组，找出终结符和非终结符以及直接firstVT集合
	for (int i = 0; i < n; i++)
	{
		char leftVN = program[i][0];
		
		//从该文法规则右部表达式中查找最后一个终结符
		for (int k = program[i].length() - 1; k > 2; k--)
		{
			char rightCh = program[i][k];
			if (isVT(rightCh) == 1) {
				//是终结符
				
				//将该非终结符和终结符入栈

				//将该对应的坐标赋为1
				int h = vnIndex(leftVN);
				int l = vtIndex(rightCh);
				if (lastVT[h][l] == 0) {
					lastVT[h][l] = 1;
					string spt = "";
					spt.push_back(leftVN);
					spt.push_back(',');
					spt.push_back(rightCh);
					s.push(spt);
				}
				break;
			}
		}
	}

	while (!s.empty()) {
		//弹栈
		string sp = s.top();
		s.pop();
		char spVN = sp[0];
		char spVT = sp[2];
		//遍历文法规则
		for (int i = 0; i < n; i++)
		{
			//判断该文法规则的右部最后一个字符是否为非终结符,且如果为非终结符判断是否是该栈顶的非终结符
			if (program[i][program[i].length() - 1] == spVN) {
				//将该栈顶的终结符与该文法规则的左部非终结符入栈
				int h = vnIndex(program[i][0]);
				int l = vtIndex(spVT);
				if (lastVT[h][l] == 0) {
					lastVT[h][l] = 1;
					string spt = "";
					spt.push_back(program[i][0]);
					spt.push_back(',');
					spt.push_back(spVT);
					s.push(spt);
				}
			}
		}

	}
}

//构建优先关系表
void createRelation()
{
	//先将关系表初始化为空
	for (int i = 0; i < vtn + 1; i++)
	{
		for (int j = 0; j < vtn + 1; j++) {
			relation[i][j] = ' ';
		}
	}
	for (int i = 0; i < n; i++)
	{
		string pro = program[i];
		//对文法中所有的终结符相等
		for (int p = 3; p < pro.length() - 1; p++)
		{
			int pt = vtIndex(pro[p]);
			if (pt != -1) {
				//pro[p]是终结符
				for (int  q = p + 1; q < pro.length(); q++)
				{
					int qt = vtIndex(pro[q]);
					if (qt != -1) {
						relation[pt][qt] = '=';
					}
				}
			}
		}
		for (int j = 3; j < program[i].length() - 1; j++) {
			int vtj = vtIndex(pro[j]);
			int vnj = vnIndex(pro[j]);
			int vtj1 = vtIndex(pro[j + 1]);
			int vnj1 = vnIndex(pro[j + 1]);
			if (vtj != -1 && vtj1 != -1) {
				//j和j+1处的字符都是终结符
				relation[vtj][vtj1] = '=';
			}
			else if (vtj != -1 && vnj1 != -1) {
				//j是终结符，j+1是非终结符
				//则j < firstVT(j+1)
				for (int k = 0; k < vtn; k++)
				{
					if (firstVT[vnj1][k] == 1) {
						relation[vtj][k] = '<';
					}
				}
			}
			else if (vnj != -1 && vtj1 != -1) {
				//j是非终结符，j+1是终结符
				//lastVT(j) > j+1
				for (int  k = 0; k < vtn; k++)
				{
					if (lastVT[vnj][k] == 1) {
						relation[k][vtj1] = '>';
					}
				}
			}
			else {
				cout << "文法错误！" << endl;
			}
		}
	}
	//结束符
	for (int i = 0; i < vtn; i++)
	{
		relation[i][vtn] = '>';
		relation[vtn][i] = '<';
	}
	relation[vtn][vtn] = '=';
}

void createReplace()
{
	replaceN = 0;
	for (int i = 0; i < n; i++)
	{
		if (program[i].length() == 4 && vnIndex(program[i][3]) != -1) {
			string t = "";
			t.push_back(program[i][0]);
			//t.push_back(',');
			t.push_back(program[i][3]);
			replace[replaceN++] = t;
		}
	}

	for (int i = 0; i < replaceN; i++)
	{
		for (int k = 0; k < replace[i].length(); k++)
		{
			char ch = replace[i][k];
			for (int j = 0; j < replaceN; j++)
			{
				/*if (i == j) {
					continue;
				}*/
				int index_ch = replace[j].find_first_of(ch);
				if (index_ch != -1) {
					for (int p = 0; p < replace[j].length(); p++)
					{
						if (replace[i].find_first_of(replace[j][p]) == -1) {
							replace[i].push_back(replace[j][p]);
						}
					}
					/*replace[i].append(replace[j].substr(0, index_ch));
					if(replace[j].length() > (index_ch + 1))
					replace[i].append(replace[j].substr(index_ch + 1));*/
				}
			}

		}
	}
	
	for (int i = 0; i < vnn; i++)
	{
		replaceM[i] = "";
	}
	for (int i = 0; i < replaceN; i++)
	{
		for (int j = 0; j < replace[i].length(); j++)
		{
			char ch = replace[i][j];
			int chI = vnIndex(ch);
			for (int k = 0; k < replace[i].length(); k++)
			{
				if (k != j && replaceM[chI].find_first_of(replace[i][k]) == -1) {
					replaceM[chI].push_back(replace[i][k]);
				}
			}
		}
	}
	
}



int vnIndex(char c) {
	for (int i = 0; i < vnn; i++)
	{
		if (VN[i] == c) {
			return i;
		}
	}
	return -1;
}

int vtIndex(char c) {
	for (int i = 0; i < vtn; i++)
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

void showFirstVT()
{
	cout << endl;
	cout << "firstVT:" << endl;
	for (int i = 0; i < vnn; i++)
	{
		cout << VN[i]<<":\t";
		for (int j = 0; j < vtn; j++)
		{
			if (firstVT[i][j] == 1) {
				cout << VT[j] << "\t";
			}
		}
		cout << endl;
	}
	/*cout << "\t";
	for (int l = 0; l < vtn; l++) {
		cout << VT[l] << "\t";
	}
	cout << endl;
	for (int h = 0; h < vnn; h++) {
		cout << VN[h]<<"\t";
		for (int l = 0; l < vtn; l++) {
			cout << firstVT[h][l] << "\t";
		}
		cout << endl;
	}*/
}

void showLastVT()
{
	cout << endl;
	cout << "lastVT:" << endl;
	//cout << "\t";

	for (int i = 0; i < vnn; i++)
	{
		cout << VN[i] << ":\t";
		for (int j = 0; j < vtn; j++)
		{
			if (lastVT[i][j] == 1) {
				cout << VT[j] << "\t";
			}
		}
		cout << endl;
	}




	/*for (int l = 0; l < vtn; l++) {
		cout << VT[l] << "\t";
	}
	cout << endl;
	for (int h = 0; h < vnn; h++) {
		cout << VN[h] << "\t";
		for (int l = 0; l < vtn; l++) {
			cout << lastVT[h][l] << "\t";
		}
		cout << endl;
	}*/
}

//显示优先关系表
void showRelation()
{
	cout << endl;
	cout << "优先关系表:" << endl;
	cout << "\t";
	for (int l = 0; l < vtn; l++) {
		cout << VT[l] << "\t";
	}
	cout << "#";
	cout << endl;
	for (int h = 0; h < vtn; h++) {
		cout << VT[h] << "\t";
		for (int l = 0; l < vtn; l++) {
			cout << relation[h][l] << "\t";
		}
		cout << relation[h][vtn];
		cout << endl;
	}
	cout << "#\t";
	for (int i = 0; i < vtn; i++)
	{
		cout << relation[vtn][i] << "\t";
	}
	cout << endl;
	
}

//从文法规则中找到终结符和非终结符，保存到终结符数组和非终结符数组
void createVTVN() {
	vnn = 0;
	vtn = 0;
	for (int i = 0; i < n; i++)
	{
		if (vnIndex(program[i][0]) == -1) {
			//非终结符数组中没有改文法规则的左部表达式
			VN[vnn++] = program[i][0];
		}
		for (int j = 3; j < program[i].length(); j++)
		{
			if (isVT(program[i][j]) == 1 && vtIndex(program[i][j]) == -1) {
				VT[vtn++] = program[i][j];
			}
		}
	}
}

//分析
int analyse() {
	createReplace();
	cout << "步骤\t栈\t关系\t分析字符\t剩余串\t\t操作" << endl;

	step = 0;//

	//将结束符存入终结符数组中
	VT[vtn++] = '#';
	sk = 0;
	analyseS[sk] = '#';
	int depth;
	while (!(sk == 1 && analyseStr[ap] == '#'))
	{
		char ch = analyseStr[ap];
		depth = 0;
		int skI = vtIndex(analyseS[sk]);
		//在栈中找距离栈顶最近的终结符

		while (skI == -1) {
			depth++;
			skI = vtIndex(analyseS[sk - depth]);
		}

		int fxI = vtIndex(ch);

		if (relation[skI][fxI] == '<' || relation[skI][fxI] == '=') {
			//分析字符入栈
			showAnalyseData('<',"移进");
			analyseS[++sk] = ch;
			ap++;
		}
		else if (relation[skI][fxI] == '>')
		{
			//规约
			showAnalyseData('>',"规约");
			char postVT = analyseS[sk - depth];
			int depth2 = 0;
			int postVTI = vtIndex(postVT);
			while (depth2 < sk)
			{
				++depth2;
				char currV = analyseS[sk - depth - depth2];
				int currVI = vtIndex(currV);
				if (currVI != -1) {
					//是终结符
					if (relation[currVI][postVTI] != '=') {
						break;
					}
					else {
						postVTI = currVI;
					}
				}
			}
			string guiYueStr = "";
			for (int ppi = sk - depth - depth2 + 1; ppi <= sk; ppi++)
			{
				guiYueStr.push_back(analyseS[ppi]);
				//analyseS[ppi] = '\0';
 			}
			char guiYueResult = guiYue(guiYueStr);
			if (guiYueResult == '\0') {
				//cout << "分析错误" << endl;
				return 0;
			}
			sk = sk - depth - depth2 + 1;
			analyseS[sk] = guiYueResult;
		}
		else {
			//cout << "分析错误" << endl;
			return 0;
		}
	}
	showAnalyseData(' ',"结束");
	if (analyseStr[ap] == '#') {
		//cout << "分析错误" << endl;
		return 1;
	}
	return 0;
	
}

//查找符合符合右部表达式为guiYueStr的文法规则的左部表达式
char guiYue(string guiYueStr) {

	for (int i = 0; i < n; i++)
	{
		string programT = program[i].substr(3);
		if (programT == guiYueStr) {
			return program[i][0];
		}
	}

	for (int i = 0; i < n; i++)
	{
		string programT = program[i].substr(3);
		//判断长度是否相同
		if (programT.length() != guiYueStr.length()) {
			continue;
		}
		
		//判断非终结符是否相同或是否可替换
		for (int j = 0; j < programT.length(); j++)
		{
			if (!(programT[j] == guiYueStr[j] || (vnIndex(guiYueStr[j]) != -1 && replaceM[vnIndex(guiYueStr[j])].find_first_of(programT[j]) != -1))) {
				continue;
			}
		}
		return program[i][0];

	}
	
	
	
	
	
	return '\0';
}




void showAnalyseData(char re,string opt) {
	cout << ++step << "\t";
	//输出栈
	for (int i = 0; i <= sk; i++)
	{
		cout << analyseS[i];
	}
	cout << "\t";
	cout << re << "\t";
	cout << analyseStr[ap]<<"\t\t";
	for (int  j = ap; j < analyseStr.length(); j++)
	{
		cout << analyseStr[j];
	}
	cout << "\t\t";
	cout << opt << endl;

}