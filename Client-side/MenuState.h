#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"
#include "Button.h"

class MenuState : public GameState
{
    public:
        MenuState();
        virtual ~MenuState();
        virtual void update();
        virtual void render();
        virtual bool onEnter();
        virtual bool onExit();
        virtual std::string getStateId()const{return menuID;};
    private:
        const static std::string menuID;
        Button* startButton;
        Button* exitButton;
};

#endif // MENUSTATE_H
