#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>
#include <time.h>
#include <math.h>

#define PORT 8080

using namespace std;

const int LEVEL_WIDTH = 2000;
const int LEVEL_HEIGHT = 2000;

void sending(char *message, int player) {
    send(player, message, strlen(message), 0);
}

string receiving(int player) {
    char buffer[1024] = {0};
    int valread = read(player, buffer, 1024);
    return buffer;
}

const int gameScale = 10;

int RoundNum(int num, int step) {
    if (num >= 0)
        return ((num + (step / 2)) / step) * step;
    else
        return ((num - (step / 2)) / step) * step;
}

int main() {

    // VÁLTOZTATOTT
    string player1_dir;
    string player2_dir;

    int server_fd, new_socket, valread;
    int player1, player2;
    string player1X, player1Y, player2X, player2Y;
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

    // Player1 kezdo pozi
    srand (time(NULL));
    int player1X_int = RoundNum(round((int)(rand() % LEVEL_WIDTH)),gameScale);
    player1X_int = 1100;
    player1X = std::to_string(player1X_int);
    //srand (time(NULL));
    int player1Y_int = RoundNum(round((int)(rand() % LEVEL_HEIGHT)),gameScale);
    player1Y_int = 1100;
    player1Y = std::to_string(player1Y_int);
    cout << "Player1 position X: " << player1X << endl;
    cout << "Player1 position Y: " << player1Y << endl;

    // Player2 kezdo pozi
    //srand (time(NULL));
    int player2X_int = RoundNum(round((int)(rand() % LEVEL_WIDTH)),gameScale);
    player2X_int = 1200;
    player2X = std::to_string(player2X_int);
    //srand (time(NULL));
    int player2Y_int = RoundNum(round((int)(rand() % LEVEL_HEIGHT)),gameScale);
    player2Y_int = 1200;
    player2Y = std::to_string(player2Y_int);
    cout << "Player2 position X: " << player2X << endl;
    cout << "Player2 position Y: " << player2Y << endl;

    usleep(3000);

    // sending main positions
    char* char_type = new char[player1X.length()];
    strcpy(char_type, player1X.c_str());
    sending(char_type, player1);

    usleep(3000);

    char_type = new char[player1Y.length()];
    strcpy(char_type, player1Y.c_str());
    sending(char_type, player1);

    usleep(3000);

    char_type = new char[player2X.length()];
    strcpy(char_type, player2X.c_str());
    sending(char_type, player2);

    usleep(3000);

    char_type = new char[player2Y.length()];
    strcpy(char_type, player2Y.c_str());
    sending(char_type, player2);

    usleep(3000);

    // sending other player positions
    char_type = new char[player2X.length()];
    strcpy(char_type, player2X.c_str());
    sending(char_type, player1);

    usleep(3000);

    char_type = new char[player2Y.length()];
    strcpy(char_type, player2Y.c_str());
    sending(char_type, player1);

    usleep(3000);

    char_type = new char[player1X.length()];
    strcpy(char_type, player1X.c_str());
    sending(char_type, player2);

    usleep(3000);

    char_type = new char[player1Y.length()];
    strcpy(char_type, player1Y.c_str());
    sending(char_type, player2);

    while(true) {
        // VÁLTOZATOTT
        player1_dir = receiving(player1);
        player2_dir = receiving(player2);
        cout << player1_dir << endl;
        cout << player2_dir << endl;

        char_type = new char[player2_dir.length()];
        strcpy(char_type, player2_dir.c_str());
        sending(char_type, player1);

        char_type = new char[player1_dir.length()];
        strcpy(char_type, player1_dir.c_str());
        sending(char_type, player2);
    }

    return 0;

}