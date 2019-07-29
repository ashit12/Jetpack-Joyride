#ifndef BOOMERANG_H
#define BOOMERANG_H

#include "main.h"

class Boomerang
{
public:
    Boomerang(){};
    Boomerang(float x,float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed,angle,sx,sy;
    double center_x,center_y;
private:
    VAO *ob1,*ob2,*ob3,*ob4,*ob5,*ob6,*ob7;
};

#endif // BOOMERANG_H