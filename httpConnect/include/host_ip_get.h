#ifndef MUSIC_SEARCH_HOST_IP_GET_H_
#define MUSIC_SEARCH_HOST_IP_GET_H_
#include "apue.h"
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <vector>

namespace get_host_ip{
    class Get_ip{
        public:
            Get_ip() = default;
            std::vector<std::string> getHost_ip(const char* hostname);
        private:
            struct hostent *hp;
            std::vector<std::string> vec_ip;
    };

   std::vector<std::string> Get_ip::getHost_ip(const char* hostname){
       if((hp=gethostbyname(hostname))==NULL) err_sys("get hostname error");
       int x=0;
       while(hp->h_addr_list[x]!=NULL){
        vec_ip.push_back(inet_ntoa(*((in_addr*)hp->h_addr_list[x])));
        ++x;
    }
    return vec_ip;
   }
}

#endif // !MUSIC_SEARCH_HOST_IP_GET_H_
