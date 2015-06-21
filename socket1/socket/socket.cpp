// socket.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<malloc.h>
#include<direct.h>
#include<string.h>
#include<Windows.h>

typedef unsigned char uchar;
typedef unsigned int uint;
void socket_server();
uint getNextFreeLoc(int);
void init();
FILE* fp;
struct s_details1
{
	char s_name[15];
	char f_name[15];
	char m_name[15];
	char addr[40];
	char mainfile[20];
	char roll[10];
	uchar marks_y1[6];
	uchar marks_y2[6];
	uchar n_files;
};
/*struct pair
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
	
	printf("%d,", sizeof(s_details1));
	fp = fopen("database.bin", "rb+");
	init();

	//socket_server();
	scanf("%d");
}

