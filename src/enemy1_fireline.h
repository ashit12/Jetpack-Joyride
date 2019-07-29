#ifndef FIRE_H
#define FIRE_H

#include "main.h"

class Fire
{
public:
    Fire(){};
    Fire(float x,float y,float rot);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool status;
    double speed,sx,sy,angle;
private:
    VAO *ob1,*ob2,*ob3;
};

#endif // BRICKS_H