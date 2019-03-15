#ifndef MUSIC_SEARCH_HTTPCONNECT_INCLUDE_HTTPSCONNECT_H_
#define MUSIC_SEARCH_HTTPCONNECT_INCLUDE_HTTPSCONNECT_H_

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <memory>
#include <vector>
#include "tcpConnect.h"
#include <sstream>
#include <string>
#include <regex>

namespace HTTPSConnect{
    typedef struct {
            int socket_fd;
            SSL *sslHandle;
            SSL_CTX *sslContext;
        }connection;

    class HTTPSConnect{
        public:
            HTTPSConnect():tcp_conn(new TCPConnect_::TcpConnect()),http_conn(new connection()){}
            void sslConnect(const char*); // 建立链接
            void sslDisconnect(std::shared_ptr<connection>&) const; // 断开链接
            std::string sslRead(const std::shared_ptr<connection>&) const; // 读数据流
            void sslWrite(const std::shared_ptr<connection>&,const char *) const; // 写数据流
            // socket 是一个文件，可以直接写入
            std::string trans_get(std::string&);
            std::string trans_post(std::string&);
        private:
            std::shared_ptr<TCPConnect_::TcpConnect> tcp_conn;
            std::shared_ptr<connection> http_conn;
    };

    void HTTPSConnect::sslConnect(const char* hostname){
        // connection *conn;
        // conn = (connection*)malloc(sizeof(connection));
        http_conn->sslHandle = nullptr;
        http_conn->sslContext = nullptr;
        http_conn->socket_fd = tcp_conn->socket_connect(hostname);
        if(http_conn->socket_fd>0){ // 先tcp链接一次
            // ssl链接 Register the error strings for libcrypto & libssl
            SSL_load_error_strings();
            // Register the available ciphers and digests
            SSL_library_init();
            // 准备一套方法 加密用
            http_conn->sslContext = SSL_CTX_new(SSLv23_client_method());

            // 是否要求校验对方证书 此处不验证客户端身份所以为： SSL_VERIFY_NONE
            // SSL_CTX_set_verify(m_ctx, SSL_VERIFY_NONE, NULL); // 因为是客户端，也不验证发送

            if(http_conn->sslContext == nullptr)
                ERR_print_errors_fp(stderr); // 标准错误输出
                
            // Create SSL struct for connection
            http_conn->sslHandle = SSL_new(http_conn->sslContext);

            if(http_conn->sslHandle == nullptr)
                ERR_print_errors_fp(stderr);
            if(!SSL_set_fd(http_conn->sslHandle,http_conn->socket_fd))
                ERR_print_errors_fp(stderr);
            if(SSL_connect(http_conn->sslHandle)!=1) // 建立链接
                ERR_print_errors_fp(stderr);
            }else{
                err_sys("tcp Connect error");
            }

    }

    void HTTPSConnect::sslDisconnect(std::shared_ptr<connection> &disconn) const{
        if(disconn->socket_fd) close(disconn->socket_fd);
        if(disconn->sslHandle){
            SSL_shutdown(disconn->sslHandle);
            SSL_free(disconn->sslHandle);
        }
        if(disconn->sslContext) SSL_CTX_free(disconn->sslContext);
    }

    std::string HTTPSConnect::sslRead(const std::shared_ptr<connection> &c) const{
        const int readSize = 1024;
        char *rc = NULL;
        int received,count = 0;
        char buffer[1024];
        std::stringstream s_res;
        if(c){
            while(true){
                if(!rc)
                    rc = (char*)malloc(readSize*sizeof(char)+1);
                else
                {
                    rc = (char *)realloc(rc,(count+1)*readSize*sizeof(char)+1);
                }
                memset(rc,0,readSize+1);
                received = SSL_read(c->sslHandle,buffer,readSize);
                buffer[received]='\0';
    
                if(received>0)
                    strcat(rc,buffer);

                s_res<<rc; // 拼接数据
                if(received<readSize)
                    break;
                count++;
            }
        }
        std::string str_res = s_res.str();
        free(rc);
        return str_res;
    }
    // Write  to connection
    void HTTPSConnect::sslWrite(const std::shared_ptr<connection>& c,const char *text) const{
        int ret;
        if(c) ret = SSL_write(c->sslHandle,text,strlen(text));
        if(ret == -1) err_sys("SSL write error");// TODO: 直接退出有点怪 这里需要考虑一个异常的问题
    }

    // GET request
    std::string HTTPSConnect::trans_get(std::string& strurl){
        // 首先解析url
        std::string host,strResources;
        std::regex https_confirm("(http|https)://");
        strurl = std::regex_replace(strurl,https_confirm,"");
        // 这里去掉https|http
        std::regex host_regex("[a-z]{0,7}.[a-z]{0,9}(.com|.net|.edu|.gov|.int|.mil|.org|.biz)");
        std::sregex_iterator srl_chk2(strurl.begin(),strurl.end(),host_regex);
        host = (srl_chk2->begin())->str(); // TODO: 这里需要分析很多，正则

        std::regex e((srl_chk2->begin())->str());
        strResources = std::regex_replace(strurl,e,"");

        // 首先建立链接
        std::stringstream getstream;
        getstream << "GET " << strResources << " HTTP/1.1\r\n"; // 这里决定是GET请求
        getstream << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n";
        //stream << "Accept-Encoding: gzip, deflate, br\r\n";//不要编码，否则还得多一个解码的步骤
        getstream << "Accept-Language: zh-CN,zh;q=0.9\r\n";
        getstream << "Connection: Keep-Alive\r\n";
        getstream << "Host: " << host << "\r\n";
        // stream << "Referer:https://www.bzqll.com/2019/01/262.html\r\n";
        getstream << "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36\r\n";
        getstream << "\r\n";
        // connection *c;
        //c = sslConnect();
        std::string ss = getstream.str();

        const char *sendData = ss.c_str();
        const char *hostname = host.c_str();
        std::shared_ptr<connection> http_conn_get;
        sslConnect(hostname);
        sslWrite(http_conn,sendData);
        return sslRead(http_conn);
    }
}


#endif // !MUSIC_SEARCH_HTTPCONNECT_INCLUDE_HTTPSCONNECT_H_
