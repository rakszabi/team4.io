#include "Game.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <time.h>
#include <map>
#include <sstream>

stringstream ss;

//b static variables
vector<vector<Tile*>>Game::gameArea;
SDL_Event Game::e;
Game* Game::p_Instance = nullptr;
const int Game::gameScale = 10;
bool Game::inMenu = true;
bool Game::isConnectToServer = false;
//e static variables
std::string random_id( size_t length )
{
    srand (time(NULL));
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

Game::Game(int areaHeight,int areaWidth,string playerName,SDL_Window *window)
{
    // posX = 100;
    // posY = 100;
    renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
    this->areaHeight = areaHeight;
    this->areaWidth = areaWidth;
    this->quit = false;

    this->LEVEL_WIDTH = 2000;
    this->LEVEL_HEIGHT = 2000;
    (this->camera) =new SDL_Rect{0,0,640,480};
    GSM = new GameStateMachine();
    std::cout<<"Game successfully constructed with "<<LEVEL_WIDTH<<"*"<<LEVEL_HEIGHT<<" size map!"<<std::endl;

//    for(int i = 0;i<1;i++)
//    {
//        this->Players.push_back(new Player(areaHeight,areaWidth,random_id(10)));
//    }


}
Game* Game::getInstance(int areaHeight,int areaWidth,string playerName,SDL_Window *window)
{
    if(!p_Instance)
    {
        p_Instance = new Game(areaHeight,areaWidth,playerName,window);
        return p_Instance;
    }else
    {
        cout<<"Game is already constructed please use the other overloaded function!"<<endl;
        return nullptr;
    }

}
Game* Game::getInstance()
{
    if(!p_Instance)
    {
        cout<<"Game is not initialized please provide initializing variables"<<endl;
        return nullptr;
    }else
    {
        return p_Instance;
    }
}
Game::~Game()
{
    //TODO not all memory freed
    for(auto &k:gameArea)
    {
        for(auto &p:k)
        {
            delete p;
        }
    }
    delete ThePlayer;
    SDL_DestroyRenderer(this->renderer);

}
bool Game::isRunning()
{
    return !quit;
}
void Game::End()
{
    this->quit=true;
}
void Game::initBoard()
{
    std::cout<<"Board is initializing..."<<std::endl;
    this->ThePlayer = new Player(areaHeight, areaWidth,random_id(30));
    this->OtherPlayer = new Player(areaHeight, areaWidth, random_id(30));

    //render tiles for the first time && get starting zone around  player...
    for(int i = 0; i < this->LEVEL_HEIGHT/Game::gameScale; i++)
    {
        Game::gameArea.push_back(vector<Tile*>(this->LEVEL_WIDTH/Game::gameScale));

        for(int j = 0; j < this->LEVEL_WIDTH/Game::gameScale; j++)
        {
            Game::gameArea[i][j]=(new Tile(j*Game::gameScale,i*Game::gameScale,{255,255,255}));

        }
    }
//    for(auto k:Players)
//    {
//        startingArea(k);
//    }
//startingArea(this->ThePlayer);

    std::cout<<"Board is successfully initialized!"<<std::endl;
}
//draws 1-1 tile-owned around player
void Game::startingArea(Player* player)
{
    int x = player->getX();
    int y = player->getY();

    for(int i = x-Game::gameScale; i <= x+Game::gameScale; i+=Game::gameScale)
    {
        for(int j = y-Game::gameScale; j <= y+Game::gameScale; j+=Game::gameScale)
        {
            if((j>0&&j<this->LEVEL_HEIGHT)&&(i>0&&i<this->LEVEL_WIDTH))
            {

            player->setTileO(getTile(i,j));
            }
        }
    }
}

Tile* Game::getTile(int x,int y)
{
  //get the indexed tile on (x,y) divided by scale
    return Game::gameArea.at(y/Game::gameScale).at(x/Game::gameScale);
}
void Game::render()
{

    //b render Tiles
    //(DONE)TODO render optimization to render only what is visible for player instead of whole map
    //loop through tiles skip those with smaller or bigger value of y and x (in perspective of player)

 for(auto &p:gameArea)
    {

        vector<Tile*> tmp(p.begin(),p.end());
        //check if the y coordinate of the tile vector column visible to player
        //pYp is the bottom Y value pYm is the top (screen)
            int pYp=ThePlayer->getY()+(Game::getInstance()->getCamera()->h/2);
            int pYm=ThePlayer->getY()-(Game::getInstance()->getCamera()->h/2);
            //the actual row's Y value
            int kY=tmp[0]->getY();

            //making test variables  match real values if out of bounds
            if(pYp>Game::LEVEL_HEIGHT)
            {
                pYm -=(pYp-Game::LEVEL_HEIGHT);
                pYp = Game::LEVEL_HEIGHT;
            }
            if(pYm<0)
            {
                pYp+=(-1*pYm);
                pYm=0;
            }

            //if y is in bound proceed with render those x tiles <=&&=> player x
            if((kY<=pYp)&&(kY>=pYm))
            {
        for(auto k:tmp)
        {

        //check if the x coordinate of the tile vector row visible to player
            int kX=k->getX();
            int pXp=ThePlayer->getX()+(Game::getInstance()->getCamera()->w/2);
            int pXm=ThePlayer->getX()-(Game::getInstance()->getCamera()->w/2);
            if(pXp>Game::LEVEL_WIDTH)
            {
                pXm -=(pXp-Game::LEVEL_WIDTH);
                pXp = Game::LEVEL_WIDTH;

            }
            if(pXm<0)
            {
                pXp+=(-1*pXm);
                pXm=0;
            }
            if((kX>=pXm)&&(kX<=pXp))
            {
            k->render();
            }
        }
            }
    }

    //e render Tiles

    //b render Player
    //this->OtherPlayer->render(this->renderer);
    this->ThePlayer->render(this->renderer);
    //this->OtherPlayer->render(this->renderer);
    //e render Player


}

void Game::fillContested(Player* player,int gameAreaWidth,int gameAreaHeight) {
    //get the boundary rectangle of owned territory
        int maxX = 0;
        int minX = gameAreaWidth;
        int maxY = 0;
        int minY = gameAreaHeight;
        for (auto t : player->getTilesO()) {
            if(t->getX() > maxX) maxX = t->getX();
            if(t->getX() < minX) minX = t->getX();
            if(t->getY() > maxY) maxY = t->getY();
            if(t->getY() < minY) minY = t->getY();
        }
        //checking if there is open space in owned tiles horizontally/vertically
    map<Tile*,int> needToFillMap;
    for(auto i = minY;i<=maxY;i+=Game::gameScale)
    {
            for(auto j= minX;j<=maxX;j+=Game::gameScale)
            {
                Tile *tmpTile = Game::getTile(j,i);

                if((tmpTile->getOwner()==player))
                {
                    if(Game::getTile(j+Game::gameScale,i)->getOwner()==nullptr)
                    {

                        Tile* startNode = Game::getTile(j,i);
                        while(Game::getTile(j+Game::gameScale,i)->getOwner()==nullptr&&(j+Game::gameScale)<=maxX)
                        {
                        j+=Game::gameScale;
                        }
                        Tile* endNode = Game::getTile(j+Game::gameScale,i);


                        if(endNode->getOwner()!=nullptr||endNode->getOwner()==player)
                        {
                            for(auto z = startNode->getX();z<=endNode->getX();z+=Game::gameScale)
                              {
                                needToFillMap[Game::getTile(z,i)]++;
                              }
                        }
                    }
                }


            }
    }

    for(auto i = minX;i<=maxX;i+=Game::gameScale)
    {
            for(auto j= minY;j<=maxY;j+=Game::gameScale)
            {
                Tile *tmpTile = Game::getTile(i,j);

                if((tmpTile->getOwner()==player))
                {
                    if(Game::getTile(i,j+Game::gameScale)->getOwner()==nullptr)
                    {
                        Tile* startNode = Game::getTile(i,j);
                        while(Game::getTile(i,j+Game::gameScale)->getOwner()==nullptr&&(j+Game::gameScale)<=maxY)
                        {
                        j+=Game::gameScale;
                        }
                        Tile* endNode = Game::getTile(i,j+Game::gameScale);


                        if(endNode->getOwner()!=nullptr||endNode->getOwner()==player)
                        {
                            for(auto z = startNode->getY();z<=endNode->getY();z+=Game::gameScale)
                              {
                                needToFillMap[Game::getTile(i,z)]++;
                              }
                        }
                    }
                }


            }
    }

    for(auto t:needToFillMap)
    {
        if(t.second>1)
        {

        player->setTileO(t.first);
        }
    }
}

//Main game logic
void Game::update()
{
    // this->ThePlayer->changePosition(this->posX, this->posY);

        while( SDL_PollEvent(&Game::e)!=0)
        {
            if( e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if(e.type == SDL_KEYDOWN)
            {
                //if it is the opposite direction skip
                switch(e.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if(this->ThePlayer->dx != Player::velocity)
                    {
                    this->ThePlayer->dx = -Player::velocity;
                    this->ThePlayer->dy= 0;
                    Client::changeDirection("l");
                    }
                    break;
                case SDLK_RIGHT:
                    if(this->ThePlayer->dx != -Player::velocity)
                    {
                    this->ThePlayer->dx= Player::velocity;
                    this->ThePlayer->dy=0;
                    Client::changeDirection("r");
                    }
                    break;
                case SDLK_UP:
                    if(this->ThePlayer->dy != Player::velocity)
                    {
                    this->ThePlayer->dx= 0;
                    this->ThePlayer->dy=-Player::velocity;
                    Client::changeDirection("u");
                    }
                    break;
                case SDLK_DOWN:
                    if(this->ThePlayer->dy != -Player::velocity)
                    {
                    this->ThePlayer->dx= 0;
                    this->ThePlayer->dy=Player::velocity;
                    Client::changeDirection("d");
                    }
                    break;

                default:
                    break;
                }
            }

        }

        if(Game::getInstance()->otherPlayersDir == "u") {
            if(this->OtherPlayer->dy != Player::velocity) {
                // cout << "fel" << endl;
                this->OtherPlayer->dx = 0;
                this->OtherPlayer->dy = -Player::velocity;
            }
        }
        if(Game::getInstance()->otherPlayersDir == "d") {
            if(this->OtherPlayer->dy != -Player::velocity) {
                // cout << "le" << endl;
                this->OtherPlayer->dx = 0;
                this->OtherPlayer->dy = Player::velocity;
            }
        }
        if(Game::getInstance()->otherPlayersDir == "l") {
            if(this->OtherPlayer->dx != Player::velocity) {
                // cout << "balra" << endl;
                this->OtherPlayer->dx = -Player::velocity;
                this->OtherPlayer->dy = 0;
            }
        }
        if(Game::getInstance()->otherPlayersDir == "r") {
            if(this->OtherPlayer->dx != -Player::velocity) {
                // cout << "jobbra" << endl;
                this->OtherPlayer->dx = Player::velocity;
                this->OtherPlayer->dy = 0;
            }
        }

//        for(auto t:this->Players)
//    {
//        t->update();
//    }
this->OtherPlayer->update();
this->ThePlayer->update();
//this->OtherPlayer->update();
}
SDL_Renderer* Game::getRenderer()
{
    return this->renderer;
}

SDL_Rect* Game::getCamera()
{
    return this->camera;
}

int Game::getLevelWidth()
{
    return this->LEVEL_WIDTH;
}

int Game::getLevelHeight()
{
    return this->LEVEL_WIDTH;
}

void Game::connectToServer() {
    Game::isConnectToServer = true;
    cout << "Connected" << endl;
    static int mainPosX, mainPosY, otherPosX, otherPosY;

    ss << Client::messageReceiver();
    ss >> mainPosX;
    ss.clear();
    ss << Client::messageReceiver();
    ss >> mainPosY;
    cout << "Main position X: " << mainPosX << endl;
    cout << "Main position Y: " << mainPosY << endl;
    ThePlayer->changePosition(mainPosX, mainPosY);
    startingArea(ThePlayer);

    ss.clear();
    ss << Client::messageReceiver();
    ss >> otherPosX;
    ss.clear();
    ss << Client::messageReceiver();
    ss >> otherPosY;
    cout << "Other player position X: " << otherPosX << endl;
    cout << "Other player position Y: " << otherPosY << endl;
    OtherPlayer->changePosition(otherPosX, otherPosY);
    
    startingArea(OtherPlayer);
}