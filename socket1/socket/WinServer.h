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
int meta_off = 0;
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
	char marks_y1[6];
	char marks_y2[6];
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
uint getNextFreeLoc(int);
void addStudentDetailsAtOffset(int, s_details*); 
void updateMarks(char*, int*, int);
int getStudentOffset(char*); 
void getStudentDetails(char*, s_details*); 
void addNewStudent(s_details* );
void init();
void parseInput(char*, char*);
char** getWords(char*); 
int len1(char**);
void parsePost(char**, char*);
void parseGet(char**, char*);
void parseUpdate(char**, char*); 
void getStringFromDetails(char*, s_details*);
void getStringFromInt(char* , int );

//END OF FUNCTION DEFS
//______________________________________
//ALLOCATION
void getStringFromInt(char* str, int num)
{
	sprintf(str, "%d", num);
}

void parsePost(char** input, char* output)
{
	if (!strcmp(input[1], "details"))
	{
		s_details* sd = (s_details*)malloc(sizeof(s_details));
		strcpy(sd->roll, input[2]);
		strcpy(sd->s_name, input[3]);
		strcpy(sd->f_name, input[4]);
		strcpy(sd->m_name, input[5]);
		strcpy(sd->addr, input[6]);
		addNewStudent(sd);
		strcpy(output, "new student details added\n");
	}
	else if (!strcmp(input[1], "file"))
	{
		char roll[10];
		char filepath[30] ;
		filepath[0] = '\0';
		strcat(filepath, input[2]);
		strcat(filepath, "/");
		strcat(filepath, input[3]);
		FILE* rec  = fopen(filepath, "w");
		int i = 0;
		while (1)
		{
			if (input[4][i] == '('&&input[4][i + 1] == ')')
			{
				break;
			}
			fputc(input[4][i], rec);
			i++;
		}
		fclose(rec);
	}
}

void getStringFromDetails(char* temp, s_details* sd)
{
	char n[4];
	strcat(temp,":");
	strcat(temp, sd->roll);
	strcat(temp, ":");
	strcat(temp, sd->s_name);
	strcat(temp, ":");
	strcat(temp, sd->f_name);
	strcat(temp, ":");
	strcat(temp, sd->m_name);
	strcat(temp, ":");
	strcat(temp, sd->addr);
	for (int i = 0; i < 6; i++)
	{
		strcat(temp, ":");
		int k = (int)sd->marks_y1[i];
		getStringFromInt(n, (int)sd->marks_y1[i]);
		strcat(temp, n);
	}
	for (int i = 0; i < 6; i++)
	{
		strcat(temp, ":");
		int k = (int)sd->marks_y2[i];
		getStringFromInt(n, sd->marks_y2[i]);
		strcat(temp, n);
	}
}
void parseGet(char** input, char* output)
{
	if (!strcmp(input[1], "details"))
	{
		char temp[100];
		temp[0] = '\0';
		s_details* sd = (s_details*)malloc(sizeof(s_details));
		getStudentDetails(input[2],sd);
		strcpy(output, "post:details");
		getStringFromDetails(temp, sd);
		strcat(output, temp);
	}
	if (!strcmp(input[1], "file"))
	{
		char roll[10];
		char filepath[30];
		filepath[0] = '\0';
		strcat(filepath, input[2]);
		strcat(filepath, "/");
		strcat(filepath, input[3]);
		strcat(output, "post:file:");
		strcat(output, input[3]);
		strcat(output, ":");
		//______
		FILE* send = fopen(filepath, "r");
		char ch;
		char temp[2];
		temp[1] = '\0';
		int i = strlen(output);
		while (1)
		{
			ch = fgetc(send);
			output[i] = ch;
			if (ch == EOF)
			{
				output[i] = '(';
				output[i + 1] = ')';
				output[i + 2] = '\0';
				break;
			}
			i++;
		}

		fclose(send);
	}

}

void parseUpdate(char** input, char* output)
{
	if (!strcmp(input[1], "marks"))
	{
		int year = atoi(input[3]);
		s_details* sd = (s_details*)malloc(sizeof(s_details));
		getStudentDetails(input[2],sd);
		char* arr = (year == 1) ? sd->marks_y1 : sd->marks_y2;
		for (int i = 0; i < 6; i++)
		{
			arr[i] =(int) atoi(input[4 + i]);
		}
		addStudentDetailsAtOffset(getStudentOffset(sd->roll), sd);
		strcat(output, "student marks have been updated\n");
	}

}
int len1(char** arr)
{
	int i = 0;
	while (arr[i][0] != '$')
		i++;
	return i;
}

char** getWords(char* str)
{
	fflush(stdout);
	int n_tok = 0, i = 0;
	char** words;
	char* w;
	for (i = 0; i < strlen(str); i++)
	{
		if (str[i] == ':')
			n_tok++;
	}
	n_tok++;
	words = (char**)malloc((n_tok + 1)*sizeof(char*));
	w = strtok(str, ":");
	i = 0;
	words[i++] = w;
	while (i < n_tok)
	{
		w = strtok(NULL, ":");
		words[i++] = w;
	}

	words[i] = (char*)malloc(sizeof(char));
	words[i][0] = '$';
	return words;
}
void parseInput(char* input, char* output)
{
	char** tokens = getWords(input);
	if (!strcmp(tokens[0], "post"))
	{
		parsePost(tokens, output);
	}
	else if (!strcmp(tokens[0], "get"))
	{
		parseGet(tokens, output);
	}
	else if (!strcmp(tokens[0], "update"))
	{
		parseUpdate(tokens, output);
	}
}

void createFolder(char* name)
{
	wchar_t wtext[20];
	mbstowcs(wtext, name, strlen(name) + 1);//Plus null
	LPWSTR ptr = wtext;
	CreateDirectory(ptr, NULL);
}

uint getNextFreeLoc(int n_blocks)
{
	/*
	unsigned char b;
	int bytepos = 0;
	int bitpos = 0;
	uchar req = (n_blocks == 1) ? 0x80 : 0x40;
	rewind(fp);
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
	fseek(fp, bytepos , SEEK_SET);
	b = b | sample(bitpos);
	if (n_blocks == 2)
	{
		b = b | sample(bitpos + 1);
	}
	fwrite(&b, 1, 1, fp);
	return bytepos * 8 * 128 + bitpos * 128 + 1000+sizeof(meta);*/
	meta* m = (meta*)malloc(sizeof(meta));
	rewind(fp);
	fread(m, sizeof(meta), 1, fp);
	int t = m->n_st;
	free(m);
	return t*sizeof(s_details)+sizeof(meta);
}

//END OF ALLOCATION
//______________________________________
void init(){
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
	rewind(fp);
	fwrite(m, sizeof(meta), 1, fp);
	createFolder(sd->roll);
	addStudentDetailsAtOffset(off,sd);
	free(m);
}
void getStudentDetails(char* roll,s_details* sd)
{
	meta* m = (meta*)malloc(sizeof(meta));
	rewind(fp);
	fread(m, sizeof(meta), 1, fp);
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
	rewind(fp);
	fread(m, sizeof(meta), 1, fp);
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
	char* arr;
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
	char* temp;
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
	/*
	printf("%s",recvbuf);
	replyto_client(recvbuf, csock);
	replybuf[0] = '\0';
	*/
	parseInput(recvbuf,replybuf);
	replyto_client(replybuf, csock);

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

	char recvbuf[1024] = {'\0'};
	int recvbuf_len = 1024;
	int recv_byte_cnt;

	memset(recvbuf, 0, recvbuf_len);
	if((recv_byte_cnt = recv(*csock, recvbuf, recvbuf_len, 0))==SOCKET_ERROR){
		fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
		free (csock);
		return 0;
	}

	printf("Received bytes %d\nReceived string \"%s\"\n", recv_byte_cnt, recvbuf);
	process_input(recvbuf, recv_byte_cnt, csock);

    return 0;
}