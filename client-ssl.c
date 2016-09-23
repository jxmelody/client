/*client*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <errno.h>
#include <unistd.h>
#include<string.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "cJSON.h"
#define BUFFLEN 1024
#define SERVER_PORT 30000

void ShowCerts(SSL * ssl)
{
	X509 *cert;
	char *line;
	cert = SSL_get_peer_certificate(ssl);
	if (cert != NULL) {
		printf("数字证书信息:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("证书: %s\n", line);
		free(line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("颁发者: %s\n", line);
		free(line);
		X509_free(cert);
	} else
	printf("无证书信息!\n");
}

char *  makeJson()
{
	cJSON * pJsonRoot = NULL;
	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		//error happend here
		return NULL;
	}
	 // cJSON_AddStringToObject(pJsonRoot, "sign","viewlog");
	// cJSON_AddStringToObject(pJsonRoot, "sign","createcode");
	cJSON_AddStringToObject(pJsonRoot, "sign","login");
	//cJSON_AddStringToObject(pJsonRoot, "sign","opendoor");
	//cJSON_AddStringToObject(pJsonRoot, "sign","reg");
	cJSON_AddStringToObject(pJsonRoot, "username","youfei");
	cJSON_AddStringToObject(pJsonRoot, "password","123456");
	  cJSON_AddStringToObject(pJsonRoot, "reg_code","BMVFIAHUSNAKYOSI");
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
	int s,len;
	char *j;
	struct sockaddr_in server;
	char buffer[BUFFLEN+1];
	SSL_CTX *ctx;
	SSL *ssl;
	int n = 0;
	
	/* SSL 库初始化, */
	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (ctx == NULL) {
		ERR_print_errors_fp(stdout);
		exit(1);
	}
	/* 创建一个 socket 用于 tcp 通信 */
	if((s=socket(AF_INET,SOCK_STREAM,0))<0)
	{
			perror("Socket");
			exit(errno);
	}
	printf("socket created\n");
	/* 初始化服务器端(对方)的地址和端口信息 */
	bzero(&server, sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	//server.sin_addr.s_addr=htonl(INADDR_ANY);
	server.sin_port=htons(SERVER_PORT);
	
	if(connect(s,(struct sockaddr*)&server,sizeof(server))!=0)
	{
		perror("Connect ");
		exit(errno);
	}
	
	/* 基于 ctx 产生一个新的 SSL */
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, s);
	/* 建立 SSL 连接 */
	if (SSL_connect(ssl) == -1)
		ERR_print_errors_fp(stderr);
	else {
		printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
		ShowCerts(ssl);
	}
	
	memset(buffer,0,BUFFLEN);
	j=makeJson();
	
	strcpy(buffer,j);
	printf("%s",buffer);
	SSL_write(ssl,buffer,strlen(buffer));
	memset(buffer,0,BUFFLEN);
	n=SSL_read(ssl,buffer,BUFFLEN);
	
	if (n>0)
	{
		printf("%s",buffer);
	}
	finish:
	/* 关闭连接 */
	SSL_shutdown(ssl);
	SSL_free(ssl);
	close(s);
	SSL_CTX_free(ctx);
	return 0;
}
	


