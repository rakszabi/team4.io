#include "Tile.h"
#include "Player.h"
#include <iostream>
#include "Game.h"
Tile::Tile(int x, int y,SDL_Color _color)
{

    this->color.r = _color.r;
    this->color.g = _color.g;
    this->color.b = _color.b;
    this->rectangle.x = x;
    this->rectangle.y = y;
    this->rectangle.w=Game::gameScale;
    this->rectangle.h=Game::gameScale;
    this->owner=nullptr;
    this->contestedO = nullptr;
}

Tile::~Tile()
{
    //dtor

}
SDL_Color Tile::getColor()
{
    if(this->getOwner()&&!this->getContestedO())
    {
    return this->getOwner()->getColor();
    }else if(this->getOwner()&&this->getContestedO())
    {
        SDL_Color c;
        c.a = 100;
        c.r = this->getContestedO()->getColor().r;
        c.g = this->getContestedO()->getColor().g;
        c.b = this->getContestedO()->getColor().b;
    return c;
    }else if(!this->getOwner()&&this->getContestedO())
    {
        SDL_Color c;
        c.a = 100;
        c.r = this->getContestedO()->getColor().r;
        c.g = this->getContestedO()->getColor().g;
        c.b = this->getContestedO()->getColor().b;
        return c;
    }else
    {
        return this->color;
    }
}
Player* Tile::getContestedO()
{
    return this->contestedO;
}
void Tile::setContestedO(Player *contestedOwner)
{
    this->contestedO = contestedOwner;
}
Player* Tile::getOwner()
{
    return this->owner;
}
void Tile::setOwner(Player *owner)
{

    this->owner = owner;
}

int Tile::getX()
{
    return this->rectangle.x;
}

int Tile::getY()
{
    return this->rectangle.y;
}
void Tile::render()
{
        //camera
        SDL_Rect rectangleTmp;
        rectangleTmp.x = rectangle.x-Game::getInstance()->getCamera()->x;
        rectangleTmp.h = rectangle.h;
        rectangleTmp.w = rectangle.w;
        rectangleTmp.y = rectangle.y-Game::getInstance()->getCamera()->y;
        SDL_SetRenderDrawColor( Game::getInstance()->getRenderer(), this->getColor().r, this->getColor().g,this->getColor().b,100);
        SDL_RenderFillRect( Game::getInstance()->getRenderer(), &rectangleTmp );
}

