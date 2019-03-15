#ifndef MUSIC_SEARCH_HTTPCONNECT_INCLUDE_TCP_CONNECT_H_
#define MUSIC_SEARCH_HTTPCONNECT_INCLUDE_TCP_CONNECT_H_
#include "apue.h"
#include "proto.h"

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>


namespace TCPConnect_{
class TcpConnect{
    public:
        TcpConnect():socket_fd(0){}
        int socket_connect(const char* hostname){
            host = gethostbyname(hostname);
            socket_fd = socket(AF_INET,SOCK_STREAM,0); // TCP连接
            if(socket_fd == -1){
                return socket_fd; // 无法建立链接
            }
            remo_server.sin_family = AF_INET;
            remo_server.sin_port = htons(PORT); // 绑定端口
            remo_server.sin_addr = *((struct in_addr*)host->h_addr_list[0]); // 绑定ip
            error = connect(socket_fd,(struct sockaddr*)&remo_server,sizeof(struct sockaddr)); // 建立链接

            if(error == -1){ // 连接失败
                return socket_fd;
            }
            return socket_fd; // 返回文件描述符
        }
    private:
        int error;
        int socket_fd; // 获取socketfd文件描述符
        struct hostent *host;
        struct sockaddr_in remo_server;
};
}

#endif // !MUSIC_SEARCH_HTTPCONNECT_INCLUDE_TCP_CONNECT_H
