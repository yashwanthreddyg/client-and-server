//#pragma once
#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

typedef unsigned int uint;
typedef unsigned char uchar;

//__________________________________
//GLOBALS

extern FILE* fp;
int meta_off = 1024;
//END OF GLOBALS
//__________________________________

//STRUCTURES

struct s_details
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
struct pair
{
	char roll[10];
	uint det_off;
};
struct meta
{
	int n_st;
	pair s_blocks[7000];
};

//END OF STRUCTURES
//______________________________________
//FUNCTION DEFS

void createFolder(char*);
uchar sample(int);
uint getNextFreeLoc(int);
void addStudentDetailsAtOffset(int, s_details*); 
void updateMarks(char*, int*, int);
int getStudentOffset(char*); 
void getStudentDetails(char*, s_details*); 
void addNewStudent(s_details* );
void init();

//END OF FUNCTION DEFS
//______________________________________
//ALLOCATION

void createFolder(char* name)
{
	wchar_t wtext[20];
	mbstowcs(wtext, name, strlen(name) + 1);//Plus null
	LPWSTR ptr = wtext;
	CreateDirectory(ptr, NULL);
}
uchar sample(int pos)
{
	switch (pos)
	{
	case 0:
		return (uchar)0x80;
		break;
	case 1:
		return (uchar)0x40;
		break;
	case 2:
		return (uchar)0x20;
		break;
	case 3:
		return (uchar)0x10;
		break;
	case 4:
		return (uchar)0x08;
		break;
	case 5:
		return (uchar)0x04;
		break;
	case 6:
		return (uchar)0x02;
		break;
	case 7:
		return (uchar)0x01;
		break;
	default:
		return (uchar)-1;
		break;
	}
}

uint getNextFreeLoc(int n_blocks)
{
	unsigned char b;
	int bytepos = 0;
	int bitpos = 0;
	uchar req = (n_blocks == 1) ? 0x80 : 0x40;
	fread(&b, 1, 1, fp);
	while (1)
	{
		int flag = 0;
		while (b == (uchar)0xFF)
		{
			bytepos++;
			fread(&b, 1, 1, fp);
		}
		uchar temp = b;
		for (int k = 0; k < 8; k++)
		{
			if (temp >= req)
			{
				temp = temp << 1;
				temp = temp | sample(7);
				bitpos++;
			}
			else
			{
				flag = 1;
				break;
			}
		}
		if (flag)
			break;
	}
	fseek(fp, bytepos + 2, SEEK_SET);
	b = b | sample(bitpos);
	if (n_blocks == 2)
	{
		b = b | sample(bitpos + 1);
	}
	fwrite(&b, 1, 1, fp);
	return bytepos * 8 * 128 + bitpos * 128 + 1000+sizeof(meta);
}

//END OF ALLOCATION
//______________________________________
void init(){

	fseek(fp, 1024, 0);
	meta* m = (meta*)malloc(sizeof(meta));
	fread(m, sizeof(meta), 1, fp);
	m->n_st = 0;
	/*int* m1 = (int*)malloc(6 * 4);
	for (int i = 0; i < 6; i++)
		m1[i] = 21;
	s_details* sd = (s_details*)malloc(sizeof(s_details));
	s_details* sd1 = (s_details*)malloc(sizeof(s_details));
	strcpy(sd->f_name, "fnameyo");
	strcpy(sd->m_name, "mnamemama");
	strcpy(sd->s_name, "snamelol");
	strcpy(sd->addr, "addrkirr");
	strcpy(sd->mainfile, "mainfilhahae");
	strcpy(sd->roll, "rollerer");
	memcpy(sd->marks_y1, m1, 6 * 4);
	memcpy(sd->marks_y2, m1, 6 * 4);
	addNewStudent(sd);
	getStudentDetails(sd->roll,sd1);
	printf("%s", sd1->f_name);*/
}
void addStudentDetailsAtOffset(int offset, s_details* sd)
{
	fseek(fp, offset, SEEK_SET);
	fwrite(sd, sizeof(s_details), 1, fp);
}
void addNewStudent(s_details* sd)
{
	int off = getNextFreeLoc(1);
	meta* m = (meta*)malloc(sizeof(meta));
	fseek(fp, meta_off, SEEK_SET);
	fread(m, sizeof(meta), 1, fp);
	m->s_blocks[m->n_st].det_off = off;
	strcpy(m->s_blocks[m->n_st].roll, sd->roll);
	m->n_st++;
	fwrite(m, sizeof(meta), 1, fp);
	createFolder(sd->roll);
	addStudentDetailsAtOffset(off,sd);
	free(m);
}
void getStudentDetails(char* roll,s_details* sd)
{
	meta* m = (meta*)malloc(sizeof(meta));
	fseek(fp, meta_off, SEEK_SET);
	int i;
	for (i = 0; i < m->n_st; i++)
	{
		if (!strcmp(m->s_blocks[i].roll, roll))
			break;
	}
	fseek(fp, m->s_blocks[i].det_off, SEEK_SET);
	fread(sd, sizeof(s_details),1, fp);
}
int getStudentOffset(char* roll)
{
	meta* m = (meta*)malloc(sizeof(meta));
	fseek(fp, meta_off, SEEK_SET);
	for (int i = 0; i < m->n_st; i++)
	{
		if (!strcmp(m->s_blocks[i].roll, roll))
			return m->s_blocks[i].det_off;
	}
	return -1;
}
void updateMarks(char* roll, int* newmarks, int year)
{
	s_details* temp = (s_details*)malloc(sizeof(s_details));
	getStudentDetails(roll,temp);
	uchar* arr;
	if (year == 1)
		arr = temp->marks_y1;
	else
		arr = temp->marks_y2;
	for (int i = 0; i < 6; i++)
		arr[i] = newmarks[i];
	addStudentDetailsAtOffset(getStudentOffset(roll), temp);
}

void getMarks(char* roll, int* marks,int year)
{
	s_details* sd = (s_details*)malloc(sizeof(s_details));
	getStudentDetails(roll, sd);
	uchar* temp;
	if (year == 1)
		temp = sd->marks_y1;
	else
		temp = sd->marks_y2;
	for (int i = 0; i < 6; i++)
		marks[i] = temp[i];
}

//______________________________________

//CORE SERVER
bool flag=true;
int mid = 0;
int count1 =0;
char *Data[100];
int count=1;
int values[100];
DWORD WINAPI SocketHandler(void*);
void replyto_client(char *buf, int *csock);

void socket_server() {

	//The port you want the server to listen on
	int host_port= 1101;

	unsigned short wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
 	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 ||
		    HIBYTE( wsaData.wVersion ) != 2 )) {
	    fprintf(stderr, "No sock dll %d\n",WSAGetLastError());
		goto FINISH;
	}

	//Initialize sockets and set options
	int hsock;
	int * p_int ;
	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n",WSAGetLastError());
		goto FINISH;
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n", WSAGetLastError());
		free(p_int);
		goto FINISH;
	}
	free(p_int);

	//Bind and listen
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY ;
	
	/* if you get error in bind 
	make sure nothing else is listening on that port */
	if( bind( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
		fprintf(stderr,"Error binding to socket %d\n",WSAGetLastError());
		goto FINISH;
	}
	if(listen( hsock, 10) == -1 ){
		fprintf(stderr, "Error listening %d\n",WSAGetLastError());
		goto FINISH;
	}
	
	//Now lets do the actual server stuff

	int* csock;
	sockaddr_in sadr;
	int	addr_size = sizeof(SOCKADDR);
	
	while(true){
		printf("waiting for a connection\n");
		csock = (int*)malloc(sizeof(int));
		
		if((*csock = accept( hsock, (SOCKADDR*)&sadr, &addr_size))!= INVALID_SOCKET ){
			//printf("Received connection from %s",inet_ntoa(sadr.sin_addr));
			CreateThread(0,0,&SocketHandler, (void*)csock , 0,0);
		}
		else{
			fprintf(stderr, "Error accepting %d\n",WSAGetLastError());
		}
	}

FINISH:
;
}


void process_input(char *recvbuf, int recv_buf_cnt, int* csock) 
{

	char replybuf[1024]={'\0'};
	printf("%s",recvbuf);
	replyto_client(recvbuf, csock);
	replybuf[0] = '\0';
}

void replyto_client(char *buf, int *csock) {
	int bytecount;
	
	if((bytecount = send(*csock, buf, strlen(buf), 0))==SOCKET_ERROR){
		fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
		free (csock);
	}
	printf("replied to client: %s\n",buf);
}

DWORD WINAPI SocketHandler(void* lp){
    int *csock = (int*)lp;

	char recvbuf[1024];
	int recvbuf_len = 1024;
	int recv_byte_cnt;

	memset(recvbuf, 0, recvbuf_len);
	if((recv_byte_cnt = recv(*csock, recvbuf, recvbuf_len, 0))==SOCKET_ERROR){
		fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
		free (csock);
		return 0;
	}

	//printf("Received bytes %d\nReceived string \"%s\"\n", recv_byte_cnt, recvbuf);
	process_input(recvbuf, recv_byte_cnt, csock);

    return 0;
}