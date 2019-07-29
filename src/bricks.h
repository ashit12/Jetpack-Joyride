#ifndef BRICKS_H
#define BRICKS_H

#include "main.h"

class Bricks
{
public:
    Bricks(){};
    Bricks(float x,float y,color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    float sx,sy;
private:
    VAO *object;
};

#endif // BRICKS_H