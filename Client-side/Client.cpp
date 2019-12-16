#include "Client.h"
#include "Game.h"

#define PORT 8080

using namespace std;

int sock = 0;
char *dir = "t";

void Client::buildConnect() {
    sock = 0;
    int valread;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //Connect IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        cout << "Invalid address or address not supported" << endl;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection failed" << endl;
    }
}

void Client::messageSender(char *message) {
    send(sock, message, strlen(message), 0);
}

void Client::changeDirection(char *_dir) {
    dir = _dir;
}

void Client::sendDirection() {
    send(sock, dir, strlen(dir), 0);
}

string Client::messageReceiver() {
    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    return buffer;
}