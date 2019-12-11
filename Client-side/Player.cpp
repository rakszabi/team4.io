#include "Player.h"
#include "Tile.h"
#include "Game.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include  <iostream>
#include <math.h>
using namespace std;
int Player::velocity = 10;
int RoundNum(int num, int step)
{
    if (num >= 0)
        return ((num + (step / 2)) / step) * step;
    else
        return ((num - (step / 2)) / step) * step;
}
Player::Player(int _height,int _width,string _name)
{
    srand (time(NULL));
    this->rectangle.x = RoundNum(round((int)(rand() % ((Game::getInstance()->getLevelWidth())))),Game::gameScale);
    this->rectangle.y = RoundNum(round((int)(rand() % (Game::getInstance()->getLevelHeight()))),Game::gameScale );

    if(this->rectangle.x < 0)
    {
        this->rectangle.x += Game::gameScale;
    }
    else if(this->rectangle.x > (Game::getInstance()->getLevelWidth()-Game::gameScale))
    {
        this->rectangle.x-= Game::gameScale;
    }
    if(this->rectangle.y < Game::gameScale)
    {
        this->rectangle.y+= Game::gameScale;
    }
    else if(this->rectangle.y > (Game::getInstance()->getLevelHeight()) - Game::gameScale)
    {
        this->rectangle.y -= Game::gameScale;
    }
    this->tileColor.r = (int)(rand() % (255));
    this->tileColor.g = (int)(rand() % (255));
    this->tileColor.b = (int)(rand() % (255));
    //Player's width and height
    this->rectangle.h=Game::gameScale;
    this->rectangle.w=Game::gameScale;
    gameAreaHeight = _height;
    gameAreaWidth = _width;
    this->name=_name;
    double randWay = rand()%4;
    std::cout<<randWay<<std::endl;
    if (randWay < 1)
    {
        this->dx = Player::velocity;
        this->dy = 0;
    }
    else if (randWay < 2)
    {
        this->dx = -Player::velocity;
        this->dy = 0;
    }
    else if (randWay < 3)
    {
        this->dx = 0;
        this->dy = Player::velocity;
    }
    else
    {
        this->dx = 0;
        this->dy = -Player::velocity;
    }

}

Player::~Player()
{
    //dtor
}
int Player::getX()
{
    return this->rectangle.x;
}

int Player::getY()
{
    return this->rectangle.y;
}

SDL_Color Player::getColor()
{
    return this->tileColor;
}

void Player::move()
{
    this->rectangle.x+=dx;
    this->rectangle.y+=dy;
}

void Player::die()
{
    std::cout<<"Player named: "<<this->name<<" died. The owned area was: "<<this->getPercentO()<<"%"<<std::endl;
    isAlive = false;

    Game::getInstance()->End();
    for(auto oTC :tilesO)
    {
        oTC->setOwner(nullptr);
    }

    for(auto cTC:tilesC)
    {
        cTC->setContestedO(nullptr);
    }
    this->tilesO.clear();
    this->tilesC.clear();
    this->currentTile = nullptr;
}


void Player::setTileO(Tile *t)
{
    this->tilesO.push_back(t);
    t->setOwner(this);
    t->setContestedO(nullptr);
}


void Player::removeTileO(Tile* t)
{

    for(std::vector<Tile*>::iterator it = this->tilesO.begin(); it!=tilesO.end(); it++)
    {
        if(*it==t)
        {
            this->tilesO.erase(it);
        }
    }
}


vector<Tile*> Player::getTilesO()
{
    return this->tilesO;
}

double Player::getPercentO()
{
    return (double)this->getTilesO().size()*100/(Game::getInstance()->getLevelHeight()*Game::getInstance()->getLevelWidth()/100);
}


void Player::setTileC(Tile* t)
{
    t->setContestedO(this);
    this->tilesC.push_back(t);
}


vector<Tile*> Player::getTilesC()
{
    return this->tilesC;
}


void Player::contestToO()
{
    for (auto t : this->tilesC)
    {
        this->setTileO(t);
    	t->setContestedO(nullptr);
    }
    this->tilesC.erase(this->tilesC.begin(),this->tilesC.end());
}

void Player::checkCollision(Tile* t)
{
    if(t->getContestedO() != nullptr)
    {
        t->getContestedO()->die();
    }
}


void Player::setCurrentTile(Tile *currentTile)
{
    this->currentTile = currentTile;
}


int Player::getDx()
{
    return this->dx;
}

int Player::getDy()
{
    return this->dy;
}


string Player::getName()
{
    return this->name;
}

bool Player::getAlive()
{
    return this->isAlive;
}


void Player::setAlive(bool alive)
{
    this->isAlive = alive;
}


int Player::compareTo(Player *player)
{
    return (player->getTilesO().size()<this->tilesO.size()?1:-1);
}
void Player::update()
{
            this->move();
            Game::getInstance()->getCamera()->x = ( getX() + this->rectangle.h / 2 ) - this->gameAreaWidth / 2;
            Game::getInstance()->getCamera()->y = ( getY() + this->rectangle.h / 2 ) - this->gameAreaHeight / 2;

    if( Game::getInstance()->getCamera()->x < 0 )
    {
        Game::getInstance()->getCamera()->x = 0;
    }
    if( Game::getInstance()->getCamera()->y < 0 )
    {
        Game::getInstance()->getCamera()->y = 0;
    }
    if( Game::getInstance()->getCamera()->x > Game::getInstance()->getLevelWidth() - Game::getInstance()->getCamera()->w )
    {
        Game::getInstance()->getCamera()->x = Game::getInstance()->getLevelWidth() - Game::getInstance()->getCamera()->w;
    }
    if( Game::getInstance()->getCamera()->y > Game::getInstance()->getLevelHeight() - Game::getInstance()->getCamera()->h )
    {
        Game::getInstance()->getCamera()->y = Game::getInstance()->getLevelHeight() - Game::getInstance()->getCamera()->h;
    }
            if(this->getX() < 0 || this->getX() >= Game::getInstance()->getLevelWidth() || this->getY() < 0 || this->getY() >= Game::getInstance()->getLevelHeight()){
                this->die();

            }else{
                Tile* tile = Game::getTile(this->getX(), this->getY());
                this->checkCollision(tile);
                this->setCurrentTile(tile);
                if (tile->getOwner() != this) {
                    this->setTileC(tile);
                } else if (this->getTilesC().size() > 0) {
                    this->contestToO();
                    Game::fillContested(this,this->gameAreaWidth,this->gameAreaHeight);


                }
            }
}
void Player::render(SDL_Renderer *_rend)
{
        SDL_SetRenderDrawColor( _rend, this->color.r, this->color.g,this->color.b, 255 );
        //getInstance()->getCamera()
        SDL_Rect rectangleTmp;
        rectangleTmp.x = rectangle.x-Game::getInstance()->getCamera()->x;
        rectangleTmp.h = rectangle.h;
        rectangleTmp.w = rectangle.w;
        rectangleTmp.y = rectangle.y-Game::getInstance()->getCamera()->y;
        //actual render
        SDL_RenderDrawRect(_rend,&rectangleTmp);
        SDL_RenderPresent(_rend);
}
