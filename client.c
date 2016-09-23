/*client*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>

#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#define BUFFLEN 1024
#define SERVER_PORT 30000
char *  makeJson()
{
	cJSON * pJsonRoot = NULL;
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		//error happend here
		return NULL;
	}
	//   cJSON_AddStringToObject(pJsonRoot, "sign","viewlog");
	//cJSON_AddStringToObject(pJsonRoot, "sign","createcode");
	//  cJSON_AddStringToObject(pJsonRoot, "sign","login");
	cJSON_AddStringToObject(pJsonRoot, "sign","reg");
	cJSON_AddStringToObject(pJsonRoot, "username","youfei");
	cJSON_AddStringToObject(pJsonRoot, "password","123456");
	cJSON_AddStringToObject(pJsonRoot, "reg_code","SSFUIJZQURKNQRWU");
	char * p = cJSON_Print(pJsonRoot);
	if(NULL == p)
	{
		cJSON_Delete(pJsonRoot);
		return NULL;
	}
	cJSON_Delete(pJsonRoot);
	return p;
	
}

int main()
{
	int s;
	char *j;
	struct sockaddr_in server;
	char buffer[BUFFLEN];
	int n = 0;
	s=socket(AF_INET,SOCK_STREAM,0);
	
	memset(&server,0,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	//server.sin_addr.s_addr=htonl(INADDR_ANY);
	server.sin_port=htons(SERVER_PORT);
	
	connect(s,(struct sockaddr*)&server,sizeof(server));
	memset(buffer,0,BUFFLEN);
	
	j=makeJson();
	/*
	int i;
	for(i=0;i<=BUFFELEN;i++)
	{
		buffer[i]=*(j+i);
	}
	*/
	strcpy(buffer,j);
	
	
	send(s,buffer,strlen(buffer),0);
	memset(buffer,0,BUFFLEN);
	n=recv(s,buffer,BUFFLEN,0);
	
	if (n>0)
	{
		printf("%s",buffer);
	}
	close(s);
	return 0;
}
