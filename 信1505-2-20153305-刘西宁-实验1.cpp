//����ԭ��ʵ��һ  ����״̬ת��ͼ�ֹ�����ʷ�ɨ����
#include <stdio.h>

#include <ctype.h>
#include <string.h>

#define N 9
//����������
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
		printf("�ļ��򿪴���,�����˳�");
		return 0;
	}

	analyse(fp);

	fclose(fp);

	return 0;
}

//�ж��Ƿ��Ǳ�����keyWord
int isKeyWord(char *token) {
	int i;
	int r;
	for (i = 1; i < N; i++) {
		if ((r = strcmp(keyWord[i], token)) == 0) {
			//ƥ��
			return 1;
		}
	}
	//��ƥ��
	return 0;
}

//�ж��Ƿ��������
int isOperator(char ch) {
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=') {
		return 1;
	}
	return 0;
}

//�ж��Ƿ��Ƿָ���
int isSeparator(char ch) {
	if (ch == ',' || ch == ';' || ch == '{' || ch == '}' || ch == '(' || ch == ')') {
		return 1;
	}
	return 0;
}

//�ж��Ƿ�����Ҫ���Ե��ַ�
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

	char ch;//�������ַ�
	ch = fgetc(fp);
	while (!feof(fp)) {
		char token[20] = { '\0' };//ȫ���ÿ�
		int i = 0;//token�����±�ָ��
				  //ֻҪ�ļ�û�ж���ĩβ��ѭ��
				  //ch = fgetc(fp);
		token[i++] = ch;
		if (isalpha(ch)) {
			//ch����ĸ
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
				//�Ǳ�����
				out(1, token);
			}
			else {
				//�Ǳ�ʶ��
				out(2, token);
			}
		}
		else if (isdigit(ch)) {
			//������
			while (!feof(fp)) {
				ch = fgetc(fp);
				if (isdigit(ch)) {
					token[i++] = ch;
				}
				else {
					break;
				}
			}
			//�ǳ����޷�����������
			out(3, token);
		}
		else if (isOperator(ch)) {
			//�������

			out(4, token);
			ch = fgetc(fp);
		}
		else if (isSeparator(ch)) {
			//�Ƿָ���
			out(5, token);
			ch = fgetc(fp);
		}
		else if (isKong(ch)) {
			//�ǿո�
			//��ȡ��һ���ַ�
			ch = fgetc(fp);
		}
		else {
			//������Ч�ַ�
			printf("���������ļ��г����޷�ʶ����ַ�:%c", ch);
			break;
		}
	}
}









