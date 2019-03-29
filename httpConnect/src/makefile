vpath %.h ../include

default:search_music.o
	@g++ -std=c++11 -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -ljsoncpp -lcurl -lssl -lcrypto -o search_music.t search_music.o

search_music.o:search_music.cc songlist.h 
    g++ -std=c++11 -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -ljsoncpp -lcurl -lssl -lcrypto -c search_music.cc
.PONY:clean
clean:
	-rm search_music.t search_music.o
