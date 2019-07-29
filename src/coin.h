#ifndef COIN_H
#define COIN_H

#include "main.h"

class Coin
{
public:
    Coin(){};
    Coin(float x,float y,int type);
    glm::vec3 position;
    int cd,val;
    float rotation,rad;
    bool status;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed,sx,sy;
private:
    VAO *object;
};

#endif // COIN_H
