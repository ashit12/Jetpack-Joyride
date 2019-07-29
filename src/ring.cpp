#include "ring.h"
#include "main.h"

void fillcircle_ring(GLfloat arr[],int n,float x,float y,float radius)
{
    float angle = 0;
    for(int i=0;i<9*n;i+=9,angle+=(2*M_PI)/n)
    {
    	arr[i]=x,arr[i+1]=y,arr[i+2]=0.0f;
        arr[i+3]=x+radius*cos(angle),arr[i+4]=y+radius*sin(angle),arr[i+5]=0.0f;
        arr[i+6]=x+radius*cos(angle+(2*M_PI)/n),arr[i+7]=y+radius*sin(angle+(2*M_PI)/n),arr[i+8]=0.0f;
    }
}

Ring::Ring(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    int n=50;
    this->speed = back_move_limit;
    this->sx=this->sy=1.0;
    this->out=2.5f;
    this->in=1.5f;
    GLfloat out_circle[9*n],in_circle[9*n];
    fillcircle_ring(out_circle,n,0.0f,0.0f,out);
    fillcircle_ring(in_circle,n,0.0f,0.0f,in);
    this->ob1 = create3DObject(GL_TRIANGLES, 75, out_circle, {244,134,104}, GL_FILL);
    this->ob2 = create3DObject(GL_TRIANGLES, 75, in_circle, COLOR_BACKGROUND, GL_FILL);

}

void Ring::draw(glm::mat4 VP) {
    float sz = 1.0;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->ob1);
    draw3DObject(this->ob2);
}

void Ring::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ring::tick() {
    this->set_position(this->position.x-back_move_limit/5,this->position.y);
}

