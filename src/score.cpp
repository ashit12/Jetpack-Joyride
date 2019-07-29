#include "score.h"
#include "main.h"

void fill_rect(GLfloat arr[],float cx,float cy,int type){
    if(type==0)
    {
        GLfloat temp[]={
            cx,cy,0.0f,
            cx+0.25f/screen_zoom,cy,0.0f,
            cx,cy-0.05f/screen_zoom,0.0f,
            cx+0.25f/screen_zoom,cy,0.0f,
            cx,cy-0.05f/screen_zoom,0.0f,
            cx+0.25f/screen_zoom,cy-0.05f/screen_zoom,0.0f
        };
        for(int i=0;i<18;i++)
            arr[i]=temp[i];
    }
    else
    {
        GLfloat temp[]={
            cx,cy,0.0f,
            cx+0.05f/screen_zoom,cy,0.0f,
            cx,cy-0.25f/screen_zoom,0.0f,
            cx+0.05f/screen_zoom,cy,0.0f,
            cx,cy-0.25f/screen_zoom,0.0f,
            cx+0.05f/screen_zoom,cy-0.25f/screen_zoom,0.0f
        };
        for(int i=0;i<18;i++)
            arr[i]=temp[i];
    }
    
}
Score::Score(float x, float y, color_t c1) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat arr[18];
    float cx = 0.0f, cy = 0.0f, maj_len=0.2f/screen_zoom;
    for(int i=0;i<10;i++,cx+=2*maj_len)
    {
        fill_rect(arr,cx,cy,0);
        this->obj[i].ob[0] = create3DObject(GL_TRIANGLES, 6, arr, c1, GL_FILL);
        
        fill_rect(arr,cx+maj_len,cy,1);
        this->obj[i].ob[1] = create3DObject(GL_TRIANGLES, 6, arr, c1, GL_FILL);
        
        fill_rect(arr,cx+maj_len,cy-maj_len,1);
        this->obj[i].ob[2] = create3DObject(GL_TRIANGLES, 6, arr, c1, GL_FILL);
        
        fill_rect(arr,cx,cy-2*maj_len,0);
        this->obj[i].ob[3] = create3DObject(GL_TRIANGLES, 6, arr, c1, GL_FILL);
        
        fill_rect(arr,cx,cy-maj_len,1);
        this->obj[i].ob[4] = create3DObject(GL_TRIANGLES, 6, arr, c1, GL_FILL);
        
        fill_rect(arr,cx,cy,1);
        this->obj[i].ob[5] = create3DObject(GL_TRIANGLES, 6, arr, c1, GL_FILL);
        
        fill_rect(arr,cx,cy-maj_len,0);
        this->obj[i].ob[6] = create3DObject(GL_TRIANGLES, 6, arr, c1, GL_FILL);
    }
}

void Score::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // glm::mat4 scale     = glm::scale(glm::vec3(1.0/screen_zoom,1.0/screen_zoom,1.0f));
    Matrices.model *= (translate * rotate );
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]); 
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<7;j++)
        {
            if(this->obj[i].status[j]==true)
                draw3DObject(this->obj[i].ob[j]);
        }
    }

}

void Score::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Score::tick() {
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<7;j++)
        {
            if(vec[i][j]=='1')
                this->obj[i].status[j]=true;
            else
                this->obj[i].status[j]=false;            
        }
    }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<7;j++)
        {
            if(vec[i][j]=='1')
                this->obj[i].status[j]=true;
            else
                this->obj[i].status[j]=false;            
        }
    }
    this->set_position(this->position.x,this->position.y);
}

