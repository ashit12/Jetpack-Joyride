#ifndef LIFE_H
#define LIFE_H

#include "main.h"

struct va{
    VAO *obj[3];
};

class Life
{
public:
    Life(){};
    Life(float x,float y,color_t c);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    va heart[5];
};

#endif // BRICKS_H