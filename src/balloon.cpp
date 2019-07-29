#include "balloon.h"
#include "main.h"

void fill_balloon(GLfloat arr[],int n)
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

Balloon::Balloon(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->status=true;
    rad=0.1f;
    x_speed = 0.05f;
    y_speed = 0.05f; 
    int n=100;
    GLfloat vertex_buffer_data[9*n];
    fill_balloon(vertex_buffer_data,n);
    this->object = create3DObject(GL_TRIANGLES, 300, vertex_buffer_data, color, GL_FILL);
}

void Balloon::draw(glm::mat4 VP) {
    if(this->status==false)
        return ;
    // float sz = 1.0;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // glm::mat4 scale     = glm::scale(glm::vec3(this->sx,this->sy,sz));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Balloon::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Balloon::tick() {
    this->position.x += x_speed;
    float x = this->position.x;
    this->position.y += y_speed;
    if(this->position.y<=surface_limit)
        this->status=false;
   y_speed -= gravity;
   this->set_position(this->position.x,this->position.y);
}

