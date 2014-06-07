#ifndef GAME_H
#define GAME_H

#include "client.h"

class Game
{
public:
    Game(bool host);

private:
    Client* player;
    Server* server;
};

#endif // GAME_H
