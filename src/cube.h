#pragma once
#include "main.h"

// #ifndef CUBE_H
// #define CUBE_H


class Cube {
public:
    Cube() {}
    Cube(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick(float offset);
    double speed;
private:
    VAO *object;
};

// #endif // CUBE_H
