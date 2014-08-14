#include "reader.h"

#include <iostream>
#include <cstdlib>
#include <cctype>

Reader::Reader(char *ipAddress, int port) {
    // Create socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        std::cerr << "ERROR opening socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    // Set server address
    memset( &serverAddress, 0, sizeof(serverAddress) );
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ipAddress);
    serverAddress.sin_port        = htons(port);
    // Connect to socket
    if ( connect(s, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0 ) {
        std::cerr << "ERROR connecting to server" << std::endl;
        exit(EXIT_FAILURE);
    }
    // Open socket as a file
    in = fdopen(s, "r");
}

Reader::~Reader() {
    fclose(in);
}

std::string Reader::readLine() {
    while ( fgets(buffer, BUFFER_SIZE, in) == NULL || isalpha(buffer[0]) ) {
    }
    std::string l(buffer);
    std::cout << l << std::endl;
    return l;
}
