#pragma once
#ifndef  CLIENT_H
#define CLIENT_H
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <limits>

#include <stdlib.h>
#include <conio.h>
#include <WS2tcpip.h>
#include <Windows.h>
#pragma comment(lib,"WS2_32.lib")

class Client
{
private:
#define BUF_SIZE 1024
#define SERVER_PORT 798
#define DEFAULT_PORT "798"
#define MSG_BUF_SIZE 1024
public:
	Client();
	~Client();
private:
	WORD winsock_ver;
	WSADATA wsa_data;
	SOCKET sock_clt;
	struct addrinfo  hints,*result;

	int addr_len;
	char buf_ip[BUF_SIZE];

	bool connect_flag;

	
public:
	bool conflag();
	int connection();
	int send(char *content);
	char * get();
	void close();

	int GetPara(char *callPara);
	char *FunSCallProxy(char *CallName,const char *CallPara);
};

#endif //  CLIENT_H
