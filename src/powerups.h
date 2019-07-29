#ifndef POWERUP_H
#define POWERUP_H

#include "main.h"

class Powerup
{
public:
    Powerup(){};
    Powerup(float x,float y,int type);
    glm::vec3 position;
    float rotation;
    int type;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double x_speed,y_speed,radius,sx,sy;
    bool status;
private:
    VAO *ob1,*ob2,*ob3,*ob4,*ob5,*ob6,*ob7;
};

#endif // POWERUP_H