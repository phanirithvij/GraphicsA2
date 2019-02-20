#pragma once
#include "main.h"

// #ifndef Volcano_H
// #define Volcano_H


class Volcano {
public:
    Volcano() {}
    Volcano(float x, float y, float z, color_t color);
    glm::vec3 position;
    glm::vec3 rotation;
    float radius;
    float height;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick(float offset);
    bool plane_on_my_top();
private:
    VAO *object;
};

// #endif // Volcano_H
