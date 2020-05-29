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
		c1.GetPara(CallPara);	//将获取的参数安全放入参数数组中

		if (c1.FunSCallProxy(CallName,CallPara) == nullptr)
		{
			printf("Receive failed!!!\n");
			break;
		}
	}
	return 0;
}

//用来清空输入流
void clearcin()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
	//std::cin.sync();
	std::cin.clear();
}

//功能：如果字符串内容能代表一个整形数字则返回T
bool isInteger(std::string &word)
{
	if (word.find_first_not_of("01234567890") != std::string::npos)
		return false;
	else
		return true;
}

//功能：如果字符串内容能代表一个浮点数则返回T
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