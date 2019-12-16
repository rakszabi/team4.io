#include <SDL2/SDL.h>
#include <iostream>
#include "Button.h"
#include "Game.h"
using namespace std;
const int WIDTH = 640;
const int HEIGHT = 480;

int main(int argc,char* args[])
{


    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Team4io",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT,
                                          0);


    Game::getInstance(HEIGHT,WIDTH,"dummy",window);
    Game::getInstance()->initBoard();
    Button button(WIDTH/2-150,HEIGHT/3,300,50);


    while(Game::getInstance()->isRunning())
    {
                SDL_SetRenderDrawColor( Game::getInstance()->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear(  Game::getInstance()->getRenderer() );
        if(Game::inMenu || !Game::isConnectToServer)
        {
            while(SDL_PollEvent(&Game::e))
            {

                button.handle_events();
                if( Game::e.type == SDL_QUIT)
                    {
                        Game::getInstance()->End();

                    }


            }
            button.show();
        }else
        {

                Game::getInstance()->update();
                Game::getInstance()->render();
                Client::sendDirection();
                SDL_Delay(300);

        }
    }
    delete Game::getInstance();
    SDL_Quit();
    SDL_DestroyWindow(window);

    return 0;
}
