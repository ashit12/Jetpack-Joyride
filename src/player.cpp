#include "player.h"
#include "main.h"

void fillplayer(GLfloat arr[],int n,float x,float y)
{
    float angle = (2*M_PI);
	float radius = 0.1f;
    for(int j=0,i=0;i<9*n;i+=9,j++)
    {
    	arr[i]=x,arr[i+1]=y,arr[i+2]=0.0f;
        arr[i+3]=x+radius*cos(angle*j/n),arr[i+4]=y+radius*sin(angle*j/n),arr[i+5]=0.0f;
        arr[i+6]=x+radius*cos(angle*(j+1)/n),arr[i+7]=y+radius*sin(angle*(j+1)/n),arr[i+8]=0.0f;
    }
}

Player::Player(float x,float yt)
{
    this->position = glm::vec3(x, yt, 0);
    this->score=this->flag=this->inv=0;
    this->buff=0;
    this->sx=this->sy=1.0f;
    this->angle=M_PI;
    y_speed=0,x_speed=0.05;
    this->cd=0;
    life=3;
    int n=100;
    GLfloat back[]={
        0.0f,0.2f,0.0f,
        0.0f,0.4f,0.0f,
        0.2f,0.4f,0.0f,
        0.0f,0.4f,0.0f,
        0.2f,0.4f,0.0f,
        0.2f,0.2f,0.0f
    },mid[]={
        0.2f,0.2f,0.0f,
        0.2f,1.0f,0.0f,
        0.8f,1.0f,0.0f,
        0.8f,1.0f,0.0f,
        0.2f,0.2f,0.0f,
        0.8f,0.2f,0.0f
    },jet[]={
        0.0f,0.2f,0.0f,
        0.1f,0.1f,0.0f,
        0.2f,0.2f,0.0f
    },j[]={
        0.0f,0.2f,0.0f,
        0.2f,0.2f,0.2f,
        0.1f,0.0f,0.0f
    },t1[9*n],t2[9*n];
    fillplayer(t1,n,0.3f,0.1f);
    fillplayer(t2,n,0.7f,0.1f);
    color_t mi = {163,147,49};
    color_t t = {35,35,26};
    color_t b = {9,4,70};
    //Main body
    this->ob1 = create3DObject(GL_TRIANGLES, 6, back, b, GL_FILL);
    this->ob2 = create3DObject(GL_TRIANGLES, 6, mid, mi, GL_FILL);
    this->ob3 = create3DObject(GL_TRIANGLES, 300, t1, t, GL_FILL);
    this->ob4 = create3DObject(GL_TRIANGLES, 300, t2, t, GL_FILL);
    //Fire
    this->ob5 = create3DObject(GL_TRIANGLES, 3, j, COLOR_ARRAY[0], GL_FILL);    
    this->ob6 = create3DObject(GL_TRIANGLES, 3, jet, COLOR_RED, GL_FILL);

    float gap = 0.20f,y=0.3f,w=0.05f;
    for(int i=0;i<4;i++,y+=gap)
    {
        GLfloat hilt[]={
            0.8f,y,0.0f,
            0.8f,y+w,0.0f,
            0.8f+w,y,0.0f,
            0.8f,y+w,0.0f,
            0.8f+w,y,0.0f,
            0.8f+w,y+w,0.0f
        },base[]={
            0.8f+w,y-w,0.0f,
            0.8f+3*w/2,y-w,0.0f,
            0.8f+w,y+w*2,0.0f,
            0.8f+3*w/2,y-w,0.0f,
            0.8f+w,y+w*2,0.0f,
            0.8f+3*w/2,y+w*2,0.0f
        },str[]={
            0.8f+3*w/2,y,0.0f,
            0.8f+3*w/2,y+w,0.0f,
            0.95f,y,0.0f,
            0.8f+3*w/2,y+w,0.0f,
            0.95f,y,0.0f,
            0.95f,y+w,0.0f,

        },tri[]={
            0.95f,y,0.0f,
            0.95f,y+w,0.0f,
            1.0f,y+w/2,0.0f
        };
      color_t sword={239,242,239};
      color_t b = {34,116,165};
      color_t hil = {193,41,46};
      color_t bac = {252,171,100};
      this->sw[i].obj[0] = create3DObject(GL_TRIANGLES, 6, hilt, hil, GL_FILL);
      this->sw[i].obj[1] = create3DObject(GL_TRIANGLES, 6, str,  sword, GL_FILL);
      this->sw[i].obj[2] = create3DObject(GL_TRIANGLES, 6, base, b, GL_FILL);
      this->sw[i].obj[3] = create3DObject(GL_TRIANGLES, 3, tri,  sword, GL_FILL);  

    }
}
void Player::draw(glm::mat4 VP) {
    float sz = 1.0;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 scale     = glm::scale(glm::vec3(this->sx,this->sy,sz));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    Matrices.model *= (translate* scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->ob1);
    draw3DObject(this->ob2);
    draw3DObject(this->ob3);
    draw3DObject(this->ob4);
    if(this->flag)
    {
        draw3DObject(this->ob5);
        draw3DObject(this->ob6);
    }
    if(this->buff>0)
    {
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                draw3DObject(this->sw[i].obj[j]);
    }
}

void Player::set_position(float x, float y){
    this->position = glm::vec3(x, y, 0);
}

void Player::tick() {
    this->cd --;
    this->inv --;
    this->buff--;
    if(hold)
    {
        this->position.x = curx+cur_rad*cos(angle);
        this->position.y = cury+cur_rad*sin(angle);
        this->set_position(this->position.x,this->position.y);
        this->angle-=M_PI/60;
        return ;
    }
    if(this->y_speed)
        this -> y_speed -= gravity;
    this->set_position(this->position.x+this->x_speed,this->position.y+this->y_speed);
    if(this->position.x+this->x_speed <= left_limit)
        this->position.x = left_limit,this->x_speed=0.0f;
    if(this->position.x+this->x_speed >= right_limit)
        this->position.x = right_limit,this->x_speed=0.0f;
    if(this->position.y+this->y_speed <= surface_limit)
        this->position.y = surface_limit,this->y_speed=0.0f;
    if(this->position.y+this->y_speed >= sky_limit)
        this->position.y = sky_limit,this->y_speed=-0.01f;
}