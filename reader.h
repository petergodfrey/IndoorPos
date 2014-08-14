#ifndef READER_H
#define READER_H

#include <cstdio>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

class Reader {

    private:
        char               buffer[BUFFER_SIZE];
        int                s;             // Socket
        FILE               *in;
        struct sockaddr_in serverAddress;

    public:
        Reader(char *ipAddress, int port);
        ~Reader();
        std::string readLine();

};

#endif // READER_H
