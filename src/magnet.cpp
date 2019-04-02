#include "magnet.h"
#include "main.h"

Magnet::Magnet(float x, float y, color_t colormagnet, color_t colorback, color_t colorsquare) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    existence = true;
    speed = 0.01;
    GLfloat g_vertex_buffer_data[324];
    float x1 =0,y1=-0.5;
    float x2=x1*0.99619469809-y1*0.08715574272,y2=x1*0.08715574274+y1*0.99619469809;
    for (int i=0;i<36;i++)
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

    this->object1 = create3DObject(GL_TRIANGLES, 108, g_vertex_buffer_data, colormagnet, GL_FILL);
    x1 =0;
    y1=-0.3;
    x2=x1*0.99619469809-y1*0.08715574272;
    y2=x1*0.08715574274+y1*0.99619469809;
    for (int i=0;i<36;i++)
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
    this->object2 = create3DObject(GL_TRIANGLES, 108, g_vertex_buffer_data, colorback, GL_FILL);
    GLfloat vertex_buffer_data[]={
        0.0, 0.3, 0,
        0.0, 0.5, 0,
        -0.2, 0.3, 0,

       -0.2, 0.5, 0,
       -0.2, 0.3, 0,
       0.0, 0.5, 0,
    
       0.0, -0.3, 0,
       0.0, -0.5, 0,
       -0.2, -0.3, 0,

       -0.2, -0.5, 0,
       -0.2, -0.3, 0,
       0.0, -0.5, 0
    };
    this->object3 = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data, colorsquare, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Magnet::tick() {
    this->position.x += speed;
    //this->position.y -= speed;
}

bounding_box_t Magnet::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
