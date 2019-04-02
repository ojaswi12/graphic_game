#include "main.h"

#ifndef Magnet_H
#define Magnet_H


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t colormagnet, color_t colorback, color_t colorsquare);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    bool existence;
    bounding_box_t bounding_box();
private:
    VAO *object1;
    VAO *object2;
    VAO *object3;
};

#endif // Magnet_H
