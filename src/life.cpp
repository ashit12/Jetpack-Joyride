#include "life.h"
#include "main.h"

void fill_life(GLfloat arr[],int n,float radius,float x,float y)
{
    float angle = (2*M_PI);
    for(int j=0,i=0;i<9*n;i+=9,j++)
    {
    	arr[i]=x,arr[i+1]=y,arr[i+2]=0.0f;
        arr[i+3]=x+radius*cos(angle*j/n),arr[i+4]=y+radius*sin(angle*j/n),arr[i+5]=0.0f;
        arr[i+6]=x+radius*cos(angle*(j+1)/n),arr[i+7]=y+radius*sin(angle*(j+1)/n),arr[i+8]=0.0f;
    }
}

Life::Life(float x, float y,color_t c) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    int n=100;
    float cx=0.0f;
    for(int i=0;i<max_life;i++,cx+=0.6f)
    {
        GLfloat left[9*n],right[9*n];
        float rad = 0.15f;
        fill_life(left,n,rad/screen_zoom,(cx+rad*cos(3*M_PI/4))/screen_zoom,rad*sin(3*M_PI/4)/screen_zoom);
        fill_life(right,n,rad/screen_zoom,(cx+rad*cos(M_PI/4))/screen_zoom,rad*sin(M_PI/4)/screen_zoom);
        color_t col_out_cir = {209,190,207};
        GLfloat tri[]={
            cx+0.0f,-0.3f,0.0f,
            cx+0.222f,0.0f,0.0f,
            cx-0.222f,0.0f,0.0f
        };
        for(int i=0;i<9;i++)
            tri[i]/=screen_zoom;
        this->heart[i].obj[0] = create3DObject(GL_TRIANGLES, 300,left, c, GL_FILL);
        this->heart[i].obj[1] = create3DObject(GL_TRIANGLES, 300,right, c, GL_FILL);
        this->heart[i].obj[2] = create3DObject(GL_TRIANGLES, 3,tri, c, GL_FILL);
    } 
}

void Life::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    for(int i=0;i<cur_life;i++)
    {
        for(int j=0;j<3;j++)
            draw3DObject(this->heart[i].obj[j]);
    }
}

void Life::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Life::tick() {
    this->set_position(this->position.x,this->position.y);
}

