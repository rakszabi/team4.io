#ifndef PLAYER_H
#define PLAYER_H
#include <SDL2/SDL.h>
#include <string>
#include <vector>
using namespace std;
class Tile;//forward declaration mert circular dependency
class Player
{
public:
    Player(int _height,int _width,string Name);
    virtual ~Player();

    SDL_Rect rectangle;//Player's x,y height and width
    int gameAreaHeight;
    int gameAreaWidth;
    int dx;
    int dy;
    string name;
    int getX();
    int getY();
    SDL_Color getColor();
    void move();
    void die();
    void setTileO(Tile *t);
    void removeTileO(Tile *t);
    vector<Tile*> getTilesO();
    double getPercentO();
    void setTileC(Tile* t);
    vector<Tile*> getTilesC();
    void contestToO();
    void checkCollision(Tile* t);
    void setCurrentTile(Tile *currentTile);
    int getDx();
    int getDy();
    string getName();
    bool getAlive();
    void update();
    void render(SDL_Renderer* renderer);
    void setAlive(bool alive);
    int compareTo(Player *player);//compares two player's owned territory
    bool operator!=(Player *other)
    {
        return !(this->getName() == other->getName());
    }
    static int velocity;
private:
    vector<Tile*> tilesC;
    vector<Tile*> tilesO;
    SDL_Color color={255,0,0};
    SDL_Color tileColor;
    Tile* currentTile;
    bool isAlive = true;
};

#endif // PLAYER_H
