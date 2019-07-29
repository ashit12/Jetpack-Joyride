#include "main.h"

#ifndef RING_H
#define RING_H


class Ring {
public:
    Ring() {}
    Ring(float x, float y);
    glm::vec3 position;
    float rotation,w;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed,sx,sy,in,out;
private:
    VAO *ob1,*ob2;
};

#endif // RING_H
