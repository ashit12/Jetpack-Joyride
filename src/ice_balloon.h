#ifndef ICE_BALLOON_H
#define ICE_BALLOON_H

#include "main.h"

class Ice
{
public:
    Ice(){};
    Ice(float x,float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool status;
    double x_speed,y_speed,sx,sy;
private:
    VAO *object;
};

#endif // Balloon_H