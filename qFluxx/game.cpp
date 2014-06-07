#include "game.h"

Game::Game(bool host, QString name, QString ip)
{
    if(host)
        server = new Server;
    player = new Client(name,ip);
}
