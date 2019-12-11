#include "GameStateMachine.h"

GameStateMachine::GameStateMachine()
{
    //ctor
}

GameStateMachine::~GameStateMachine()
{
    //dtor
}

void GameStateMachine::pushState(GameState* state)
{
    this->gameStates.push(state);
    this->gameStates.top()->onEnter();
}
void GameStateMachine::popState()
{
    if(!this->gameStates.empty())
    {
        if(this->gameStates.top()->onExit())
        {
            delete this->gameStates.top();
            this->gameStates.pop();
        }
    }
}
void GameStateMachine::changeState(GameState* state)
{
    if(!this->gameStates.empty())
    {
        if(this->gameStates.top()->getStateId()==state->getStateId())
        {
            return;
        }else
        {
            this->popState();
        }
    }
    this->pushState(state);
}
