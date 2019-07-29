#include "coin.h"
#include "main.h"

void fill(GLfloat arr[],int n)
{
    float angle = (2*M_PI);
	float radius = 0.1f;
    for(int j=0,i=0;i<9*n;i+=9,j++)
    {
    	arr[i]=arr[i+1]=arr[i+2]=0.0f;
        arr[i+3]=radius*cos(angle*j/n),arr[i+4]=radius*sin(angle*j/n),arr[i+5]=0.0f;
        arr[i+6]=radius*cos(angle*(j+1)/n),arr[i+7]=radius*sin(angle*(j+1)/n),arr[i+8]=0.0f;
    }
}

Coin::Coin(float x, float y, int type) {
    this->position = glm::vec3(x, y, 0);
    this->status=true;
    this->rotation=0;
    this->sx=this->sy=1.0;
    color_t color;
    if(type==0)
        val=1,color={193,223,31};
    else
        val=5,color={0,117,242};
    float sz = 1.0;
    rad = 0.1f;
    speed = 0.04f;
    cd=0;
    int n=100;
    GLfloat vertex_buffer_data[9*n];
    fill(vertex_buffer_data,n);
    this->object = create3DObject(GL_TRIANGLES, 300, vertex_buffer_data, color, GL_FILL);
}

void Coin::draw(glm::mat4 VP) {
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
    draw3DObject(this->object);
}

void Coin::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Coin::tick() {
    this->set_position(this->position.x-back_move_limit,this->position.y);
    this->rotation += M_PI/60;
}

