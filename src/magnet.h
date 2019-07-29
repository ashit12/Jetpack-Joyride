#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y);
    glm::vec3 position;
    float rotation,out,in,w;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed,sx,sy;
private:
    VAO *ob1,*ob2,*ob3,*ob4;
};

#endif // MAGNET_H
