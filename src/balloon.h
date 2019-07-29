#ifndef BALLOON_H
#define BALLOON_H

#include "main.h"

class Balloon
{
public:
    Balloon(){};
    Balloon(float x,float y,color_t color);
    glm::vec3 position;
    float rotation,rad;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool status;
    double x_speed,y_speed,sx,sy;
private:
    VAO *object;
};

#endif // Balloon_H