#include "enemy1_fireline.h"
#include "main.h"

void ffillcircle(GLfloat arr[],int n,float x,float y)
{
    float angle = (2*M_PI);
	float radius = 0.2f;
    for(int j=0,i=0;i<9*n;i+=9,j++)
    {
    	arr[i]=x,arr[i+1]=y,arr[i+2]=0.0f;
        arr[i+3]=x+radius*cos(angle*j/n),arr[i+4]=y+radius*sin(angle*j/n),arr[i+5]=0.0f;
        arr[i+6]=x+radius*cos(angle*(j+1)/n),arr[i+7]=y+radius*sin(angle*(j+1)/n),arr[i+8]=0.0f;
    }
}


Fire::Fire(float x, float y, float rot) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rot;
    int n=100;
    this->sx=this->sy=1.0;
    speed = 0.04f;
    this->status=true;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat top_circle[9*n],bottom_circle[9*n],rectangle[]={
        -1.2f,0.125f,0.0f,
        -1.2f,-0.125f,0.0f,
        1.2f,0.125f,0.0f,
        -1.2f,-0.125f,0.0f,
        1.2f,0.125f,0.0f,
        1.2f,-0.125f,0.0f
    };
    ffillcircle(top_circle,n,-1.2f,0.0f);
    ffillcircle(bottom_circle,n,1.2f,0.0f);
    for(int j=0;j<3*n;j+=3)
    {
       // first_circle[j]-=0.1f,first_circle[j+1]+=0.1f;
       // second_circle[j]+=2.1f,second_circle[j+1]+=0.1f;
    }
    this->ob1 = create3DObject(GL_TRIANGLES, 300, top_circle, COLOR_ARRAY[0], GL_FILL);
    this->ob2 = create3DObject(GL_TRIANGLES, 300, bottom_circle, COLOR_ARRAY[0], GL_FILL);
    this->ob3 = create3DObject(GL_TRIANGLES, 6, rectangle, COLOR_RED, GL_FILL);
}

void Fire::draw(glm::mat4 VP) {
    if(this->status==false)
        return ;
    float sz = 1.0;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 0, 1));
    glm::mat4 scale     = glm::scale(glm::vec3(this->sx,this->sy,sz));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->ob3);
    draw3DObject(this->ob1);
    draw3DObject(this->ob2);
}

void Fire::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Fire::tick() {
    this->set_position(this->position.x-back_move_limit,this->position.y);
}

