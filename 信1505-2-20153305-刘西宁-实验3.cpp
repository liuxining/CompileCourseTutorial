/*
����ԭ��ʵ����
*/


#include <stdio.h>
#include <stdlib.h>

void E();
void G();
void T();
void F();
void S();
void match();
void error();
void showData(char*, char);
void printFXC();
void printSYC();


char* str = new char[100];
int i = 0;

void main()
{
	printf("��������Ŵ�(#����)\n");
	scanf("%s", str);
	printf("�ķ�\t������\t�����ַ�\tʣ�മ\n");
	E();
	if (str[i] == '#') {
		printf("Accept\n");
	}
}

void E()
{
	char ch = str[i];
	if (str[i] == '(' || str[i] == 'i')
	{
		showData("E->TG", ch);
		T();
		G();
	}
	else
	{
		error();
	}
}

void G()
{
	char ch = str[i];
	if (str[i] == '+' || str[i] == '-')
	{
		match();
		if (ch == '+')
		{
			showData("G->+TG", ch);
		}
		else
		{
			showData("G->-TG", ch);
		}
		T();
		G();
	}
	else if (str[i] == ')' || str[i] == '#' || str[i] == '(' || str[i] == 'i')
	{
		showData("G->^", ch);
	}
	else
	{
		error();
	}
}

void T()
{
	char ch = str[i];
	if (str[i] == '(' || str[i] == 'i')
	{
		showData("T->FS", ch);
		F();
		S();
	}
	else {
		error();
	}
}

void F()
{
	char ch = str[i];
	if (str[i] == '(')
	{
		match();
		showData("F->(E)", ch);
		E();
		if (ch == ')')
		{
			match();
			showData(")->)", ')');
		}
		else {
			error();
		}
	}
	else if (str[i] == 'i')
	{
		match();
		showData("F->i", 'i');
	}
	else
	{
		error();
	}
}

void S()
{
	char ch = str[i];
	if (str[i] == '+' || str[i] == '-' || str[i] == ')' || str[i] == '#' || str[i] == '(' || str[i] == 'i')
	{
		showData("S->^", ch);
	}
	else if (str[i] == '*' || str[i] == '/')
	{
		match();
		if (ch == '*') {
			showData("S->*FS", ch);
		}
		else
		{
			showData("S->/FS", ch);
		}
		F();
		S();
	}
}

void match()
{
	if (str[i] != '#') {
		i++;
	}

}

//��ӡ������
void printFXC()
{
	for (int j = 0; j < i; j++)
	{
		printf("%c", str[j]);
	}
}

//��ӡʣ�മ
void printSYC()
{
	printf("%s", str + i);
}

void error()
{
	printf("error\n");
	exit(0);
}

void showData(char* str, char ch)
{
	printf("%s\t", str);
	printFXC();
	printf("\t%c\t\t", ch);
	printSYC();
	printf("\n");
}