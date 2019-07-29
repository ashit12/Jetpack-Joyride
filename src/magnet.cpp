#include "magnet.h"
#include "main.h"

void fillcircle_mag(GLfloat arr[],int n,float x,float y,float radius)
{
    float angle = (M_PI)/2;
    for(int i=0;i<9*n;i+=9,angle+=(2*M_PI)/n)
    {
    	arr[i]=x,arr[i+1]=y,arr[i+2]=0.0f;
        arr[i+3]=x+radius*cos(angle),arr[i+4]=y+radius*sin(angle),arr[i+5]=0.0f;
        arr[i+6]=x+radius*cos(angle+(2*M_PI)/n),arr[i+7]=y+radius*sin(angle+(2*M_PI)/n),arr[i+8]=0.0f;
    }
}

Magnet::Magnet(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    int n=50;
    this->speed = back_move_limit;
    float l = 0.5f;
    this->sx=this->sy=1.0;
    this->out=l/2;
    this->in=l/2-0.1f;
    this->w=0.5f;
    GLfloat out_circle[9*n],in_circle[9*n],out_rect[]={
        0.0f,0.0f,0.0f,
        l,0.0f,0.0f,
        l,l,0.0f,
        0.0f,0.0f,0.0f,
        0.0f,l,0.0f,
        l,l,0.0f,
    }, in_rect[]={
        0.0f,0.1f,0.0f,
        l,0.1f,0.0f,
        l,0.4f,0.0f,
        0.0f,0.1f,0.0f,
        0.0f,0.4f,0.0f,
        l,0.4f,0.0f,
    };
  fillcircle_mag(out_circle,n,0.0f,0.25f,out);
  fillcircle_mag(in_circle,n,0.0f,0.25f,in);
  this->ob1 = create3DObject(GL_TRIANGLES, 150, out_circle, COLOR_RED, GL_FILL);
  this->ob2 = create3DObject(GL_TRIANGLES, 150, in_circle, COLOR_BACKGROUND, GL_FILL);
  this->ob3 = create3DObject(GL_TRIANGLES, 6, out_rect, COLOR_BLACK, GL_FILL);
  this->ob4 = create3DObject(GL_TRIANGLES, 6, in_rect, COLOR_BACKGROUND, GL_FILL);

}

void Magnet::draw(glm::mat4 VP) {
    float sz = 1.0;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->ob1);
    draw3DObject(this->ob2);
    draw3DObject(this->ob3);
    draw3DObject(this->ob4);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Magnet::tick() {
    this->set_position(this->position.x-back_move_limit,this->position.y);
}

