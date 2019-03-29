# music_search
http->music-search-api->trie tree->fstream
### JSON正则获取 参考掘金链接
[JS正则表达式略长](https://juejin.im/post/5965943ff265da6c30653879)
### TCP-SSL-HTTP HTTPS构造过程 socket文件描述符传输信息
配套算法，随机数，get请求获取资源
### 下载，参考libcurl库设计 线程下载
curl - MAC 需要引入 brew install curl
### 后续参考，多线程多并发考虑

#### 编译参考
`g++ -g -std=c++11  search_music.cc -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -ljsoncpp -lcurl  -lssl -lcrypto -o test.out`

后面直接添加了makefile，直接make也可以


