#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main(){
    // regex re("ab{2,5}c");
    // // smatch pieces_match;
    // string srl = "abc abbc abbbc abbbbc abbbbbc abbbbbbc";
    //  // 表达式e与目标字符序列中某个子序列间是否匹配
    // // regex_search(srl,pieces_match,re);
    // // for(auto pim:pieces_match)
    // //     cout << pim << endl;
    // sregex_iterator srl_chk(srl.begin(),srl.end(),re);
    // sregex_iterator srl_end;
    // for(;srl_chk!=srl_end;++srl_chk){
    //     cout << srl_chk->str() << endl;
    // }
    // // regex_match只考虑完全匹配

    // regex re1("a[abc]b");
    // string srl1="aab abb abb";
    // sregex_iterator srl_chk1(srl1.begin(),srl1.end(),re1);
    // sregex_iterator srl_end1;
    // for(;srl_chk1!=srl_end1;++srl_chk1){
    //     cout << srl_chk1->str() << endl;
    // }

    // regex re2("a[^abc]b");
    // string srl2="abb acb a1b a3b a4b";
    // sregex_iterator srl_chk2(srl2.begin(),srl2.end(),re2);
    // sregex_iterator srl_end2;
    // for(;srl_chk2!=srl_end2;++srl_chk2){
    //     cout << srl_chk2->str() << endl;
    // }
    // regex re2("\\d{2,5}?");
    // string srl2="123 12345 12345 3211";
    // sregex_iterator srl_chk2(srl2.begin(),srl2.end(),re2);
    // sregex_iterator srl_end2;
    // for(;srl_chk2!=srl_end2;++srl_chk2){
    //     cout << srl_chk2->str() << endl;
    // }

    // regex re3("[1][^4]\\d{9}");
    // string srl2="17600000000 13123456789 15123456789 14123456789";
    // sregex_iterator srl_chk2(srl2.begin(),srl2.end(),re3);
    // sregex_iterator srl_end2;
    // for(;srl_chk2!=srl_end2;++srl_chk2){
    //     cout << srl_chk2->str() << endl;
    // }
    
    // regex re3("^[0-9]{4}(-|\/)(0[1-9]|1[0-2])(-|\/)(0[1-9]|[12][0-9]|3[01])");
    // string srl2="2017/06/10";
    // sregex_iterator srl_chk2(srl2.begin(),srl2.end(),re3);
    // sregex_iterator srl_end2;
    // for(;srl_chk2!=srl_end2;++srl_chk2){
    //     cout << srl_chk2->str() << endl;
    // }
    
    // regex re4("[a-z]{0,7}.[a-z]{0,9}(.com|.net|.edu|.gov|.int|.mil|.org|.biz)");
    // string srl2="https://help.aliyun.com/knowledge_detail/35751.html";
    // sregex_iterator srl_chk2(srl2.begin(),srl2.end(),re4);
    // sregex_iterator srl_end2;
    // cout << (srl_chk2->begin())->str()<<endl;
    
    regex re4("(http|https)://");
    string srl2="https://aelp.aliyun.com/knowledge_detail/35751.html";
    sregex_iterator srl_chk2(srl2.begin(),srl2.end(),re4);
    sregex_iterator srl_end2;
    regex e((srl_chk2->begin())->str());
    cout << std::regex_replace(srl2,e,"")<<endl;
    
}