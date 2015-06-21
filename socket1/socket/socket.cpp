// socket.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
typedef unsigned char uchar;
typedef unsigned int uint;
void socket_server();
void init();
FILE* fp;
/*struct s_details
{
	char s_name[15];
	char f_name[15];
	char m_name[15];
	char addr[40];
	uchar marks_y1[6];
	uchar marks_y2[6];
	uchar n_files;
	char mainfile[20];
	char roll[10];
};
struct pair
{
	char roll[10];
	uint offset;
};
struct meta
{
	int n_st;
	pair s_blocks[7000];
};*/
int _tmain(int argc, _TCHAR* argv[])
{
	fp = fopen("database.bin", "rb+");
	init();
	//socket_server();
	scanf("%d");
}

