#include "bricks.h"
#include "main.h"

Bricks::Bricks(float x, float y,color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0.04f;
    static const GLfloat vertex_buffer_data[] = {
        -0.25f,-0.15f,0.0f,
        -0.25f,0.0f,0.0f,
        0.25f,-0.15f,0.0f,
        0.25f,0.0f,0.0f,
    };
    this->object = create3DObject(GL_TRIANGLE_STRIP, 4, vertex_buffer_data, color, GL_FILL);
}

void Bricks::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    this->sx=this->sy=1.0;
    float sz = 1.0;
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 scale     = glm::scale(glm::vec3(this->sx,this->sy,sz));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Bricks::set_position(float x, float y){
    this->position = glm::vec3(x,y,0);
    if(this->position.x<=left_limit-0.25)
        this->position = glm::vec3(right_limit+1.5,y,0);
}

void Bricks::tick() {
    this->set_position(this->position.x-back_move_limit,this->position.y);
}