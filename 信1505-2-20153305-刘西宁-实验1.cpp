//编译原理实验一  根据状态转换图手工构造词法扫描器
#include <stdio.h>

#include <ctype.h>
#include <string.h>

#define N 9
//保留字数组
char* keyWord[9] = { "","if","int","for","while","do","return","break","continue" };
int isKeyWord(char* token);
int isOperator(char ch);
int isSeparator(char ch);
int isKong(char ch);
void out(int s, char* token);
void analyse(FILE* fp);

int main(int argc, const char * argv[]) {
	FILE *fp = fopen("a.txt", "r");
	if (fp == NULL) {
		printf("文件打开错误,程序退出");
		return 0;
	}

	analyse(fp);

	fclose(fp);

	return 0;
}

//判断是否是保留字keyWord
int isKeyWord(char *token) {
	int i;
	int r;
	for (i = 1; i < N; i++) {
		if ((r = strcmp(keyWord[i], token)) == 0) {
			//匹配
			return 1;
		}
	}
	//不匹配
	return 0;
}

//判断是否是运算符
int isOperator(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=') {
		return 1;
	}
	return 0;
}

//判断是否是分隔符
int isSeparator(char ch) {
	if (ch == ',' || ch == ';' || ch == '{' || ch == '}' || ch == '(' || ch == ')') {
		return 1;
	}
	return 0;
}

//判断是否是需要忽略的字符
int isKong(char ch) {
	if (ch == ' ' || ch == '\t' || ch == '\n') {
		return 1;
	}
	return 0;
}


void out(int s, char* token) {
	printf("(%d,%s)\n", s, token);
}

void analyse(FILE* fp) {

	char ch;//读到的字符
	ch = fgetc(fp);
	while (!feof(fp)) {
		char token[20] = { '\0' };//全部置空
		int i = 0;//token数组下标指针
				  //只要文件没有读到末尾就循环
				  //ch = fgetc(fp);
		token[i++] = ch;
		if (isalpha(ch)) {
			//ch是字母
			while (!feof(fp)) {
				ch = fgetc(fp);
				if (isalnum(ch)) {
					token[i++] = ch;
				}
				else {
					break;
				}
			}
			if (isKeyWord(token)) {
				//是保留字
				out(1, token);
			}
			else {
				//是标识符
				out(2, token);
			}
		}
		else if (isdigit(ch)) {
			//是数字
			while (!feof(fp)) {
				ch = fgetc(fp);
				if (isdigit(ch)) {
					token[i++] = ch;
				}
				else {
					break;
				}
			}
			//是常量无符号整型数字
			out(3, token);
		}
		else if (isOperator(ch)) {
			//是运算符

			out(4, token);
			ch = fgetc(fp);
		}
		else if (isSeparator(ch)) {
			//是分隔符
			out(5, token);
			ch = fgetc(fp);
		}
		else if (isKong(ch)) {
			//是空格
			//读取下一个字符
			ch = fgetc(fp);
		}
		else {
			//读到无效字符
			printf("分析出错，文件中出现无法识别的字符:%c", ch);
			break;
		}
	}
}









