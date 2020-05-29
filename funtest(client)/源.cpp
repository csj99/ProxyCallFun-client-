#include <iostream>
#include <thread>
#include "client.h"

void clearcin();

int main()
{
	Client c1;

	while (c1.conflag())
	{
		char CallName[MSG_BUF_SIZE];
		char CallPara[MSG_BUF_SIZE];
		printf("Please input the name of called function:  ");
		scanf_s("%s", CallName,MSG_BUF_SIZE);
		clearcin();
		c1.GetPara(CallPara);	//����ȡ�Ĳ�����ȫ�������������

		if (c1.FunSCallProxy(CallName,CallPara) == nullptr)
		{
			printf("Receive failed!!!\n");
			break;
		}
	}
	return 0;
}

//�������������
void clearcin()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
	//std::cin.sync();
	std::cin.clear();
}

//���ܣ�����ַ��������ܴ���һ�����������򷵻�T
bool isInteger(std::string &word)
{
	if (word.find_first_not_of("01234567890") != std::string::npos)
		return false;
	else
		return true;
}

//���ܣ�����ַ��������ܴ���һ���������򷵻�T
bool isDouble(std::string &word)
{
	size_t fpos;
	if (word.find_first_not_of("01234567890.") == std::string::npos)
	{
		fpos = word.find(".");
		if (fpos == 0 || fpos == word.size())
			return false;
		else if (fpos != word.rfind("."))
			return false;
		else
			return true;
	}
	else
		return false;
}