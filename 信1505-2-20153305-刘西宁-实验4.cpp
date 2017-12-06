//
//  Byyl_tutorial4
//  刘西宁
//

#include <iostream>
#include <stack>
#include <string>
#define BEGIN_CH 'E'
#define VNN 5
#define VTN 8

using namespace std;

//分析函数
int analyse();
int topIsVt();//判断栈顶元素是否是非终结符
int piPei();
int getVTIndex();
int getVNIndex();
void showStack();//输出分析栈字符
void showSYC();//输出剩余串
void showData(string);//输出分析信息


char vn[] = { 'E','G','T','F','S' };
char vt[] = { '+','-','*','/','(',')','i','#' };
char fxb[VNN][VTN][20] = {
	{ "-","-","-","-","E->TG","-","E->TG","-" },
	{ "G->+TG","G->-TG","","","G->^","G->^","G->^","G->^" },
	{ "-","-","-","-","T->FS","-","T->FS","-" },
	{ "-","-","-","-","F->(E)","-","F->i","-" },
	{ "S->^","S->^","S->*FS","S->/FS","S->^","S->^","S->^","S->^" }
};

char a[100];
int i = 0;//当前分析字符在符号串中的位置
int num = 0;//记录分析次数
stack<char> s = stack<char>();


void main(int argc, const char * argv[]) {
	cin >> a;
	//a = "i+i*i#";
	if (!analyse()) {
		cout << "Accept" << endl;
	}
	else {
		cout << "Error" << endl;
	}
}

int analyse()
{
	//将结束符入栈
	s.push('#');
	//将开始符号入栈
	s.push(BEGIN_CH);
	int t = 0;

	cout << "步骤\t分析栈\t剩余输入串\t推导所用产生式或匹配" << endl;
	//栈顶不是结束符号并且上次匹配正确(t=0)
	while (s.top() != '#' && t == 0)
	{
		t = piPei();
	}
	//循环结束
	//如果栈顶不是# 或者输入串没有匹配完成 或者上次匹配错误(t=1),则匹配错误
	if (s.top() != '#' || a[i] != '#' || t == 1) {
		return 1;
	}
	showData("接受");
	return 0;
}

//匹配
int piPei()
{
	//判断当前栈顶是不是终结符
	if (topIsVt())
	{
		//栈顶是终结符

		//判断栈顶元素与分析字符是否相同，相同则匹配，否则匹配错误
		if (s.top() == a[i])
		{
			//栈顶元素与分析元素相同
			//栈顶出栈

			//构造匹配信息
			string str = "\"";
			str.append(1, a[i]);
			str.append("\"");
			str.append("匹配");

			showData(str);//显示匹配信息
			s.pop();//将当前匹配的字符出栈
			i++;//移到分析串的下一个字符

			return 0;//返回0代表匹配成功
		}
		else
		{
			//匹配错误
			return 1;
		}
	}
	else
	{
		//栈顶不是终结符
		//获取栈顶元素在非终结符数组的下标
		int m = getVNIndex();
		//获取分析字符在终结符数组的下标
		int n = getVTIndex();
		if (m < 0 || m > VNN || n < 0 || n > VTN) {
			//越界，返回错误
			return 1;
		}
		//获取分析字符串
		char* chs = fxb[m][n];
		//栈顶元素与分析字符没有匹配串，出错
		if (strcmp(chs, "-") == 0) {
			return 1;
		}
		//匹配信息为分析字符串
		string str = chs;
		//显示匹配信息
		showData(str);

		//当前栈顶出栈
		s.pop();
		//判断当前匹配的分析串是不是匹配空（例如S->^）
		if (chs[3] != '^')
		{
			//X->占3个字符，右部表达式下标从3开始
			int k = 3;
			//获取匹配的右部表达式的终止下标
			while (chs[++k] != '\0');

			//将右部表达式倒序入栈
			while (--k >= 3) {
				s.push(chs[k]);
			}	
		}
		return 0;
	}
}

//判断栈顶是不是终结符
int topIsVt()
{
	int k;
	char ch = s.top();
	for (k = 0; k < VTN; k++) {
		if (vt[k] == ch) {
			return 1;//是则返回1
		}
	}
	return 0;//不是返回0
}

//获取栈顶
int getVNIndex()
{
	int k;
	char ch = s.top();
	for (k = 0; k < VNN; k++) {
		if (vn[k] == ch) {
			return k;
		}
	}
	return -1;
}


int getVTIndex()
{
	int k;
	char ch = a[i];
	for (k = 0; k < VTN; k++) {
		if (vt[k] == ch) {
			return k;
		}
	}
	return -1;
}

//输出分析栈的内容
void showStack()
{
	//cout <<"size:"<< s.size();
	char *st = new char[s.size() + 1];
	int st_i = (int)s.size();
	int st_it = st_i;
	st[st_i--] = '\0';
	while (!s.empty()) {
		st[st_i--] = s.top();
		s.pop();
	}
	for (int st_ir = 0; st_ir < st_it; st_ir++)
	{
		s.push(st[st_ir]);
	}

	cout << st;
}


//输出剩余串
void showSYC() {
	cout << a + i;
}

//输出分析信息
void showData(string str)
{
	cout << ++num << "\t";
	showStack();
	cout << "\t";
	showSYC();
	cout << "\t\t";
	cout << str << endl;
}


