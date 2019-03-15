#include "../include/httpsConnect.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;
// using namespace get_host_ip;

int main(int argc,char **argv){
    // shared_ptr<Get_ip> getip(new Get_ip());
    // // Get_ip *getip=new Get_ip();
    // vector<string> vec_ips;
    // if(argc<2) err_sys("no URL name");
    // vec_ips = getip->getHost_ip(argv[1]);
    // try{
    // for(auto ips:vec_ips){
    //     cout << ips << endl;
    // }
    // }catch(runtime_error& e){
    //     cout << "for wrong" <<endl;
    //     cout << e.what() <<endl;
    // }
    HTTPSConnect::HTTPSConnect *httpC = new HTTPSConnect::HTTPSConnect();
    string surl = "https://api.bzqll.com/music/tencent/search?key=579621905&s=123&limit=1&offset=0&type=song";
    string result = httpC->trans_get(surl);
    cout << result << endl;
    delete httpC;
    return 0;
}