#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <iostream>
#include <time.h>
#include <math.h>
#include <sstream>

#define PORT 8080

using namespace std;

const int LEVEL_WIDTH = 2000;
const int LEVEL_HEIGHT = 2000;

double posMatrix[LEVEL_WIDTH/10][LEVEL_HEIGHT/10];

void checkArea(double player){
    double isInArea = false;
    for(int i=0; i<LEVEL_HEIGHT/10; i++){
        for(int j=0; j<LEVEL_WIDTH/10; j++){
            // if(isInArea)
            //     posMatrix[i][j] = player;
            // if(posMatrix[i][j] == (player+0.1) || posMatrix[i][j] == (player)){
            //     posMatrix[i][j] = player;
            //     isInArea = !isInArea;
            // }
            if (posMatrix[i][j] == player && !isInArea || posMatrix[i][j] == player+0.1 && !isInArea) {
                isInArea = true;
            } else if (posMatrix[i][j] == player && isInArea || posMatrix[i][j] == player+0.1 && isInArea) {
                isInArea = false;
            }

            if(isInArea) {
                posMatrix[i][j] = player;
            }

        }
    }
}

int scoreCalc(double player){
    int score = 0;
    for(int i=0; i<LEVEL_HEIGHT/10; i++){
        for(int j=0; j<LEVEL_WIDTH/10; j++){
            if(posMatrix[i][j] == player){
                score++;
            }
        }
    }
    return score;
}

void sending(char *message, int player) {
    send(player, message, strlen(message), 0);
}

void sending(string message, int player) {
    char* char_type = new char[message.length()];
    strcpy(char_type, message.c_str());
    sending(char_type, player);
}

void sending(int message, int player) {
    stringstream ss;
    ss << message;
    string messageString;
    ss >> messageString;
    char* char_type = new char[messageString.length()];
    strcpy(char_type, messageString.c_str());
    sending(char_type, player);
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
    int server_fd, new_socket, valread;
    int player1, player2;
    string player1X, player1Y, player2X, player2Y, player1_dir, player2_dir;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    bool winP1, winP2 = false;
    char buffer[1024] = {0};

    for(int i=0; i<LEVEL_HEIGHT/10; i++){
        for(int j=0; j<LEVEL_WIDTH/10; j++){
            posMatrix[i][j] = 0.0;
        }
    }

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
    int player1X_int = 1000;
    player1X = std::to_string(player1X_int);
    int player1X_int2 = player1X_int/10;      // le kell osztani tízzel mert a mátrix 200x200-as
    //srand (time(NULL));
    int player1Y_int = 1000;
    player1Y = std::to_string(player1Y_int);
    int player1Y_int2 = player1Y_int/10;

    for(int i=player1X_int2-2; i<player1X_int2+2; i++){
        for(int j=player1Y_int2-2; j<player1Y_int2+2; j++){
            posMatrix[i][j] = 1.1;
        }
    }
    cout << "Player1 position X: " << player1X << endl;
    cout << "Player1 position Y: " << player1Y << endl;

    // Player2 kezdo pozi
    //srand (time(NULL));
    int player2X_int = 1300;             //RoundNum(round((int)(rand() % (LEVEL_WIDTH-300))),gameScale);
    player2X = std::to_string(player2X_int);
    int player2X_int2 = player2X_int/10;
    //srand (time(NULL));
    int player2Y_int = 1300;             //RoundNum(round((int)(rand() % (LEVEL_HEIGHT-300))),gameScale);
    player2Y = std::to_string(player2Y_int);
    int player2Y_int2 = player2Y_int/10;

    for(int i=player2X_int2-2; i<player2X_int2+2; i++){
        for(int j=player2Y_int2-2; j<player2Y_int2+2; j++){
            posMatrix[i][j] = 2.1;
        }
    }
    cout << "Player2 position X: " << player2X << endl;
    cout << "Player2 position Y: " << player2Y << endl;

    // sending main positions
    usleep(3000);
    sending(player1X, player1);

    usleep(3000);
    sending(player1Y, player1);

    usleep(3000);
    sending(player2X, player2);

    usleep(3000);
    sending(player2Y, player2);

    // sending other player positions
    usleep(3000);
    sending(player2X, player1);

    usleep(3000);
    sending(player2Y, player1);

    usleep(3000);
    sending(player1X, player2);

    usleep(3000);
    sending(player1Y, player2);

    while(true) {
        player1_dir = receiving(player1);
        player2_dir = receiving(player2);
        cout << player1_dir << endl;
        cout << player2_dir << endl;

        if(player1_dir == "u"){
            player1X_int2 -= 1;
            if(player1X_int2 == 0 || posMatrix[player1X_int2][player1Y_int2] == 1.2)
                winP2 = true;
            else if(posMatrix[player1X_int2][player1Y_int2] == 2.2)
                winP1 = true;
            else if(posMatrix[player1X_int2][player1Y_int2] == 1.1)
                checkArea(1.1);
            else
                posMatrix[player1X_int2][player1Y_int2] = 1.2;
        }
        else if(player1_dir == "d"){
            player1X_int2 += 1;
            if(player1X_int2 == 200 || posMatrix[player1X_int2][player1Y_int2] == 1.2)
                winP2 = true;
            else if(posMatrix[player1X_int2][player1Y_int2] == 2.2)
                winP1 = true;
            else if(posMatrix[player1X_int2][player1Y_int2] == 1.1)
                checkArea(1.1);
            else
                posMatrix[player1X_int2][player1Y_int2] = 1.2;
        }
        else if(player1_dir == "l"){
            player1Y_int2 -= 1;
            if(player1Y_int2 == 0 || posMatrix[player1X_int2][player1Y_int2] == 1.2)
                winP2 = true;
            else if(posMatrix[player1X_int2][player1Y_int2] == 2.2)
                winP1 = true;
            else if(posMatrix[player1X_int2][player1Y_int2] == 1.1)
                checkArea(1.1);
            else
                posMatrix[player1X_int2][player1Y_int2] = 1.2;
        }
        else if(player1_dir == "r"){
            player1Y_int2 += 1;
            if(player1Y_int2 == 200 || posMatrix[player1X_int2][player1Y_int2] == 1.2)
                winP2 = true;
            else if(posMatrix[player1X_int2][player1Y_int2] == 2.2)
                winP1 = true;
            else if(posMatrix[player1X_int2][player1Y_int2] == 1.1)
                checkArea(1.1);
            else
                posMatrix[player1X_int2][player1Y_int2] = 1.2;
        }
        cout << "tailPos player1: " << player1X_int2 << ":" << player1Y_int2 << endl;

        if(player2_dir == "u"){
            player2X_int2 -= 1;
            if(player2X_int2 == 0 || posMatrix[player2X_int2][player2Y_int2] == 2.2)
                winP1 = true;
            else if(posMatrix[player2X_int2][player2Y_int2] == 1.2)
                winP2 = true;
            else if(posMatrix[player2X_int2][player2Y_int2] == 1.1)
                checkArea(2.1);
            else
                posMatrix[player2X_int2][player2Y_int2] = 2.2;
        }
        else if(player2_dir == "d"){
            player2X_int2 += 1;
            if(player2X_int2 == 200 || posMatrix[player2X_int2][player2Y_int2] == 2.2)
                winP1 = true;
            else if(posMatrix[player2X_int2][player2Y_int2] == 1.2)
                winP2 = true;
            else if(posMatrix[player2X_int2][player2Y_int2] == 1.1)
                checkArea(2.1);
            else
                posMatrix[player2X_int2][player2Y_int2] = 2.2;
        }
        else if(player2_dir == "l"){
            player2Y_int2 -= 1;
            if(player2Y_int2 == 0 || posMatrix[player2X_int2][player2Y_int2] == 2.2)
                winP1 = true;
            else if(posMatrix[player2X_int2][player2Y_int2] == 1.2)
                winP2 = true;
            else if(posMatrix[player2X_int2][player2Y_int2] == 1.1)
                checkArea(2.1);
            else
                posMatrix[player2X_int2][player2Y_int2] = 2.2;
        }
        else if(player2_dir == "r"){
            player2Y_int2 += 1;
            if(player2Y_int2 == 200 || posMatrix[player2X_int2][player2Y_int2] == 2.2)
                winP1 = true;
            else if(posMatrix[player2X_int2][player2Y_int2] == 1.2)
                winP2 = true;
            else if(posMatrix[player2X_int2][player2Y_int2] == 1.1)
                checkArea(2.1);
            else
                posMatrix[player2X_int2][player2Y_int2] = 2.2;
        }
        cout << "tailPos player2: " << player2X_int2 << ":" << player2Y_int2 << endl;

        if (winP1)
        {
            cout << "player1 won" << endl;
            sending("win", player1);
            sending("loose", player2);
            sending(scoreCalc(1.1), player1);
            sending(scoreCalc(2.1), player1);
            return 0;
        }
        else if (winP2)
        {
            cout << "player2 won" << endl;
            sending("win", player2);
            sending("loose", player1);
            sending(scoreCalc(2.1), player2);
            sending(scoreCalc(1.1), player2);
            return 0;
        }

        sending(player2_dir, player1);
        sending(player1_dir, player2);
    }

    return 0;

}