#ifndef TILE_H
#define TILE_H
#include <string>
#include <SDL2/SDL.h>
using namespace std;
class Player;//forward declaration mert circular dependency
class Tile
{
    public:
        Tile(int x, int y,SDL_Color _color);
        virtual ~Tile();
        SDL_Color getColor();
        Player* getContestedO();
        void setContestedO(Player *contestedO);
        Player* getOwner();
        void render();
        void update();
        void setOwner(Player *owner);
        int getX();
        int getY();
        bool operator==( Tile& rhs)
        {
            return (this->getX()==rhs.getX())&&(this->getY()==rhs.getY());
        }

        SDL_Rect rectangle;
    private:
        Player* owner;
        SDL_Color color;//alapertelmezett feher
        Player* contestedO;
        SDL_Color blendColors();
};
#endif // TILE_H
