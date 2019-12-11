#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.h"


class PlayState : public GameState
{
    public:
        PlayState();
        virtual ~PlayState();
        virtual void update();
        virtual void render();
        virtual bool onEnter();
        virtual bool onExit();
        virtual std::string getStateId()const{return playID;};
    private:
        const static std::string playID;
};

#endif // PLAYSTATE_H
