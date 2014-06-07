#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include "cardLib.h"

class Client
{
public:
    Client(String name);
private:
    vector<Card*> myHands;

};

#endif // CLIENT_H
