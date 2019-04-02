#include "life.h"
#include "main.h"

Life::Life(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    existence = 1;
    speed = 0.01;
    /*static const GLfloat vertex_buffer_data[] = {
        -0.2, -0.2, 0, // vertex 1
        0.2,  -0.2, 0, // vertex 2
        0.2,  0.2, 0, // vertex 3

        0.2,  0.2, 0, // vertex 3
        -0.2, 0.2, 0, // vertex 4
        -0.2, -0.2, 0 // vertex 1
    };*/
    GLfloat g_vertex_buffer_data[657];
    float x1,y1;
    x1=0.1;
    y1=0.0;
    float x2=x1*0.99619469809-y1*0.08715574272,y2=x1*0.08715574274+y1*0.99619469809;
    for (int i=0; i<36; i++)
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
    x1=0.1;
    y1=0.0;
    x2=x1*0.99619469809-y1*0.08715574272;
    y2=x1*0.08715574274+y1*0.99619469809;
    for (int i=36; i<72; i++)
    {
        g_vertex_buffer_data[i*9]=0.2;
        g_vertex_buffer_data[i*9+1]=0.0f;
        g_vertex_buffer_data[i*9+2]=0.0f;
        g_vertex_buffer_data[i*9+3]=x1+0.2;
        g_vertex_buffer_data[i*9+4]=y1;
        g_vertex_buffer_data[i*9+5]=0.0f;
        g_vertex_buffer_data[i*9+6]=x2+0.2;
        g_vertex_buffer_data[i*9+7]=y2;
        g_vertex_buffer_data[i*9+8]=0.0f;
        x1=x2;
        y1=y2;
        x2=x1*0.99619469809-y1*0.08715574274;
        y2=x1*0.08715574274+y1*0.99619469809;
    }

    g_vertex_buffer_data[72*9] = -0.1;
    g_vertex_buffer_data[72*9 + 1] = 0.0;
    g_vertex_buffer_data[72*9 + 2] = 0;
    g_vertex_buffer_data[72*9 + 3] = 0.3;
    g_vertex_buffer_data[72*9 + 4] = 0.0;
    g_vertex_buffer_data[72*9 + 5] = 0.0;
    g_vertex_buffer_data[72*9 + 6] = 0.1;
    g_vertex_buffer_data[72*9 + 7] = -0.2;
    g_vertex_buffer_data[72*9 + 8] = 0.0;

    this->object = create3DObject(GL_TRIANGLES, 219, g_vertex_buffer_data, color, GL_FILL);
}

void Life::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Life::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Life::tick() {
    this->position.x += speed;
    //this->position.y -= speed;
}

bounding_box_t Life::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
