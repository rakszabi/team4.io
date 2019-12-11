#ifndef CLIENT_H
#define CLIENT_H
#include <SDL2/SDL.h>

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <time.h>
//#include <unistd.h>
#include <string>


class Client
{
    public:
        // Client();
        // virtual ~Client();
        static void buildConnect();
        static void messageSender(char *message);
        static std::string messageReceiver();
        // static std::string messageReceive;
        // static void messageReceiver();
        // static int sock;
        // static int valread;
        // static char* name;
        // static char* direction;
        // static char buffer[1024];

    private:


};

#endif // CLIENT_H
