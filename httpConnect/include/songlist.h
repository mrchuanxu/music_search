#ifndef MUSIC_SEARCH_HTTPCONNECT_INCLUDE_SONGLIST_H_
#define MUSIC_SEARCH_HTTPCONNECT_INCLUDE_SONGLIST_H_

#include "httpsConnect.h"
#include <json/json.h>
#include <memory>
#include <map>
#include <string>
#include <regex>
#include <json/json.h>
#include <sstream>
#include <tuple>
#include <curl/curl.h>

namespace songlist
{
class Songlist_Get
{
  public:
    Songlist_Get() : httpC(new HTTPSConnect::HTTPSConnect())
    {
    }
    // id与name 这里可以构建trie树 传值 右值
    void song_lists(std::string &);
    std::map<int, std::tuple<std::string, std::string,std::string>> &getsong_lists(std::string &);
    void gbkencode(std::string &); // 将中文编码
    unsigned char ToHex(unsigned char x);
    static size_t download_to_file(void *pBuffer,size_t nSize,size_t nMemByte,FILE* fp);
    void down_to_load(std::string&,std::string&);
  private:
    std::shared_ptr<HTTPSConnect::HTTPSConnect> httpC;
    std::map<int, std::tuple<std::string,std::string,std::string>> map_res;
    std::string songfile_name;
};
void Songlist_Get::song_lists(std::string &song_name)
{
    gbkencode(song_name);
    std::string surl = "https://api.bzqll.com/music/tencent/search?key=579621905&s=" + song_name + "&limit=10&offset=0&type=song";

    std::string result = httpC->trans_get(surl);
    // 去除响应头
    std::regex response_re(".*\r\n");
    std::sregex_iterator srl_chk(result.begin(), result.end(), response_re);
    std::sregex_iterator srl_end;
    std::stringstream ss_stream;

    for (; srl_chk != srl_end; srl_chk++)
        ss_stream << srl_chk->str(); // 拼接响应头

    std::string ss = ss_stream.str();

    // 去除响应头正则
    std::regex response_rm(ss);

    std::string strResources = std::regex_replace(result, response_rm, "");
    Json::Reader reader;
    Json::Value value;
    // 读data数据
    if (reader.parse(strResources, value))
    {
        if (value["result"].asString() == "SUCCESS")
        {
            const Json::Value arrayObj = value["data"];
            for (int i = 0; i < arrayObj.size(); i++)
            {
                std::get<0>(map_res[i]) = arrayObj[i]["id"].asString();
                std::get<1>(map_res[i]) = arrayObj[i]["name"].asString();
                std::get<2>(map_res[i]) = arrayObj[i]["singer"].asString();
            }
        }
        else
        {
            err_msg("sorry,connect error,please try again");
        }
    }
}
std::map<int, std::tuple<std::string, std::string,std::string>> &Songlist_Get::getsong_lists(std::string &song_name)
{
    song_lists(song_name);
    return map_res;
}
void Songlist_Get::gbkencode(std::string &str)
{
    std::string strTemp = "";
    for (int i = 0; i < str.size(); i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    str = strTemp;
}

unsigned char Songlist_Get::ToHex(unsigned char x)
{
    return x > 9 ? x + 55 : x + 48;
}

void Songlist_Get::down_to_load(std::string& id_str,std::string& song_name){
    // 把下载到的数据以追加的方式写入文件(一定要有a，否则前面写入的内容就会被覆盖了)
	FILE* fp = NULL;
    std::string filename = song_name+".mp3"; // 赋予名字
    const char* file_name = filename.c_str(); // 转换string
	fp = fopen(file_name, "ab+"); // 追加输入 后面要做判断文件是否存在
    CURL* curl = curl_easy_init();
    std::string songResourcesUrl = "https://api.bzqll.com/music/tencent/url?key=579621905&id="+id_str+"&br=192";
    curl_easy_setopt(curl,CURLOPT_URL,songResourcesUrl.c_str());
    // 设置接收数据
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,download_to_file);//  再开一个线程下载

    // 设置写入文件描述符
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
    // 设置重定向最大次数
    curl_easy_setopt(curl,CURLOPT_MAXREDIRS,5);
    // 设置301 302重定向
    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);
    curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0);

    // 执行请求
    CURLcode retcCode = curl_easy_perform(curl);
    const char *pError = curl_easy_strerror(retcCode);
    // 清理curl
    fclose(fp);
    curl_easy_cleanup(curl);
}

size_t Songlist_Get::download_to_file(void *pBuffer,size_t nSize,size_t nMemByte,FILE* fp){
    size_t nWrite = fwrite(pBuffer, nSize, nMemByte, fp);
	return nWrite;
}

} // namespace songlist
#endif // !MUSIC_SEARCH_HTTPCONNECT_INCLUDE_SONGLIST_H_
