#include "enemy4_boss.h"
#include "main.h"


void fill_boss(GLfloat arr[],int n,float x,float y,float radius)
{
    float angle = (2*M_PI);
    for(int j=0,i=0;i<9*n;i+=9,j++)
    {
    	arr[i]=x,arr[i+1]=y,arr[i+2]=0.0f;
        arr[i+3]=x+radius*cos(angle*j/n),arr[i+4]=y+radius*sin(angle*j/n),arr[i+5]=0.0f;
        arr[i+6]=x+radius*cos(angle*(j+1)/n),arr[i+7]=y+radius*sin(angle*(j+1)/n),arr[i+8]=0.0f;
    }
}

Boss::Boss(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->angle = 0;
    this->dir = -1;
    this->sx=this->sy=1.0;
    this->y_speed=0.05f;
    this->cd=0;
    int n=100;
    float in_rad=0.25f,top_rad=0.05f,wid=0.3f,thick=0.05f,len=0.2f;
    GLfloat top[9*n],mid[9*n];
    fill_boss(mid,n,0.0,0.0,in_rad);
    fill_boss(top,n,0.0,0.35,top_rad);
    GLfloat midr[]={
        -wid,wid,0.0f,
        -wid,-wid,0.0f,
        wid,wid,0.0f,
        -wid,-wid,0.0f,
        wid,wid,0.0f,
        wid,-wid,0.0f
    },topt[]={
        0.1f,wid,0.0f,
        -0.1f,wid,0.0f,
        0.0f,0.5f,0.0f
    },leftl[]={
        -wid/2-thick,-wid,0.0f,
        -wid/2+thick,-wid,0.0f,
        -wid/2-thick,-wid-len,0.0f,
        -wid/2+thick,-wid,0.0f,
        -wid/2-thick,-wid-len,0.0f,
        -wid/2+thick,-wid-len,0.0f
    },rightl[]={
        wid/2-thick,-wid,0.0f,
        wid/2+thick,-wid,0.0f,
        wid/2-thick,-wid-len,0.0f,
        wid/2+thick,-wid,0.0f,
        wid/2-thick,-wid-len,0.0f,
        wid/2+thick,-wid-len,0.0f
    },leftf[]={
        -wid/2+2*thick,-wid-len/1.5,0.0f,
        -wid/2-2*thick,-wid-len/1.5,0.0f,
        -wid/2,-wid-len/0.5,0.0f
    },rightf[]={
        wid/2+2*thick,-wid-len/1.5,0.0f,
        wid/2-2*thick,-wid-len/1.5,0.0f,
        wid/2,-wid-len/0.5,0.0f
    },leftr[]={
        -wid,2*thick,0.0f,
        -wid,-2*thick,0.0f,
        -wid-0.1,-2*thick-0.1f,0.0f,
        -wid,-2*thick,0.0f,
        -wid-0.1,-2*thick-0.1f,0.0f,
        -wid-0.12,-2*thick-0.15f,0.0f
    },sh[]={
        -wid-0.1,-0.3,0.0f,
        -wid-0.2,0.5,0.0f,
        -wid-0.2,-0.3,0.0f,
        -wid-0.1,-0.3,0.0f,
        -wid-0.2,0.5,0.0f,
        -wid-0.1,0.5,0.0f
    };
    color_t c1 = {5,5,23}, c2={234,214,55}, c3={250,121,33} ,c4={79,56,36};
    this->obj[0] = create3DObject(GL_TRIANGLES, 6, midr, c1, GL_FILL);
    this->obj[2] = create3DObject(GL_TRIANGLES, 3, topt, c1, GL_FILL);
    this->obj[4] = create3DObject(GL_TRIANGLES, 6, leftl, c1, GL_FILL);
    this->obj[5] = create3DObject(GL_TRIANGLES, 6,  rightl, c1, GL_FILL);
    this->obj[6] = create3DObject(GL_TRIANGLES, 6,  leftr, c1, GL_FILL);
    
    this->obj[7] = create3DObject(GL_TRIANGLES, 6,  sh, c4, GL_FILL);
    
    this->obj[3] = create3DObject(GL_TRIANGLES, 300, top, c2, GL_FILL);
    this->obj[1] = create3DObject(GL_TRIANGLES, 300, mid, c2, GL_FILL);

    this->obj[9] = create3DObject(GL_TRIANGLES, 3,  leftf, c3, GL_FILL);
    this->obj[10] = create3DObject(GL_TRIANGLES, 3,  rightf, c3, GL_FILL);


}

void Boss::draw(glm::mat4 VP) {
    float sz = 1.0;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 scale     = glm::scale(glm::vec3(this->sx,this->sy,sz));
    Matrices.model *= (translate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if(this->y_speed<0)
    {
        draw3DObject(this->obj[9]);
        draw3DObject(this->obj[10]);
    }
    for(int i=0;i<8;i++)
        draw3DObject(this->obj[i]);
}

void Boss::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boss::tick() {
    this->cd--;
    this->set_position(this->position.x-back_move_limit/4,this->position.y-this->y_speed);
    if(this->position.y-0.5f<=surface_limit)
        this->y_speed *= -1, this->position.y=surface_limit+0.5f;
    if(this->position.y+0.25f>=sky_limit)
        this->y_speed *= -1, this->position.y=sky_limit-0.5f;
}

