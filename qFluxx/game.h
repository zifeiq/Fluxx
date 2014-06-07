#ifndef GAME_H
#define GAME_H

#include <string>
#include "client.h"

using std::string;

class Game
{
public:
    Game(bool host, QString name, QString ip);
    Client* getPlayer();
    Server* getServer();

private:
    Client* player;
    Server* server;
};

#endif // GAME_H
