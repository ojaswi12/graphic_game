#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color, int size, int slope) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    existence = true;
    speed = 0.01;
    if (slope == 0)
    {
        GLfloat g_vertex_buffer_data[648];
        float x1,y1;
        if (size==1)
        {
            x1=0.2;
            y1=0.2;
        }
        else
        {
            x1=0.3;
            y1=0.3;
        }
        float x2=x1*0.99619469809-y1*0.08715574272,y2=x1*0.08715574274+y1*0.99619469809;
        for (int i=0;i<72;i++)
        {
            g_vertex_buffer_data[i*9]=0.0f;
            g_vertex_buffer_data[i*9+1]=0.0f;
            g_vertex_buffer_data[i*9+2]=0.0f;
            g_vertex_buffer_data[i*9+3]=x1;
            g_vertex_buffer_data[i*9+4]=y1;
            g_vertex_buffer_data[i*9+5]=0.0f;
            g_vertex_buffer_data[i*9+6]=x2;
            g_vertex_buffer_data[i*9+7]=y2;
            g_vertex_buffer_data[i*9+8]=0.0f;
            x1=x2;
            y1=y2;
            x2=x1*0.99619469809-y1*0.08715574274;
            y2=x1*0.08715574274+y1*0.99619469809;
        }

        this->object = create3DObject(GL_TRIANGLES, 216, g_vertex_buffer_data, color, GL_FILL);
    }
    else
    {
        GLfloat g_vertex_buffer_data[666];
        float x1,y1;
        if (size==1)
        {
            x1=0.2;
            y1=0.2;
        }
        else
        {
            x1=0.3;
            y1=0.3;
        }
        float x2=x1*0.99619469809-y1*0.08715574272,y2=x1*0.08715574274+y1*0.99619469809;
        for (int i=0;i<72;i++)
        {
            g_vertex_buffer_data[i*9]=0.0f;
            g_vertex_buffer_data[i*9+1]=0.0f;
            g_vertex_buffer_data[i*9+2]=0.0f;
            g_vertex_buffer_data[i*9+3]=x1;
            g_vertex_buffer_data[i*9+4]=y1;
            g_vertex_buffer_data[i*9+5]=0.0f;
            g_vertex_buffer_data[i*9+6]=x2;
            g_vertex_buffer_data[i*9+7]=y2;
            g_vertex_buffer_data[i*9+8]=0.0f;
            x1=x2;
            y1=y2;
            x2=x1*0.99619469809-y1*0.08715574274;
            y2=x1*0.08715574274+y1*0.99619469809;
        }
        g_vertex_buffer_data[648]=0.5;
        g_vertex_buffer_data[649]=0.3;
        g_vertex_buffer_data[650]=0.0;
        g_vertex_buffer_data[651]=0.5;
        g_vertex_buffer_data[652]=0.4;
        g_vertex_buffer_data[653]=0.0;
        g_vertex_buffer_data[654]=-0.5;
        g_vertex_buffer_data[655]=0.4;
        g_vertex_buffer_data[656]=0.0;

        g_vertex_buffer_data[657]=0.5;
        g_vertex_buffer_data[658]=0.3;
        g_vertex_buffer_data[659]=0.0;
        g_vertex_buffer_data[660]=-0.5;
        g_vertex_buffer_data[661]=0.4;
        g_vertex_buffer_data[662]=0.0;
        g_vertex_buffer_data[663]=-0.5;
        g_vertex_buffer_data[664]=0.3;
        g_vertex_buffer_data[665]=0.0;

        this->object = create3DObject(GL_TRIANGLES, 222, g_vertex_buffer_data, color, GL_FILL);
    } 
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x += speed;
    //this->position.y -= speed;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
