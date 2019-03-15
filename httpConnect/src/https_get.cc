#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
// #include "../include/apue.h"
// #include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <sstream>
#include <vector>


// keep track of the handle
typedef struct{
    int socket;
    SSL *sslHandle;
    SSL_CTX *sslContext;
}connection;

// define proto.h
#define SERVER "129.28.84.211"
#define PORT 443

// to establish a regular tcp connection
int tcpConnect(){
    int error,handle;
    struct hostent *host;
    struct sockaddr_in server;

    host = gethostbyname(SERVER);
    handle = socket(AF_INET,SOCK_STREAM,0);
    if(handle==-1){
        perror("Socket wrong");
        handle = 0;
    }else{
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr = *((struct in_addr *)host->h_addr);
        // bzero(&(server.sin_zero),8);
        // 指定ip
        //inet_pton(AF_INET,SERVER,&server.sin_addr);
        error = connect(handle,(struct sockaddr*)&server,sizeof(struct sockaddr));

        if(error == -1){
            perror("Connect error");
            handle = 0;
        }
    }
    printf("tcpConnect successs\n");
    return handle;
}

// Establish a connection using an SSL layer
connection *sslConnect(void){
    connection *c;
    c = (connection *)malloc(sizeof(connection));
    c->sslHandle = NULL;
    c->sslContext = NULL;

    c->socket = tcpConnect();

    if(c->socket){ // TCP握手先链接
    // Register the error strings for libcrypto & libssl
    SSL_load_error_strings();

    // Register the available ciphers and digests
    SSL_library_init();
    // 准备一套算法
   // OpenSSL_add_all_algorithms();

    // New context saying we are client using SSL2 or 3
    c->sslContext = SSL_CTX_new(SSLv23_client_method());
    if(c->sslContext == NULL)
        ERR_print_errors_fp(stderr);

    // Create an SSL struct for connection

    c->sslHandle = SSL_new(c->sslContext);

    if(c->sslHandle == NULL)
        ERR_print_errors_fp(stderr);

    // Connect the SSL struct to our connection
    if(!SSL_set_fd(c->sslHandle,c->socket))
        ERR_print_errors_fp(stderr);
    // Initiate SSL handshake

    if(SSL_connect(c->sslHandle)!=1) // 这里不能漏
        ERR_print_errors_fp(stderr);
    }else{
        perror("Connect failed");
    }
    printf("SSLConnect successs\n");
    return c;
}

// Disconnect & free connection struct
void sslDisconnect(connection *c){
    if(c->socket)
        close(c->socket);
    if(c->sslHandle){
        SSL_shutdown(c->sslHandle);
        SSL_free(c->sslHandle);
    }
    if(c->sslContext)
        SSL_CTX_free(c->sslContext);
    
    free(c);
}

// Read all available text from the connection
void sslRead(connection *c){
    const int readSize = 1024;
    char *rc = NULL;
    int received,count = 0;
    char buffer[1024];
    std::stringstream s_res;
    if(c){
        while(1){
            
            if(!rc)
                rc = (char *)malloc(readSize*sizeof(char)+1);
            else
            {
                rc = (char *)realloc(rc,(count+1)*readSize*sizeof(char)+1);
            }
            memset(rc,0,readSize+1);
            received = SSL_read(c->sslHandle,buffer,readSize);
            printf("%d\n",received);
            buffer[received]='\0';
            if(received>0)
                strcat(rc,buffer);
            
            s_res<<rc;
            if(received<readSize)
                break;
            count++;
        }
    }   
        free(rc);
        std::string str_res = s_res.str();
        const char *res = str_res.c_str();
        printf("%s\n",res);
        
    // return rc;
}

// Write text to the connection
void sslWrite(connection *c,const char *text){
    int ret;
    if(c) ret=SSL_write(c->sslHandle,text,strlen(text));
    if(ret==-1){
        printf("SSL write error\n");
    }
}

// send GET.and print(text);
int main(int argc,char **argv){
    connection *c;
    char *response;

    c=sslConnect();
    std::string strUrl = "/music/tencent/search?key=579621905&s=123&limit=10&offset=0&type=song";
    // std::string host = "www.baidu.com";
    std::string host = "api.bzqll.com";
	std::stringstream stream;
	stream << "GET "<<strUrl<<" HTTP/1.1\r\n";
	stream << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n";
	//stream << "Accept-Encoding: gzip, deflate, br\r\n";//不要编码，否则还得多一个解码的步骤
	stream << "Accept-Language: zh-CN,zh;q=0.9\r\n";
	stream << "Connection: Keep-Alive\r\n";
	stream << "Host: " << host << "\r\n";
    // stream << "Referer:https://www.bzqll.com/2019/01/262.html\r\n";
	stream << "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36\r\n";
	stream << "\r\n";
    std::string ss = stream.str();
    const char *sendData = ss.c_str();
        // printf("%s\n",sendData);
    sslWrite(c,sendData);
    sslRead(c);
    // printf("%s\n",response);
    sslDisconnect(c);
    free(response);
    return 0;
}
