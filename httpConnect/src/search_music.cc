#include "../include/songlist.h"

#include <iostream>
#include <map>
#include <tuple>

using namespace std;

int main(){
    cout << "请输入你需要查询的歌曲名字: ";
    string song_name;
    while(cin>>song_name){
        songlist::Songlist_Get *song_get = new songlist::Songlist_Get();
        map<int,tuple<string,string,string>> map_res = song_get->getsong_lists(song_name);
        for(int i = 0;i<map_res.size();i++){
            cout << i << "-" << get<1>(map_res[i]) << "-" << get<2>(map_res[i])<<endl;
        }
        cout << "请输入你需要下载的歌曲的序号" << endl; //  这里FIXME: 这里需要考虑一下用户查不到歌曲的时候，需要一轮
        int isongnum;
        while(cin>>isongnum){
            if(isongnum<map_res.size()){
                song_get->down_to_load(get<0>(map_res[isongnum]),get<1>(map_res[isongnum]));
                break;
            }
        }
        break;
    }
    return 0;
}