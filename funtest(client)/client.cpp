#include "client.h"

extern void clearcin();
extern bool isInteger(std::string &word);
extern bool isDouble(std::string &word);

Client::Client()
{ 
	int res;

	this->sock_clt = INVALID_SOCKET;
	this->winsock_ver = MAKEWORD(2,2);
	::WSAStartup(this->winsock_ver,&this->wsa_data);

	memset(&this->hints,0,sizeof(this->hints));
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo("127.0.0.1",DEFAULT_PORT,&this->hints,&this->result); 

	this->sock_clt = socket(this->result->ai_family,this->result->ai_socktype,this->result->ai_protocol);
	res = connect(this->sock_clt,this->result->ai_addr,this->result->ai_addrlen);

	if (res == SOCKET_ERROR)
	{
		this->connect_flag = false;
		printf("connect is faild!!!\n");
	}
	else
	{
		this->connect_flag = true;
		printf("connect is successed.\n");
	}
	freeaddrinfo(this->result);

	//say hi to serve验证服务器
	char buf_msg[MSG_BUF_SIZE];

	memset(buf_msg, 0, MSG_BUF_SIZE);
	::send(this->sock_clt, "hallo serve!", sizeof("hallo serve!"), 0);
	recv(this->sock_clt, buf_msg, MSG_BUF_SIZE, 0);
	if (strcmp(buf_msg, "serve ok!"))
	{
		this->connect_flag = false;
		closesocket(this->sock_clt);
		shutdown(this->sock_clt, SD_SEND);
		printf("server is bad!!!\n");
	}
}

int Client::GetPara(char *callPara)
{
	std::string Para;
	char types;
	Para.clear();

	Para.append("{");
	do {
		printf("please input what kind of parameter you want: ");
		types = getchar();
		clearcin();					//用完输入流就记得擦干净屁股是个好习惯。。。

		std::string line;
		std::string word;
		std::string temp;
		std::stringstream sline;
		std::list<std::string> ssplit;

		switch (types)
		{
		case 's':case'S':
			std::getline(std::cin, line);
			sline.str(line);
			
			while (std::getline(sline, word, ' '))
				ssplit.push_back(word);

			if (ssplit.size() != 0)
				Para.append("\"string\": ");
			if (ssplit.size() == 1)
			{
				Para.append("\"");
				Para.append(ssplit.front());
				ssplit.pop_front();
				Para.append("\"");
				Para.append(",");
			}
			else if (ssplit.size() > 1)
			{
				Para.append("[");
				while (!ssplit.empty())
				{
					Para.append("\"");
					Para.append(ssplit.front());
					ssplit.pop_front();
					Para.append("\"");
					Para.append(",");
				}
				Para.append("]");
				Para.append(",");
			}
			break;
		case 'd':case 'D':
			std::getline(std::cin, line);
			sline.str(line);
			if (!isInteger(line))
			{
				printf("error input form!!!\n");
				break;
			}
			while (std::getline(sline, word, ' '))
				ssplit.push_back(word);

			if (ssplit.size() != 0)
				temp.append("\"int\": ");
			if (ssplit.size() == 1)
			{
				temp.append("\"");
				temp.append(ssplit.front());
				ssplit.pop_front();
				temp.append("\"");
				temp.append(",");
			}
			else if (ssplit.size() > 1)
			{
				temp.append("[");
				while (!ssplit.empty())
				{
					temp.append("\"");
					temp.append(ssplit.front());
					ssplit.pop_front();
					temp.append("\"");
					temp.append(",");
				}
				temp.append("]");
				temp.append(",");
			}
			Para.append(temp);		//最后将此次输入的内容加入到参数中
			break;
		case 'f':case 'F':
			std::getline(std::cin, line);
			sline.str(line);
			if (!isDouble(line))
			{
				printf("error input form!!!\n");
				break;
			}
			while (std::getline(sline, word, ' '))
				ssplit.push_back(word);

			if (ssplit.size() != 0)
				Para.append("\"double\": ");
			if (ssplit.size() == 1)
			{
				
				Para.append("\"");
				Para.append(ssplit.front());
				ssplit.pop_front();
				Para.append("\"");
				Para.append(",");
			}
			else if (ssplit.size() > 1)
			{
				Para.append("[");
				while (!ssplit.empty())
				{
					Para.append("\"");
					Para.append(ssplit.front());
					ssplit.pop_front();
					Para.append("\"");
					Para.append(",");
				}
				Para.append("]");
				Para.append(",");
			}
			break;
		case 'q':case 'Q':
			break;
		default:
			printf("sorry,there is don't have type '%c',please inpute again!!\n",types); 
			break;
		}
	} while (!(types == 'q' || types == 'Q'));

	Para.append("}");
	strcpy_s(callPara,MSG_BUF_SIZE,Para.c_str());
	return 1;
}

bool Client::conflag()
{
	return this->connect_flag;
}

int Client::send(char *content)
{
//	printf("按下s键发送消息\n");

	return ::send(this->sock_clt,content,strlen(content),0) ;
}

char * Client::get()
{
	char getbuf[MSG_BUF_SIZE];
	int re = ::recv(this->sock_clt, getbuf, MSG_BUF_SIZE, 0);
	return re==0?nullptr:getbuf;
}

//名称：FunSCallProxy
//功能：向服务器发送需要调用的函数和参数
//返回值：服务器返回的参数结构体字符串
//注意：第一次服务器返回的值
//         “status: 1”代表成功查找到调用函数
//			“status: 0” 代表查找被调用函数失败
char * Client::FunSCallProxy(char * CallName,const char * CallPara)
{
	char msg[MSG_BUF_SIZE];
	size_t msg_len;
	int res;

	if (!this->conflag())
		return nullptr;

	msg_len = strlen(CallName);
	res = ::send(this->sock_clt, CallName,strlen(CallName), 0);
	//res = ::recv(this->sock_clt, msg, MSG_BUF_SIZE, 0);
	if (res <= 0)
	{
		printf("Disconnect from the server!!!\n");
		return nullptr;
	}
	
	res = ::send(this->sock_clt, CallPara, strlen(CallPara), 0);
	//res = ::recv(this->sock_clt, msg, MSG_BUF_SIZE, 0);
	if (res <= 0)
	{
		printf("Disconnect from the server!!!\n");
		return nullptr;
	}

	printf("%s 's return is: %s  \n",CallName,msg);
	return msg;
}

Client::~Client()
{
	closesocket(this->sock_clt);
	shutdown(this->sock_clt, SD_SEND);
}