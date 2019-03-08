#include "../include/host_ip_get.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace get_host_ip;

int main(int argc,char **argv){
    Get_ip *getip=new Get_ip();
    vector<string> vec_ips;
    if(argc<2) err_sys("no URL name");
    vec_ips = getip->getHost_ip(argv[1]);
    try{
    for(auto ips:vec_ips){
        cout << ips << endl;
    }
    }catch(runtime_error& e){
        cout << "for wrong" <<endl;
        cout << e.what() <<endl;
    }
    delete getip;
    getip = nullptr;
    return 0;
}