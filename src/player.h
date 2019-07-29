#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H

struct Sword{
    VAO *obj[4];
    bool status[4];
    Sword(){
        status[0]=status[1]=status[2]=status[3]=false;
    }
};

class Player
{
public:
    Player() {}
    Player(float x, float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    int score,cd,life,flag,inv;
    double x_speed,y_speed,sx,sy,angle;
    int buff;
private:
    VAO *ob1,*ob2,*ob3,*ob4,*ob5,*ob6;
    Sword sw[4];
};

#endif // PLAYER_H
