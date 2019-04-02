#include "main.h"

#ifndef LIFE_H
#define LIFE_H


class Life {
public:
    Life() {}
    Life(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    int existence;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // LIFE_H
