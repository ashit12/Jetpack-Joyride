#include "powerups.h"
#include "main.h"

void fill_pow(GLfloat arr[],int n,float radius,float x=0.0f,float y=0.0f)
{
    float angle = (2*M_PI);
    for(int j=0,i=0;i<9*n;i+=9,j++)
    {
    	arr[i]=x,arr[i+1]=y,arr[i+2]=0.0f;
        arr[i+3]=x+radius*cos(angle*j/n),arr[i+4]=y+radius*sin(angle*j/n),arr[i+5]=0.0f;
        arr[i+6]=x+radius*cos(angle*(j+1)/n),arr[i+7]=y+radius*sin(angle*(j+1)/n),arr[i+8]=0.0f;
    }
}

Powerup::Powerup(float x, float y,int t) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->type=t;
    x_speed = 0.03f;
    y_speed = 0.04f;
    status=true;
    int n=100;
    radius=0.4f;
    GLfloat circle[9*n];
    fill_pow(circle,n,radius);
    this->sx=this->sy=1.0;    
    //Extra coins
    if(type == 0)
    {
        GLfloat in[9*n],inn[9*n];
        fill_pow(in,n,radius-0.05);
        fill_pow(inn,n,radius-0.06);
        color_t col_out_cir = {61,49,91};
        color_t col_in_cir = {248,249,145};
        this->ob1 = create3DObject(GL_TRIANGLES, 300,circle, col_in_cir, GL_FILL);
        this->ob2 = create3DObject(GL_TRIANGLES, 300,in, COLOR_BLACK, GL_FILL);
        this->ob3 = create3DObject(GL_TRIANGLES, 300,inn, col_in_cir, GL_FILL);
    }
    
    //Extra life
    else if(type==1)
    {
        GLfloat left[9*n],right[9*n];
        float rad = 0.15f;
        fill_pow(left,n,rad,rad*cos(3*M_PI/4),rad*sin(3*M_PI/4));
        fill_pow(right,n,rad,rad*cos(M_PI/4),rad*sin(M_PI/4));
        color_t col_out_cir = {209,190,207};
        color_t col_heart = {205, 16, 93};
        GLfloat tri[]={
            0.0f,-0.3f,0.0f,
            0.222f,0.0f,0.0f,
            -0.222f,0.0f,0.0f
        };
        this->ob1 = create3DObject(GL_TRIANGLES, 300,circle, col_out_cir, GL_FILL);
        this->ob2 = create3DObject(GL_TRIANGLES, 300,left, col_heart, GL_FILL);
        this->ob3 = create3DObject(GL_TRIANGLES, 300,right, col_heart, GL_FILL);
        this->ob4 = create3DObject(GL_TRIANGLES, 3,tri, col_heart, GL_FILL);  

    }
    
    // Sword and shield
    else if(type==2)
    {
      GLfloat hilt[]={
          -0.05f,-0.15f,0.0f,
          0.05f,-0.15f,0.0f,
          -0.05f,-0.35f,0.0f,
          0.05f,-0.15f,0.0f,
          -0.05f,-0.35f,0.0f,
          0.05f,-0.35f,0.0f
      },base[]={
          -0.2f,-0.10f,0.0f,
          0.2f,-0.10f,0.0f,
          -0.2f,-0.15f,0.0f,
          0.2f,-0.10f,0.0f,
          -0.2f,-0.15f,0.0f,
          0.2f,-0.15f,0.0f
      },str[]={
          -0.05f,-0.15f,0.0f,
          -0.05f,0.30f,0.0f,
          0.05f,0.30f,0.0f,
          -0.05f,-0.15f,0.0f,
          0.05f,0.30f,0.0f,
          0.05f,-0.15f,0.0f
      },tri[]={
          -0.05f,0.30f,0.0f,
          0.05f,0.30f,0.0f,
          0.0f,0.35f,0.0f
      };
      color_t sword={239,242,239};
      color_t b = {34,116,165};
      color_t hil = {193,41,46};
      color_t bac = {252,171,100};
      this->ob1 = create3DObject(GL_TRIANGLES, 300,circle, bac, GL_FILL);
      this->ob2 = create3DObject(GL_TRIANGLES, 6, hilt, hil, GL_FILL);
      this->ob3 = create3DObject(GL_TRIANGLES, 6, str,  sword, GL_FILL);
      this->ob4 = create3DObject(GL_TRIANGLES, 6, base, b, GL_FILL);
      this->ob5 = create3DObject(GL_TRIANGLES, 3, tri,  sword, GL_FILL);  
    }
}

void Powerup::draw(glm::mat4 VP) {
    if(this->status==false)
        return ;
    float sz = 1.0;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 1, 0));
    glm::mat4 scale     = glm::scale(glm::vec3(this->sx,this->sy,sz));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->ob1);
    draw3DObject(this->ob2);
    draw3DObject(this->ob3);
    if(this->type)
    {
       draw3DObject(this->ob4);
        if(this->type>1)
           draw3DObject(this->ob5);
    }
}

void Powerup::set_position(float x, float y){
    this->position = glm::vec3(x,y,0);

}

void Powerup::tick() {
    this->set_position(this->position.x-back_move_limit-x_speed,this->position.y+this->y_speed);
    this->y_speed-=gravity;
    if(this->position.y-this->radius<=surface_limit)
        this->y_speed *= -1, this->position.y = surface_limit+radius;
    if(this->position.y>=sky_limit)
        this->y_speed *= -1, this->position.y = sky_limit-radius;
    this -> rotation += M_PI/60;
}