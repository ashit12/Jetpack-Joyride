#ifndef PARALLELFIRE_H
#define PARALLELFIRE_H

#include "main.h"

class ParallelFire
{
public:
    ParallelFire(){};
    ParallelFire(float x,float y);
    glm::vec3 position;
    float rotation,rad;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool status;
    double x_speed,y_speed,sx,sy;
private:
    VAO *ob1,*ob2,*ob3;
};

#endif // BRICKS_H