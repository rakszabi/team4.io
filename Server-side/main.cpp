#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>
#include <time.h>

#define PORT 8080

using namespace std;

void sending(char *message, int player) {
    send(player, message, strlen(message), 0);
}

string receiving(int player) {
    char buffer[1024] = {0};
    int valread = read(player, buffer, 1024);
    return buffer;
}

int main() {
    int server_fd, new_socket, valread;
    int player1, player2;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cout << "Socket failed" << endl;
        return 0;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        cout << "setsockopt" << endl;
        return 0;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
		cout << "bind failed" << endl;
		return 0;
	}

	if (listen(server_fd, 3) < 0) {
		cout << "listen" << endl;
		return 0;
	}

    // Player 1 connect
    if ((player1 = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) {
		cout << "accept" << endl;
		return 0;
	}

    // Player 2 connect
    if ((player2 = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) {
		cout << "accept" << endl;
		return 0;
	}

    cout << receiving(player1) << endl;
    cout << receiving(player2) << endl;

    usleep(1000000);

    sending("Welcome", player1);
    sending("Welcome", player2);

    usleep(100000);

    while(true) {
        cout << receiving(player1) << endl;
        cout << receiving(player2) << endl;
    }

    return 0;

}