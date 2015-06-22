// socket_client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

void socket_client();
//void sendFile(char* buff);

void sendFileExp(char* buff)
{
	FILE* fp = fopen("sending.txt","r");
	char ch;
	char temp[2];
	temp[1] = '\0';
	int i = 0;
	while (1)
	{
		ch = fgetc(fp);
		buff[i] = ch;
		if (ch==EOF)
		{
			buff[i] = '(';
			buff[i + 1] = ')';
			buff[i + 2] = '\0';
			break;
		}
		i++;
	}
	fclose(fp);
}
void receivefileExp(char*buff)
{
	FILE* fp = fopen("receiving.txt", "w");
	int i = 0;
	while (1)
	{
		if (buff[i] == '('&&buff[i + 1] == ')')
		{
			break;
		}
		fputc(buff[i], fp);
		i++;
	}
	fclose(fp);
}
int _tmain(int argc, _TCHAR* argv[])
{
	char buff[1024];
	buff[0] = '\0';
	socket_client();
	//sendFileExp(buff);
	//receivefileExp(buff);
	return 0;
}

