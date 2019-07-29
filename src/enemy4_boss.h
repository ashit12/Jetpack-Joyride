#ifndef BOSS_H
#define BOSS_H

#include "main.h"
#include "ice_balloon.h"

class Boss
{
public:
    Boss(){};
    Boss(float x,float y);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    int cd;
    int dir;
    double y_speed,angle,sx,sy;
    std::vector<Ice> ice;
private:
    VAO *obj[11];
};

#endif // BOSS_H