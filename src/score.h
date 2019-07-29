#include "main.h"

#ifndef SCORE_H
#define SCORE_H

struct Base{
    VAO *ob[7];
    bool status[7];
    Base()
    {
        for(int i=0;i<7;i++)
            status[i]=true;
    }
};

class Score{
public:
    Score() {}
    Score(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    private:
        Base obj[10];
};

#endif // SCORE_H
