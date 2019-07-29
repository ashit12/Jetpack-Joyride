#include "enemy2_parallelfireline.h"
#include "main.h"

void fillcircle(GLfloat arr[],int n,float x,float y,float radius)
{
    float angle = (2*M_PI);
    for(int j=0,i=0;i<9*n;i+=9,j++)
    {
    	arr[i]=x,arr[i+1]=y,arr[i+2]=0.0f;
        arr[i+3]=x+radius*cos(angle*j/n),arr[i+4]=y+radius*sin(angle*j/n),arr[i+5]=0.0f;
        arr[i+6]=x+radius*cos(angle*(j+1)/n),arr[i+7]=y+radius*sin(angle*(j+1)/n),arr[i+8]=0.0f;
    }
}

ParallelFire::ParallelFire(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    int n=100;
    this->status=true;
    this->sx=this->sy=1.0;
    y_speed = 0.05;
    x_speed = 0.04f;
    this->rad=0.2f;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat first_circle[9*n],second_circle[9*n],rectangle[]={
        0.0f,0.0f,0.0f,
        0.0f,0.25f,0.0f,
        2.0f,0.25f,0.0f,
        0.0f,0.0f,0.0f,
        2.0f,0.0f,0.0f,
        2.0f,0.25f,0.0f
    };
    fillcircle(first_circle,n,-0.2f,0.1f,rad);
    fillcircle(second_circle,n,2.2f,0.1f,rad);
    for(int j=0;j<3*n;j+=3)
    {
       // first_circle[j]-=0.1f,first_circle[j+1]+=0.1f;
       // second_circle[j]+=2.1f,second_circle[j+1]+=0.1f;
    }
    this->ob1 = create3DObject(GL_TRIANGLES, 300, first_circle, COLOR_ARRAY[0], GL_FILL);
    this->ob2 = create3DObject(GL_TRIANGLES, 300, second_circle, COLOR_ARRAY[0], GL_FILL);
    this->ob3 = create3DObject(GL_TRIANGLES, 6, rectangle, COLOR_RED, GL_FILL);
}

void ParallelFire::draw(glm::mat4 VP) {
    if(this->status==false)
        return ;
    float sz = 1.0;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    glm::mat4 scale     = glm::scale(glm::vec3(this->sx,this->sy,sz));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->ob1);
    draw3DObject(this->ob2);
    draw3DObject(this->ob3);
}

void ParallelFire::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void ParallelFire::tick() {
    float end_correction_up=-0.7f,end_correction_down=-0.2f;
    if(this->position.y+end_correction_up>=sky_limit || this->position.y+end_correction_down<=surface_limit)
        y_speed *= -1;
    this->set_position(this->position.x-back_move_limit,this->position.y+y_speed);
}

