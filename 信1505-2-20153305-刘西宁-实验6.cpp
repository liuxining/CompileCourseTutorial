//2017��12��8��13:29:24
//������
//����ԭ�� ʵ��6

/*
	�����ķ���ʼ����Ϊ^    ע��������ķ��в��ܰ���^
	����~����ʾ
	��Ŀ��ʽ
	��Ŀ�����_���λ��_����ʽ_�Ƿ������ �� 0_1_S->E_0
	������error��-��ʾ
*/

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <sstream>

using namespace std;

//ȫ�ֱ�������
string programOrigin[20];//��ʼ������ķ�
string program[30];//ת������ķ�
char VT[20];//�ս������
char VN[20];//���ս������
string first[30];//first��
string follow[30];//follow��
string effectiveProject[100];//��Ч��Ŀ
string analyseTable[100][30];
string analyseStr;//������

int statusS[100];//״̬ջ
char chS[100];//����ջ

int statusSI;//״̬ջʵ�ʳ���
int chSI;//����ջʵ�ʳ���

char beginVN;//�ķ���ʼ���ս��
char tuoGuangVN;
int analyseStep;
int analyseStrI;//��ǰ�����ַ��ڷ������е��±�
int effProN;//��Ч��Ŀʵ�ʸ���
int proSetN;//��Ч��Ŀ������
int no;//��ʼ���ķ�������
int n;//ת������ķ�������
int splitI;//�ָ��ַ������±�
int vtN;//�ս������
int vnN;//���ս������
string splitStr;//���ڷָ���ַ���
string splitSubStr;//���ڷָ���ַ����ķָ���


//��������
void transform();//ת���ķ�
string nextSubStr();//��ȡ��һ���ָ��ַ����Ӵ�

int ch2num(char c);//�ַ�ת����
void createVTVN();//�������е�vt/vn���浽������

void createFirst();//����first��
void createFollow();//����Follow��

int vtIndex(char c);
int vnIndex(char c);
int isVT(char c);

string num2str(int num);
int str2num(string str);

int programIndex(string pro);

void createEffectiveSet();//������Ч��Ŀ��
int setNofEffPro(string str);

void analyse();
void showAnalyseData(string, string);


void addEffPro(int setN, int dotIndex, string pro);
void showProgram();
void showFirst();//��ʾ�����ս����first��

void showFollow();//��ʾ�����ս����follow��

void showAnalyseTable();


void main() {
	cout << "�������ķ���������";
	cin >> no;
	cout << "�������ķ�����(��һ��Ϊ��չ��Ĳ���ʽ):" << endl;
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
	cout << "������Ҫ�������ַ�����#������:";
	cin >> analyseStr;
	//analyseStr = "i+i*i#";
	analyse();
	

}









//ת���ķ�
void transform() {
	n = 0;
	//string begin = "^->";
	//begin.push_back(programOrigin[0][0]);
	//program[n++] = begin;
	for (int i = 0; i < no; i++) {
		//�ж��ķ��������Ƿ���� | ����
		if (programOrigin[i].find("|") != string::npos) {
			//����
			//�򽫸��ķ����в��
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
			//��������|
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
			//ƥ��
			f = 1;
			splitI += splitSubStr.length() - 1;
			break;
		}
	}
	t = splitStr.substr(beginI, splitI - f);
	return t;
}

//����first��
void createFirst()
{
	queue<char> q = queue<char>();
	//�������ս���������еķ��ս�������,����ʼ��first��
	for (int i = vnN - 1; i >= 0; i--)
	{
		q.push(VN[i]);
		first[i] = "";
	}
	while (!q.empty())
	{
		//ȡ�����еĶ�ͷԪ��
		char vn = q.front();
		q.pop();
		int flag = 0;
		string firstT = "";
		//��������ʽ
		for (int i = 0; i < n; i++)
		{
			//�ò���ʽ���󲿷��ս��Ϊvn
			if (program[i][0] == vn) {
				int rightIndex = 3;
				while (rightIndex < program[i].length() && flag == 0)
				{
					char ch = program[i][rightIndex++];
					int vtI = vtIndex(ch);
					//����Ϊ�ս��
					if (vtI != -1 || ch == '~') {
						if (!(firstT.find(ch) < firstT.length())) {
							firstT.push_back(ch);
						}
						break;
					}
					else {
						//�ò���ʽ���Ҳ���ʼ����Ϊ���ս��
						int vnI = vnIndex(ch);
						//����ø����ս����first���������ӵ�vn��first��
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

//����follow����ǰ���Ǳ����ȹ�����first��
void createFollow()
{
	//�ķ���ʼ���ŵ�follow��Ϊ������#
	follow[0] = "#";
	queue<char> q = queue<char>();
	//�������ս���������еķ��ս��(�ķ���ʼ���ų���)�����,����ʼ��follow��
	for (int i = 1; i < vnN; i++)
	{
		q.push(VN[i]);
		follow[i] = "";
	}
	while (!q.empty())
	{
		//ȡ�����еĶ�ͷԪ��
		char vn = q.front();
		q.pop();
		int flag = 0;
		string followT = "";
		//��������ʽ
		for (int i = 0; i < n; i++)
		{
			string programT = program[i].substr(3);//ȡ�ò���ʽ���Ҳ����ʽ
			//���Ҹ÷��ս���ڸò���ʽ�Ҳ����±�
			int vnIndexOfProgramRight = programT.find(vn);
			if (vnIndexOfProgramRight < programT.length()) {//�ڸò���ʽ�Ҳ����ڸ÷��ս��
				if (vnIndexOfProgramRight != programT.length() - 1)
				{
					char thisVNLastCh = programT[vnIndexOfProgramRight + 1];
					if (vtIndex(thisVNLastCh) != -1) {//�����һ���ַ����ս��
						//�����ս�����뵽�÷��ս����follow����
						if (followT.find(thisVNLastCh) >= followT.length())
						{
							followT.push_back(thisVNLastCh);
						}
					}
					else {//�����һ���ַ��Ƿ��ս��
						//�����ս����first���ķǿ�Ԫ�ؼ��뵽�÷��ս����follow����
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
				//ȡ���ò���ʽ�Ҳ��и÷��ս��������ַ���
				string lastStr = vnIndexOfProgramRight == programT.length() - 1 ? "" : programT.substr(vnIndexOfProgramRight + 1);
				int hasNotNull = 0;//����ı��ʽʱ���в������յģ�0û�У�1��
				for (int k = 0; k < lastStr.length(); k++)
				{
					int lastStrVNI = vnIndex(lastStr[k]);
					//�жϸ÷��ս����first�Ƿ��п�
					if (lastStrVNI == -1 || first[lastStrVNI].find('~') >= first[lastStrVNI].length())
					{
						hasNotNull = 1;
						break;
					}
				}
				if (hasNotNull == 0) {//����ı��ʽ���Բ�����
					//�ò���ʽ���󲿷��ս����follow�����ڸ÷��ս����follow��
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
	cout << "����\t״̬ջ\t����ջ\t���봮\tACTION\tGOTO" << endl;
	analyseStep = 0;
	analyseStrI = 0;
	statusSI = 0;
	chSI = 0;
	statusS[statusSI++] = 0;//��ʼ״̬��ջ
	chS[chSI++] = '#';//����������ջ

	while (!(chSI == 0 || statusSI == 0 || analyseStrI == analyseStr.length()))
	{
		int currStatus = statusS[statusSI - 1];//��ǰ״̬
		char currAnalyseCh = analyseStr[analyseStrI];//��ǰ�����ַ�
		int currAnaltseChI = vtIndex(currAnalyseCh);
		if (currAnaltseChI == -1)
		{//��ǰ�����ַ������ս��
			//����
			cout << "����ľ��Ͳ��Ǹ��ķ��ľ��ͣ����������" << endl;
			exit(0);
		}
		string currAction = analyseTable[currStatus][currAnaltseChI];
		if (currAction[0] == 'S')
		{//�ƽ�����
			showAnalyseData(currAction,"");
			//��״̬��ջ
			statusS[statusSI++] = ch2num(currAction[1]);
			//����ǰ�����ַ���ջ
			chS[chSI++] = currAnalyseCh;
			analyseStrI++;//������һ�������ַ�
		}
		else if(currAction[0] == 'r')
		{
			//��Լ����
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
					cout << "��Լ����!" << endl;
					exit(0);
				}
			}
			statusS[statusSI++] = guiYueStatus;//��״̬��ջ
			chS[chSI++] = guiYueStr[0];//��Լ�ķ��ս����ջ
		}
		else if(currAction == "acc")
		{
			//����
			if (chSI == 2 && chS[chSI - 1] == beginVN && analyseStr[analyseStrI] == '#') {
				showAnalyseData(currAction, "");
				cout << "�þ��ͱ����ܣ�" << endl;
				exit(0);
			}
			else {
				cout << "��������" << endl;
				exit(0);
			}
		}
		else {
			cout << "���������þ��Ͳ��Ǹ��ķ��ľ��ͣ�" << endl;
			exit(0);
		}

	}
}


void showAnalyseData(string actionStr,string gotoStr) {
	cout << ++analyseStep << "\t";
	//���״̬ջ
	for (int i = 0; i < statusSI; i++)
	{
		cout << statusS[i];
	}
	cout << "\t";
	//�������ջ
	for (int i = 0; i < chSI; i++)
	{
		cout << chS[i];
	}
	cout << "\t";
	//������봮
	for (int i = analyseStrI; i < analyseStr.length(); i++)
	{
		cout << analyseStr[i];
	}
	cout << "\t";
	cout << actionStr << "\t" << gotoStr << endl;

}


//������Ч��Ŀ��
void createEffectiveSet()
{
	//���������ӵ��ս������
	if (vtIndex('#') == -1) {
		VT[vtN++] = '#';
	}
	//���������ά�����ʼ��Ϊ'-'���������
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < vnN + vtN; j++) {
			analyseTable[i][j] = "-";
		}
	}
	effProN = 0;
	proSetN = 0;
	//������ʽ����Ч��Ŀ����
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
			int setN = str2num(currEffPro.substr(0, currEffPro.find('_')));//��ǰ��Ŀ������Ŀ��
			int dotIndex = str2num(currEffPro.substr(currEffPro.find('_') + 1, currEffPro.find('_', currEffPro.find('_') + 1)));//��ǰ��Ŀ�ĵ����ڵ�λ��
			string currProgram = currEffPro.substr(currEffPro.find('_', currEffPro.find('_') + 1) + 1);//��ǰ��Ŀ�Ĳ���ʽ
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
			���α�����Ŀ
			�㵽����Ŀ���
				����Լ
				�Է���������Ӧ�ĸ�ֵ
			��û�е������
				���ƽ����жϵ���֮�����Ŀ�Ƿ��Ѵ��ڣ�
					���������
						���½���Ŀ�������ڸ���Ŀ���в������п���ͬʱ���Ƶ���Ŀ����������µ���Ŀ��
						���µ���Ŀ�����б����������п��Բ������µĲ���ʽ�������Ŀ��
					�������֮�����Ŀ�Ѵ��ڣ���ʲô������

					�Է���������Ӧ�ĸ�ֵ

		*/
		string currEffPro = effectiveProject[i++];//��ǰ��Ŀ
		int setN = str2num(currEffPro.substr(0, currEffPro.find('_')));//��ǰ��Ŀ������Ŀ��
		int dotIndex = str2num(currEffPro.substr(currEffPro.find('_') + 1, currEffPro.find('_', currEffPro.find('_') + 1)));//��ǰ��Ŀ�ĵ����ڵ�λ��
		string currProgram = currEffPro.substr(currEffPro.find('_', currEffPro.find('_') + 1) + 1);//��ǰ��Ŀ�Ĳ���ʽ
		int isBL = str2num(currProgram.substr(currProgram.length() - 1));//�Ƿ��ѱ�����   0δ������   1������
		currProgram = currProgram.substr(0, currProgram.length() - 2);
		if (isBL == 1)
		{//��ǰ��Ŀ�Ѿ�������
			continue;
		}
		effectiveProject[i - 1][effectiveProject[i - 1].length() - 1] = '1';
		//�жϵ��Ƿ��ѵ��ﵱǰ��Ŀ�������
		if ((dotIndex + 2) == currProgram.length()) {//���ѵ��ﵱǰ��Ŀ����ʽ�������
			//�γɿɹ�Լ�����ҵ�ǰ��Ŀ����ʽ���󲿷��ս����follow������follow����Ӧ���ս�����ɽ��й�Լ
			char currProgramLeftVN = currProgram[0];//��ǰ��Ŀ����ʽ���󲿷��ս��
			string followT = follow[vnIndex(currProgramLeftVN)];//��ǰ��Ŀ����ʽ���󲿷��ս����follow��
			for (int k = 0; k < followT.length(); k++)
			{//��follow���е�ÿһ���ս����Ӧ�ķ�����Ϊ��Լ����Լ�Ĳ���ʽΪ����Ŀ�Ĳ���ʽ
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
					cout << "���ķ�������SLR(1)����������������" << endl;
					exit(0);
				}
				else
				{
					analyseTable[setN][currVTI] = currResult;
				}
			}
		}
		else {//��δ���ﵱǰ��Ŀ����ʽ�������
			//�������ƺ�Ĳ���ʽ
			string currStr = "";
			currStr.append(num2str(dotIndex + 1));
			currStr.push_back('_');
			currStr.append(currProgram);
			//�жϸõ����ʽ�Ƿ��������Ч��Ŀ������
			int checkSetN = setNofEffPro(currStr);
			if (checkSetN == -1) {//�õ����ʽδ��������Ч��Ŀ������
				//���õ����ʽ�浽��Ч��Ŀ�����У��������µ���Ч��Ŀ�����
				addEffPro(proSetN++, dotIndex + 1, currProgram);
				//����ǰ��Ŀ���п���ͬʱ�ƶ��õ�Ĳ���ʽҲ�ŵ�������Ŀ����
				char currDotch = currProgram[dotIndex + 2];
				int j = i;
				while (j < effProN)
				{
					string currEffProJ = effectiveProject[j++];
					int setNJ = str2num(currEffProJ.substr(0, currEffProJ.find('_')));//��ǰ��Ŀ������Ŀ��
					int dotIndexJ = str2num(currEffProJ.substr(currEffProJ.find('_') + 1, currEffProJ.find('_', currEffProJ.find('_') + 1)));//��ǰ��Ŀ�ĵ����ڵ�λ��
					string currProgramJ = currEffProJ.substr(currEffProJ.find('_', currEffProJ.find('_') + 1) + 1);//��ǰ��Ŀ�Ĳ���ʽ
					int isBLJ = str2num(currProgramJ.substr(currProgramJ.length() - 1));//�Ƿ��ѱ�����   0δ������   1������
					currProgramJ = currProgramJ.substr(0, currProgramJ.length() - 2);
					if (isBLJ == 1)
					{//��ǰ��Ŀ�Ѿ�������
						continue;
					}

					if (setNJ != setN) {//��ǰ��Ŀ���Ѳ������ڲ��ҵ���Ŀ��
						break;
					}
					//����Ŀ���ƾ������ַ��Ƿ�͸ò���ʽ���Ƶ��ַ���ͬ
					char currDotChJ = currProgramJ[dotIndexJ + 2];
					if (currDotChJ == currDotch) {
						//��ͬ
						//�򽫸õ��ƺ����ĿҲ�ŵ��µ���Ŀ����
						effectiveProject[j - 1][effectiveProject[j - 1].length() - 1] = '1';
						addEffPro(proSetN - 1, dotIndexJ + 1, currProgramJ);
					}


				}
				int p = i;
				while (p < effProN)
				{
					string currEffProP = effectiveProject[p++];
					int setNP = str2num(currEffProP.substr(0, currEffProP.find('_')));//��ǰ��Ŀ������Ŀ��
					int dotIndexP = str2num(currEffProP.substr(currEffProP.find('_') + 1, currEffProP.find('_', currEffProP.find('_') + 1)));//��ǰ��Ŀ�ĵ����ڵ�λ��
					string currProgramP = currEffProP.substr(currEffProP.find('_', currEffProP.find('_') + 1) + 1);//��ǰ��Ŀ�Ĳ���ʽ
					int isBLP = str2num(currProgramP.substr(currProgramP.length() - 1));//�Ƿ��ѱ�����   0δ������   1������
					currProgramP = currProgramP.substr(0, currProgramP.length() - 2);

					if (setNP == proSetN - 1) {//��ǰ��Ŀ�������ڲ��ҵ���Ŀ��
						//�������ַ�
						char dotNextChP = currProgramP[dotIndexP + 2];
						if (vnIndex(dotNextChP) != -1)
						{
							//�������ַ��Ƿ��ս��
							//�ڲ���ʽ�в�����Ϊ�˷��ս���Ĳ���ʽ�������Ŀ��
							for (int pk = 0; pk < n; pk++)
							{
								if (program[pk][0] == dotNextChP) {
									addEffPro(setNP, 1, program[pk]);
								}
							}
						}
					}
				}

				//���������
				checkSetN = proSetN - 1;
			}






				//�����Ŀ����������֮����ַ��ɵ����鵽����Ŀ���ı��
			char passCh = currProgram[2 + dotIndex];//������֮����ַ�
			//�жϸ��ַ����ս�����Ƿ��ս��
			int passChVTI = vtIndex(passCh);
			if (passChVTI != -1) {//���ַ����ս��
				string currResult = "S";
				currResult.append(num2str(checkSetN));
				if (analyseTable[setN][passChVTI] == "-") {
					analyseTable[setN][passChVTI] = currResult;
				}
				else if(analyseTable[setN][passChVTI] != currResult){
					cout << "���ķ�������SLR(1)����������������" << endl;
					exit(0);
				}
			}
			else {//���ַ��Ƿ��ս��
				int passChVNI = vnIndex(passCh);
				string currResult = "";
				currResult.append(num2str(checkSetN));
				if (analyseTable[setN][vtN + passChVNI] == "-")
				{
					analyseTable[setN][vtN + passChVNI] = currResult;
				}
				else if(analyseTable[setN][vtN + passChVNI] != currResult){
					cout << "���ķ�������SLR(1)����������������" << endl;
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


//������Ч��Ŀ���ڵ���Ч��Ŀ��
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
	cout << "ת������ķ�����" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << program[i] << endl;
	}
}


//���ķ��������ҵ��ս���ͷ��ս�������浽�ս������ͷ��ս������
void createVTVN() {
	vnN = 0;
	vtN = 0;
	for (int i = 0; i < n; i++)
	{
		if (vnIndex(program[i][0]) == -1) {
			//���ս��������û�и��ķ�������󲿱��ʽ
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


//��ʾ�����ս����first��
void showFirst() {
	cout << "First����" << endl;
	for (int i = 0; i < vnN; i++)
	{
		cout << VN[i] << ":" << first[i]<<endl;
	}
}


//��ʾ�����ս����follow��
void showFollow() {
	cout << "Follow����" << endl;
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