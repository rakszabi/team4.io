#ifndef GAME_H
#define GAME_H
#include <string>
#include "Tile.h"
#include "Player.h"
#include "GameStateMachine.h"
#include "Client.h"
class Game
{
    public:
        virtual ~Game();
        void render();
        void update();
        //static variables
        const static int gameScale;

        string otherPlayersDir;

        bool isRunning();
        void End();
        static bool inMenu;
        static bool isConnectToServer;
        void connectToServer();
        static Tile* getTile(int x,int y);
        static void fillContested(Player* player,int gameAreaWidth,int gameAreaHeight);
        static vector<vector<Tile*>>gameArea;
        static SDL_Event e;
        SDL_Renderer* getRenderer();
        SDL_Rect* getCamera();
        int getLevelWidth();
        int getLevelHeight();
        void initBoard();
        //singleton
        static Game* getInstance();
        static Game* getInstance(int areaHeight,int areaWidth,string playerName,SDL_Window *window);
    private:
        Game(int areaHeight,int areaWidth,string playerName,SDL_Window *window);
        GameStateMachine* GSM;
        static Game * p_Instance;
        SDL_Rect* camera;
        int LEVEL_WIDTH ;
        int LEVEL_HEIGHT ;
        void startingArea(Player* player);
        int areaHeight;
        int areaWidth;
        bool quit;
        SDL_Renderer* renderer;
        vector<Player*> Players;
        Player* ThePlayer;
        Player* OtherPlayer;
        static int posX;
        static int posY;
};

#endif // GAME_H
