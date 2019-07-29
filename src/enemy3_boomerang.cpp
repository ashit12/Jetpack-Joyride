#include "enemy3_boomerang.h"
#include "main.h"


void fill_boom(GLfloat arr[],int n,float x,float y,float radius)
{
    float angle = (2*M_PI);
    for(int j=0,i=0;i<9*n;i+=9,j++)
    {
    	arr[i]=x,arr[i+1]=y,arr[i+2]=0.0f;
        arr[i+3]=x+radius*cos(angle*j/n),arr[i+4]=y+radius*sin(angle*j/n),arr[i+5]=0.0f;
        arr[i+6]=x+radius*cos(angle*(j+1)/n),arr[i+7]=y+radius*sin(angle*(j+1)/n),arr[i+8]=0.0f;
    }
}

Boomerang::Boomerang(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->angle = 0;
    this->sx=this->sy=1.0;
    this->center_x=x+3.5f,this->center_y=0.0f;
    int n=100;
    float r = 0.1f;
    GLfloat top[9*n],mid[9*n],bot[9*n];
    double midx=0.0f,midy=0.0f,topx=1.0f,topy=1.0f,botx=1.0f,boty=-1.0f;
    fill_boom(mid,n,midx,midy,r);
    fill_boom(top,n,topx,topy,r);
    fill_boom(bot,n,botx,boty,r);
    GLfloat first[]={
        midx + r*cos(3*M_PI/4),midy + r*sin(3*M_PI/4),0.0f,
        midx + r*cos(7*M_PI/4),midy + r*sin(7*M_PI/4),0.0f,
        topx + r*cos(3*M_PI/4),topy + r*sin(3*M_PI/4),0.0f,
        midx + r*cos(7*M_PI/4),midy + r*sin(7*M_PI/4),0.0f,
        topx + r*cos(3*M_PI/4),topy + r*sin(3*M_PI/4),0.0f,
        topx + r*cos(7*M_PI/4),topy + r*sin(7*M_PI/4),0.0f,
    },second[]={
        midx + r*cos(M_PI/4),  midy + r*sin(M_PI/4),0.0f,
        midx + r*cos(5*M_PI/4),midy + r*sin(5*M_PI/4),0.0f,
        botx + r*cos(M_PI/4),boty + r*sin(M_PI/4),0.0f,
        midx + r*cos(5*M_PI/4),midy + r*sin(5*M_PI/4),0.0f,
        botx + r*cos(M_PI/4),boty + r*sin(M_PI/4),0.0f,
        botx + r*cos(5*M_PI/4),boty + r*sin(5*M_PI/4),0.0f,
    };
    color_t c1={99,75,102},c2={101,13,27};
    this->ob1 = create3DObject(GL_TRIANGLES, 6, first, c2, GL_FILL);
    this->ob2 = create3DObject(GL_TRIANGLES, 6, second, c2, GL_FILL);
    this->ob3 = create3DObject(GL_TRIANGLES, 300, mid, c1, GL_FILL);
    this->ob4 = create3DObject(GL_TRIANGLES, 300, bot, c1, GL_FILL);
    this->ob5 = create3DObject(GL_TRIANGLES, 300, top, c1, GL_FILL);
}

void Boomerang::draw(glm::mat4 VP) {
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
    draw3DObject(this->ob1);
    draw3DObject(this->ob2);
    draw3DObject(this->ob3);    
    draw3DObject(this->ob4);
    draw3DObject(this->ob5);
}

void Boomerang::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boomerang::tick() {
    this->set_position(this->center_x+maj_axis*cos(angle),this->center_y+min_axis*sin(angle));
    this->angle += (M_PI/180);
    this->rotation += (M_PI/180);
    this->center_x -= back_move_limit;
}

