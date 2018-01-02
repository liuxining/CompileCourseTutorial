//实验七
//刘西宁


#include <iostream>
#include <string>
#include <stack>
using namespace std;

string str;
int strI = 5;
char tmpT = '1';


string op[] = {"+","-","*","/","<",">","<=",">=","<>","(",")","#"};
string rela[12][12] = {
	{">",">","<","<","<","<","<","<","<","<",">",">"},
	{">",">","<","<","<","<","<","<","<","<",">",">"},
	{">",">",">",">","<","<","<","<","<","<",">",">"},
	{">",">",">",">","<","<","<","<","<","<",">",">"},
	{">",">",">",">",">",">",">",">",">",">",">",">"},
	{">",">",">",">",">",">",">",">",">",">",">",">"},
	{">",">",">",">",">",">",">",">",">",">",">",">"},
	{">",">",">",">",">",">",">",">",">",">",">",">"},
	{">",">",">",">",">",">",">",">",">",">",">",">"},
	{"<","<","<","<","<","<","<","<","<","<","=",">"},
	{">",">",">",">",">",">",">",">",">","=","=",">"},
	{"<","<","<","<","<","<","<","<","<","<","<",">"},
};



string getNextStr();
void analyse(string);
int isOp(char);
int opIndex(string);
string compare(string,string);
void showQuad(string,string,string,string);



int main(){
	cout<<"请输入语句："<<endl;
	cin>>str;
	//str = "begina:=2+3*4;x:=(a+b)/c;end#";
	cout<<"结果为："<<endl;
	while(1){
		string tStr = getNextStr();
		if(tStr == "end"){
			break;
		}
		analyse(tStr);

	}

	return 0;
}

string getNextStr(){
	int tI = strI;
	while(strI < str.length() && (str[strI] != ';' && str[strI] != '#')){
		strI++;
	}
	strI++;
	return str.substr(tI,strI - tI - 1);
}

void analyse(string tStr){
	tStr.push_back('#');
	stack<string> chs;
	stack<string> ops;

	ops.push("#");
	int i = 0;
	while(tStr[i] != '=' && i < tStr.length()){
		i++;
	}
	i++;
	char ch = tStr[i++];
	string currStr = "";
	while(!ops.empty()){
		
		if(currStr != "" || isOp(ch)){
			if(currStr == ""){
				currStr.push_back(ch);
				if(i < tStr.length()){
					ch = tStr[i++];
					if((currStr == "<" || currStr == ">") && ((ch == '<' || ch == '>' || ch == '='))){
						currStr.push_back(ch);
					}
				}
			}
			
			string opTop = ops.top();
			string rela = compare(opTop,currStr);
			if(rela == "<"){
				ops.push(currStr);
				currStr = "";
			}
			else if(rela == ">"){
				ops.pop();
				string b = chs.top();
				chs.pop();
				string result;
				if(opTop == "#"){
					result = tStr.substr(0,tStr.find(":="));
					showQuad(opTop,b,"",result);
				}
				else{
					string a = chs.top();
					chs.pop();
					result = "t";
					result.push_back((char)(tmpT++));
					showQuad(opTop,a,b,result);
				
				}
				
				chs.push(result);
			}
			else {
				ops.pop();
				currStr = "";
			}
		}
		else{
			
			while(true){
				currStr.push_back(ch);
				if(i < tStr.length()){
					ch = tStr[i++];
				}
				else{
					break;
				}
				if(isOp(ch)){
					break;
				}
			}
			chs.push(currStr);
			currStr = "";
		}
	}

}

int isOp(char ch){
	if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '<' || ch == '>' || ch == '=' || ch == '(' || ch == ')' || ch == '#'){
		return 1;
	}
	return 0;
}

string compare(string s1,string s2){
	int i1 = opIndex(s1);
	int i2 = opIndex(s2);
	if(i1 == -1 || i2 == -1){
		cout<<"出错！！！"<<endl;
		exit(0);
	}
	return rela[i1][i2];
}

int opIndex(string opStr){
	for(int i = 0;i < 12;i++){
		if(op[i] == opStr){
			return i;
		}
	}
	return -1;
}

void showQuad(string op,string a,string b,string result){
	if(b == ""){
		cout<<result<<"="<<a<<endl;
	}
	else{
		cout<<result<<"="<<a<<op<<b<<endl;
	}
}