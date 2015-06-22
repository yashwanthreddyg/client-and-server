#include <winsock2.h>
#include <stdafx.h>
#include <windows.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <conio.h>
typedef unsigned char uchar;
typedef unsigned int uint;
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
	char n_files;
};
//___________________________________________________

void sendDetails(char*);
void getDetails(char*);
void updateMarks(char*);
void sendFile(char*);
//void getFile(char*); 
void getStringFromDetails(char*, s_details*);
void sendQuery(char*);
void getStringFromInt(char*, int);
void getFile(char*);
void processReply(char*);
int len1(char**);
char** getWords(char*);
//___________________________________________________

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
void processReply(char* buffer)
{
	char** toks = getWords(buffer);
	if (!strcmp(toks[1], "details"))
	{
		printf("\nstudent name : ");
		puts(toks[2]);
		printf("\nfather name : ");
		puts(toks[3]);
		printf("\nmother name : ");
		puts(toks[4]);
		printf("\naddress : ");
		puts(toks[5]);
		printf("\nmarks :");
		int k = 7;
		for (int i = 0; i < 2; i++)
		{
			printf("\nyear %d \n",  1 + i);
			for (int j = 0; j < 6; j++)
			{
				printf("%s ", toks[k++]);
			}
		}
		printf("\n");
	}
	if (!strcmp(toks[1], "file"))
	{
		FILE* rec = fopen(toks[2], "w");
		int i = 0;
		while (1)
		{
			if (toks[3][i] == '('&&toks[3][i + 1] == ')')
			{
				break;
			}
			fputc(toks[3][i], rec);
			i++;
		}
		fclose(rec);
	}
}

void getFile(char* buff)
{
	strcat(buff, "get:file:");
	char roll[10];
	char filename[15];
	printf("\nEnter roll : ");
	fflush(stdin);
	gets(roll);
	printf("\nEnter filename : ");
	fflush(stdin);
	gets(filename);
	strcat(buff, roll);
	strcat(buff, ":");
	strcat(buff, filename);
}

void sendFile(char* buff)
{
	printf("\nEnter your roll : ");
	char roll[10];
	char filename[20];
	fflush(stdin);
	gets(roll);
	strcat(buff, "post:file:");
	strcat(buff, roll);
	strcat(buff,":");
	printf("\nenter filename ");
	fflush(stdin);
	gets(filename);
	strcat(buff, filename);
	strcat(buff, ":");

	//___
	FILE* send = fopen(filename, "r");
	char ch;
	char temp[2];
	temp[1] = '\0';
	int i = strlen(buff);
	while (1)
	{
		ch = fgetc(send);
		buff[i] = ch;
		if (ch == EOF)
		{
			buff[i] = '(';
			buff[i + 1] = ')';
			buff[i + 2] = '\0';
			break;
		}
		i++;
	}

	fclose(send);
}

void sendQuery(char* buff)
{
	int choice = 0;
	printf("Choose one of the following:\n");
	printf("1.Save student details\n");
	printf("2.Get student details\n");
	printf("3.Update marks of a student\n");
	printf("4.Send a file as attachment\n");
	printf("5.Get a file from the server\n");
	printf(":");
	scanf("%d", &choice);
	printf("\n");
	switch (choice)
	{
	case 1:
		sendDetails(buff);
		break;
	case 2:
		getDetails(buff);
		break;
	case 3:
		updateMarks(buff);
		break;
	case 4:
		sendFile(buff);
		break;
	case 5:
		getFile(buff);
		break;
	}
}
void getStringFromInt(char* str, int num)
{
	sprintf(str, "%d", num);
}
void updateMarks(char* buff)
{
	char roll[10];
	char* temp = (char*)malloc(4);
	int year;
	int m[6];
	printf("Enter rollnum : ");
	fflush(stdin);
	gets(roll);
	printf("Enter year : ");
	fflush(stdin);
	scanf("%d", &year);
	printf("Enter marks sequestially :\n");
	fflush(stdin);
	scanf("%d %d %d %d %d %d", &m[0], &m[1], &m[2], &m[3], &m[4], &m[5]);
	strcat(buff, "update:marks:");
	strcat(buff, roll);
	strcat(buff, ":");
	temp[0] = (char)('0'+year);
	temp[1] = '\0';
	strcat(buff, temp);
	for (int i = 0; i < 6; i++)
	{
		strcat(buff, ":");
		getStringFromInt(temp, m[i]);
		strcat(buff, temp);
	}

}
void getDetails(char* buff)
{
	printf("\nEnter the rollnumber : ");
	char roll[10];
	fflush(stdin);
	gets(roll);
	strcat(buff, "get:details:");
	strcat(buff, roll);
}
void sendDetails(char* buff)
{
	s_details* sd = (s_details*)malloc(sizeof(s_details));
	printf("\nEnter student name : ");
	fflush(stdin);
	gets(sd->s_name);
	printf("\nEnter student rollNum : ");
	fflush(stdin);
	gets(sd->roll);
	printf("\nEnter student's father's name : ");
	fflush(stdin);
	gets(sd->f_name);
	printf("\nEnter student's mother's name : ");
	fflush(stdin);
	gets(sd->m_name);
	printf("\nEnter student's address : ");
	fflush(stdin);
	gets(sd->addr);
	char* temp = (char*)malloc(100);
	temp[0]='\0';
	getStringFromDetails(temp, sd);
	strcat(buff, "post:details:");
	strcat(buff, temp);
}
void getStringFromDetails(char* temp, s_details* sd)
{
	strcat(temp, sd->roll);
	strcat(temp, ":");
	strcat(temp, sd->s_name);
	strcat(temp, ":");
	strcat(temp, sd->f_name);
	strcat(temp, ":");
	strcat(temp, sd->m_name);
	strcat(temp, ":");
	strcat(temp, sd->addr);
	strcat(temp, ":0:0:0:0:0:0:0:0:0:0:0:0");
}

//_____________________________________________
//CORE CLIENT
int getsocket()
{
	int hsock;
	int * p_int ;
	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n",WSAGetLastError());
		return -1;
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n", WSAGetLastError());
		free(p_int);
		return -1;
	}
	free(p_int);

	return hsock;
}

void socket_client()
{

	//The port and address you want to connect to
	int host_port= 1101;
	char* host_name="127.0.0.1";

	//Initialize socket support WINDOWS ONLY!
	unsigned short wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
 	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 ||
		    HIBYTE( wsaData.wVersion ) != 2 )) {
	    fprintf(stderr, "Could not find sock dll %d\n",WSAGetLastError());
		goto FINISH;
	}

	//Initialize sockets and set any options

	//Connect to the server
	struct sockaddr_in my_addr;

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = inet_addr(host_name);

	//if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR ){
	//	fprintf(stderr, "Error connecting socket %d\n", WSAGetLastError());
	//	goto FINISH;
	//}

	//Now lets do the client related stuff
	char* buffer=(char*)malloc(1024);
	int buffer_len = 1024;
	int bytecount;
	int c;

	while(true) {

		int hsock = getsocket();
		//add error checking on hsock...
		if( connect(hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR ){
			fprintf(stderr, "Error connecting socket %d\n", WSAGetLastError());
			goto FINISH;
		}

		memset(buffer, '\0', buffer_len);
		printf("Enter your message to send here\n");
		//do modfications here
		sendQuery(buffer);

		/*
		for(char* p=buffer ; (c=getch())!=13 ; p++){
			printf("%c", c);
			*p = c;
		}*/

		if( (bytecount=send(hsock, buffer, strlen(buffer),0))==SOCKET_ERROR){
			fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
			goto FINISH;
		}
		printf("Sent bytes %d\n", bytecount);

		if((bytecount = recv(hsock, buffer, buffer_len, 0))==SOCKET_ERROR){
			fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
			goto FINISH;
		}
		//printf("Recieved bytes %d\nReceived string \"%s\"\n", bytecount, buffer);
		printf("%s\n",buffer);
		//Modify here
		processReply(buffer);

		closesocket(hsock);
	}

	//closesocket(hsock);
FINISH:
;
}